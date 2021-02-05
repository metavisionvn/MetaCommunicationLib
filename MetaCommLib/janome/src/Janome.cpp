#include <metacommlib/janome/Janome.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/janome/JanomePosition.h>
#include <metacommlib/TcpClient.h>

namespace mtcl {

Janome::Janome()
    : IRobot()
    , mIPAddress("")
    , mPort(0)
    , mDecoder(this)
    , mEncoder()
    , mSocket(nullptr)
    , mRobotInformation()
    , mIsStopping(false)
    , mSocketDescriptor(0)
    , mSpeedLevel(0)
    , mJoggingThread(nullptr)
    , mReturnHomeStatus(JRHS_None)
    , mMecaInitStatus(JMIS_None)
{
    mCurrentPosition = new JanomePosition();

    mAutoReconnectTimer = new QTimer(this);
    connect(mAutoReconnectTimer, &QTimer::timeout, this, &Janome::HandleReconnectTimerChanged);
    mAutoReconnectTimer->setInterval(1000);
}

Janome::~Janome()
{
    Stop();
    if (mSocket != nullptr)
    {
        delete mSocket;
        mSocket = nullptr;
    }

    if (mAutoReconnectTimer != nullptr)
    {
        disconnect(mAutoReconnectTimer, &QTimer::timeout, this, &Janome::HandleReconnectTimerChanged);
        mAutoReconnectTimer->stop();
    }
}

void Janome::SetConnectionAddress(string ipAddress, int port)
{
    mIPAddress = ipAddress;
    mPort = port;
}

void Janome::GetConnectionAddress(string &ipAddress, int &port)
{
    ipAddress = mIPAddress;
    port = mPort;
}

bool Janome::GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    JanomePosition* position = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (position != nullptr)
    {
        position->GetPosition(x, y, z, thetaInDegs);
        return true;
    }
    return false;
}

bool Janome::MovePosition(double x, double y, double z, double thetaInDegs)
{
    if (IsBusy())
        return false;
    string msg = mEncoder.GetMoveCmdMsg(x, y, z, thetaInDegs);
    emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
    return true;
}

bool Janome::MovePosition(unique_ptr<IRobotPosition> position)
{
    JanomePosition* pos = dynamic_cast<JanomePosition*>(position.get());
    if (pos != nullptr)
    {
        return MovePosition(pos->GetPosX(), pos->GetPosY(), pos->GetPosZ(), pos->GetPosThetaInDegs());
    }
    return false;
}

bool Janome::CmdMecaInitialize()
{
    if (IsBusy())
        return false;
    mMecaInitStatus = JMIS_None;
    string msg = mEncoder.GetRunControlMechanicalInitMsg();
    emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
    return true;
}

bool Janome::CmdReturnToHome()
{
    if (IsBusy())
        return false;
    mReturnHomeStatus = JRHS_None;
    string msg = mEncoder.GetRunControlReturnWorkHomeMsg();
    emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
    return true;
}

bool Janome::CmdJogStart(int index)
{
    if (IsBusy())
        return false;
    int movingAxis = -1;
    int movingDirection = -1;
    switch (index) {
    case 0: movingAxis = 0; movingDirection = 0; break;
    case 1: movingAxis = 0; movingDirection = 1; break;
    case 2: movingAxis = 1; movingDirection = 0; break;
    case 3: movingAxis = 1; movingDirection = 1; break;
    case 4: movingAxis = 2; movingDirection = 0; break;
    case 5: movingAxis = 2; movingDirection = 1; break;
    case 6: movingAxis = 3; movingDirection = 0; break;
    case 7: movingAxis = 3; movingDirection = 1; break;
    default: break;
    }
    if (movingAxis != -1 && movingDirection != -1)
    {
        string msg = mEncoder.GetJogStartMsg(movingAxis, movingDirection, mSpeedLevel);
        emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
        return true;
    }
    return false;
}

void Janome::CmdJogStop()
{
    if (mJoggingThread != nullptr && mJoggingThread->isRunning())
    {
        mJoggingThread->requestInterruption();
        mJoggingThread->quit();
        mJoggingThread->wait();
    }
    string msg = mEncoder.GetJogStopMsg();
    emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
}

bool Janome::CmdSetSpeedLevel(int speedLevel)
{
    if (speedLevel >= 0 && mSpeedLevel <= 2)
    {
        mSpeedLevel = speedLevel;
        return true;
    }
    return false;
}

int Janome::GetSpeedLevel() const
{
    return mSpeedLevel;
}

int Janome::GetNumberAxes()
{
    return mRobotInformation.GetNumberAxisMechanism();
}

void Janome::SetPosition(double x, double y, double z, double thetaInDegs)
{
    JanomePosition *pos = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (pos != nullptr)
    {
        pos->SetPosition(x, y, z, thetaInDegs);
    }
    emit OnPositionChanged();
}

void Janome::SetRobotInformation(const JanomeRobotInformation &robotInformation)
{
    mRobotInformation = robotInformation;
    mDeviceName = mRobotInformation.GetSeries();
    mDeviceVersion = mRobotInformation.GetSoftwareVersion();

    emit OnRobotInformUpdated();
}

void Janome::SetRobotJogStarting(bool isStarted)
{
    if (isStarted)
    {
        //send command every 100ms
        mJoggingThread = QThread::create(std::bind(&Janome::JogMovingFnc, this));
        connect(mJoggingThread, &QThread::finished, this, [ = ]() {
            delete mJoggingThread;
            mJoggingThread = nullptr;
        });
        mJoggingThread->start();
    }
    else
    {
        //Jog Failed
    }
}

void Janome::SetRobotJogMoving(bool isMoving)
{
    if (!isMoving)
    {
        //Error
        if (mJoggingThread != nullptr && mJoggingThread->isRunning())
        {
            mJoggingThread->requestInterruption();
            mJoggingThread->quit();
            mJoggingThread->wait();
        }
    }
}

void Janome::JogMovingFnc()
{
    if (mJoggingThread != nullptr)
    {
        while (!mJoggingThread->isInterruptionRequested())
        {
            string msg = mEncoder.GetJogMovingMsg(mSpeedLevel);
            emit OnSendMsgChanged(QByteArray(msg.c_str(), msg.length()));
            QThread::msleep(100);
        }
    }
}

void Janome::SetRobotReturnHomeStatus(JanomeReturnHomeStatus status)
{
    mReturnHomeStatus = status;
    emit OnRobotReturnToHomeStatus(int(mReturnHomeStatus));
}

void Janome::SetRobotMecaInitializingStatus(JanomeMecaInitializeStatus status)
{
    mMecaInitStatus = status;
    emit OnRobotMecaInitStatus(int(mMecaInitStatus));
}

bool Janome::IsBusy()
{
    return false;
    if (mReturnHomeStatus == JRHS_Moving)
        return true;
    if (mMecaInitStatus == JMIS_Initializing)
        return true;
    return false;
}

bool Janome::UpdateCurrentPosition()
{
    string acquireToolTipPosition = mEncoder.GetToolTipPositionMsg();
    emit OnSendMsgChanged(QByteArray(acquireToolTipPosition.c_str(), acquireToolTipPosition.length()));
    return true;
}

void Janome::HandleOnSocketConnectionChanged(bool isConnected)
{
    if (isConnected)
    {
        if (mAutoReconnectTimer->isActive())
            mAutoReconnectTimer->stop();
        mSocketDescriptor = mSocket->SocketDescriptor();
        SetConnectionStatus(RobotConnect_Connected);
        Initialize();
        StartBackgroundThread();
    }
    else
    {
        if (!mIsStopping && mAutoReconnectTimer != nullptr)
            mAutoReconnectTimer->start();
        SetConnectionStatus(RobotConnect_DisConnected);
    }
    emit OnConnectionStatusChanged((int)GetConnectionStatus());
}

void Janome::HandleSocketErrorChanged(const QString& errorMsg)
{
//    if (mAutoReconnectTimer != nullptr)
//        mAutoReconnectTimer->start();
}

void Janome::HandleReceivedMsgChanged(const QByteArray& message)
{
    emit OnReceivedMsg(message);
    //Decode message here
    mDecoder.Execute(message.constData(), message.length());
}

void Janome::HandleReconnectTimerChanged()
{
    if (mAutoReconnectTimer != nullptr)
        mAutoReconnectTimer->stop();
    TcpClient* client = dynamic_cast<TcpClient*>(mSocket);
    if (client)
        client->ConnectTo(mIPAddress, mPort);
}

bool Janome::Initialize()
{
    //Get Robot Information here
    string acquireRobotInformMsg = mEncoder.GetRobotInformationMsg();
    emit OnSendMsgChanged(QByteArray(acquireRobotInformMsg.c_str(), acquireRobotInformMsg.length()));
    return true;
}

bool Janome::OnStart()
{
    if (GetConnectionStatus() != RobotConnect_Connected)
    {
        if (mSocket != nullptr)
        {
            delete  mSocket;
            mSocket = nullptr;
        }
        mIsStopping = false;
        mSocketDescriptor = 0;
        mSocket = new TcpClient();
        connect(mSocket, &TcpSocketBase::OnConnectionChanged, this, &Janome::HandleOnSocketConnectionChanged);
        connect(mSocket, &TcpSocketBase::OnErrorChanged, this, &Janome::HandleSocketErrorChanged);
        connect(mSocket, &TcpSocketBase::OnReceivedMsgChanged, this, &Janome::HandleReceivedMsgChanged);
        connect(this, &Janome::OnSendMsgChanged, mSocket, &TcpSocketBase::HandleSendMessage);
        mSocket->ConnectTo(mIPAddress, mPort);
        return true;
    }
    return false;
}

void Janome::OnStop()
{
    mIsStopping = true;
    if (mAutoReconnectTimer != nullptr)
        mAutoReconnectTimer->stop();
    if (mSocket != nullptr)
        mSocket->Disconnect();
}

void Janome::OnDoWork()
{
    UpdateCurrentPosition();
}

}
