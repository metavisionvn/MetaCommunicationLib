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
    , mSocket(nullptr)
{
    mCurrentPosition = new JanomePosition();

    mAutoReconnectTimer = new QTimer(this);
    connect(mAutoReconnectTimer, &QTimer::timeout, this, &Janome::HandleReconnectTimerChanged);
    mAutoReconnectTimer->setSingleShot(true);
    mAutoReconnectTimer->setInterval(1000);
}

Janome::~Janome()
{
    if (mSocket != nullptr)
    {
        delete mSocket;
        mSocket = nullptr;
    }

    if (mAutoReconnectTimer != nullptr)
    {
        disconnect(mAutoReconnectTimer, &QTimer::timeout, this, &Janome::HandleReconnectTimerChanged);
        mAutoReconnectTimer->stop();
        mAutoReconnectTimer->deleteLater();
    }
}

void Janome::SetConnectionAddress(string ipAddress, int port)
{
    mIPAddress = ipAddress;
    mPort = port;
}

bool Janome::GetCurrentPosition(double &x, double &y, double &z)
{
    JanomePosition* position = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (position != nullptr)
    {
        position->GetPosition(x, y, z);
        return true;
    }
    return false;
}

bool Janome::MovePosition(double x, double y, double z)
{
    return true;
}

bool Janome::MovePosition(unique_ptr<IRobotPosition> position)
{
    JanomePosition* pos = dynamic_cast<JanomePosition*>(position.get());
    if (pos != nullptr)
    {
        return MovePosition(pos->GetPosX(), pos->GetPosY(), pos->GetPosZ());
    }
    return false;
}

void Janome::SetPosition(double x, double y, double z)
{
    JanomePosition *pos = dynamic_cast<JanomePosition*>(mCurrentPosition);
    if (pos != nullptr)
    {
        pos->SetPosition(x, y, z);
    }
    emit OnPositionChanged();
}

bool Janome::UpdateCurrentPosition()
{
    //Request to get current position
    return true;
}

void Janome::HandleOnSocketConnectionChanged(bool isConnected)
{
    if (isConnected)
    {
        mAutoReconnectTimer->stop();
        SetConnectionStatus(RobotConnect_Connected);
    }
    else
    {
        mAutoReconnectTimer->start();
        SetConnectionStatus(RobotConnect_DisConnected);
    }
}

void Janome::HandleSocketErrorChanged(const QString& errorMsg)
{
    mAutoReconnectTimer->start();
}

void Janome::HandleReceivedMsgChanged(const QByteArray& message)
{
    OnReceivedMsg(message);
    //Decode message here
    mDecoder.Execute(message.data(), message.size());
}

void Janome::HandleReconnectTimerChanged()
{
    mAutoReconnectTimer->stop();
    TcpClient* client = dynamic_cast<TcpClient*>(mSocket);
    if (client)
        client->ConnectTo(mIPAddress, mPort);
}

bool Janome::Initialize()
{
    return false;
}

void Janome::OnStart()
{
    if (GetConnectionStatus() != RobotConnect_Connected)
    {
        if (mSocket != nullptr)
        {
            delete  mSocket;
            mSocket = nullptr;
        }
        mSocket = new TcpClient();
        connect(mSocket, &TcpSocketBase::OnConnectionChanged, this, &Janome::HandleOnSocketConnectionChanged);
        connect(mSocket, &TcpSocketBase::OnErrorChanged, this, &Janome::HandleSocketErrorChanged);
        connect(mSocket, &TcpSocketBase::OnReceivedMsgChanged, this, &Janome::HandleReceivedMsgChanged);
        connect(this, &Janome::OnSendMsgChanged, mSocket, &TcpSocketBase::HandleSendMessage);
        mSocket->ConnectTo(mIPAddress, mPort);
    }
}

void Janome::OnStop()
{
    mAutoReconnectTimer->stop();
    mSocket->Disconnect();
}

void Janome::OnDoWork()
{
    UpdateCurrentPosition();
}

}
