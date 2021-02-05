#include <metacommlib/janome/JanomeControlForm.h>
#include "ui_JanomeControlForm.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QHostAddress>
#include <sstream>
#include <iomanip>

JanomeControlForm::JanomeControlForm(QWidget *parent)
    : mtcl::IRobotUserControl(parent)
    , ui(new Ui::JanomeControlForm)
    , mptrJanome(nullptr)
    , mNumberAxes(2)
{
    ui->setupUi(this);

    connect(ui->connectBtn, SIGNAL(clicked(bool)), this, SLOT(HandleConnectJanome()));
    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(HandlePTPsendBtnClicked()));
    connect(ui->ctrlBtnMechanicalInitialize, SIGNAL(clicked(bool)), this, SLOT(HandleMechanicalInitializeClicked()));
    connect(ui->ctrlBtnHome, SIGNAL(clicked(bool)), this, SLOT(HandleReturnHomeClicked()));

    InitControl();
    ui->ctrlLabelStatus->setText("...");

    //ui->ctrlLineEditIPAddress->setPlaceholderText("192.168.200.180");
    //ui->ctrlLineEditPort->setPlaceholderText("10031");
    ui->ctrlLineEditIPAddress->setText("192.168.200.180");
    ui->ctrlLineEditPort->setText("10031");
}

JanomeControlForm::~JanomeControlForm()
{
    delete ui;
}

bool JanomeControlForm::SetRobot(shared_ptr<mtcl::IRobot> robot)
{
    bool ret = false;
    shared_ptr<mtcl::Janome> derived = dynamic_pointer_cast<mtcl::Janome>(robot);
    if (derived != nullptr)
    {
        mptrJanome = derived;
        connect(mptrJanome.get(), &mtcl::Janome::OnPositionChanged, this, &JanomeControlForm::HandlePositionChanged);
        connect(mptrJanome.get(), &mtcl::Janome::OnConnectionStatusChanged, this, &JanomeControlForm::HandleConnectionStatusChanged);
        connect(mptrJanome.get(), &mtcl::Janome::OnReceivedMsg, this, &JanomeControlForm::HandleReceivedMsg);
        connect(mptrJanome.get(), &mtcl::Janome::OnRobotInformUpdated, this, &JanomeControlForm::HandleRobotInformUpdated);
        connect(mptrJanome.get(), &mtcl::Janome::OnRobotReturnToHomeStatus, this, &JanomeControlForm::HandleRobotReturnToHomeStatusChanged);
        connect(mptrJanome.get(), &mtcl::Janome::OnRobotMecaInitStatus, this, &JanomeControlForm::HandleRobotMecaInitStatusChanged);
        ret = true;
    }
    return ret;
}

void JanomeControlForm::HandleConnectJanome()
{
    if (mptrJanome == nullptr)
    {
        QMessageBox::information(this, tr("error"), tr("No robot attached!!!"), QMessageBox::Ok);
        return;
    }

    if (mptrJanome->GetConnectionStatus() != mtcl::RobotConnect_Connected)
    {
        bool isValidAddress = false;
        QString ipAddress = ui->ctrlLineEditIPAddress->text();
        QHostAddress address(ipAddress);
        if (QAbstractSocket::IPv4Protocol == address.protocol())
        {
            bool isOK = false;
            int port = ui->ctrlLineEditPort->text().toInt(&isOK);
            if (isOK && mptrJanome != nullptr)
            {
                isValidAddress = true;
                mptrJanome->SetConnectionAddress(ipAddress.toStdString(), port);
                mptrJanome->Start();
            }
        }
        if (!isValidAddress)
        {
            QMessageBox::warning(this, "Connection Error", "Invalid Host Address");
        }
    }
    else
    {
        mptrJanome->Stop();
    }
}

void JanomeControlForm::InitControl()
{
    QSignalMapper *signalMapper  = new QSignalMapper(this);

    connect(ui->ctrlBtnXAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnXSub, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnYAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnYSub, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnZAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnZSub, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnRAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnRSub, SIGNAL(pressed()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->ctrlBtnXAdd, 0);
    signalMapper->setMapping(ui->ctrlBtnXSub, 1);
    signalMapper->setMapping(ui->ctrlBtnYAdd, 2);
    signalMapper->setMapping(ui->ctrlBtnYSub, 3);
    signalMapper->setMapping(ui->ctrlBtnZAdd, 4);
    signalMapper->setMapping(ui->ctrlBtnZSub, 5);
    signalMapper->setMapping(ui->ctrlBtnRAdd, 6);
    signalMapper->setMapping(ui->ctrlBtnRSub, 7);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(HandleJOGCtrlBtnPressed(int)));

    connect(ui->ctrlBtnXAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnXSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnYAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnYSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnZAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnZSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnRAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnRSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
}

void JanomeControlForm::RefreshBtn()
{
    if (connectStatus) {
        ui->connectBtn->setText(tr("Disconnect"));
    } else {
        ui->connectBtn->setText(tr("Connect"));
        ui->deviceSNLabel->setText("");
        ui->DeviceNameLabel->setText("");
        ui->DeviceInfoLabel->setText("");
    }

    bool isZAxisValid = false, isRAxisValid = false;
    switch (mNumberAxes) {
    case 2: isZAxisValid = isRAxisValid = false; break;
    case 3: isZAxisValid = true; isRAxisValid = false; break;
    case 4: isZAxisValid = isRAxisValid = true; break;
    }

    ui->sendBtn->setEnabled(connectStatus);
    ui->ctrlDsbPtpX->setEnabled(connectStatus);
    ui->ctrlDsbPtpY->setEnabled(connectStatus);
    ui->ctrlDsbPtpZ->setEnabled(connectStatus && isZAxisValid);
    ui->ctrlDsbPtpR->setEnabled(connectStatus && isRAxisValid);

    ui->ctrlBtnHome->setEnabled(connectStatus);

    ui->ctrlBtnXAdd->setEnabled(connectStatus);
    ui->ctrlBtnXSub->setEnabled(connectStatus);

    ui->ctrlBtnYAdd->setEnabled(connectStatus);
    ui->ctrlBtnYSub->setEnabled(connectStatus);

    ui->ctrlBtnZAdd->setEnabled(connectStatus && isZAxisValid);
    ui->ctrlBtnZSub->setEnabled(connectStatus && isZAxisValid);

    ui->ctrlBtnRAdd->setEnabled(connectStatus && isRAxisValid);
    ui->ctrlBtnRSub->setEnabled(connectStatus && isRAxisValid);

    ui->ctrlBtnMechanicalInitialize->setEnabled(connectStatus);
    ui->ctrlBtnSpeedLow->setEnabled(connectStatus);
    ui->ctrlBtnSpeedHigh->setEnabled(connectStatus);
    ui->ctrlBtnSpeedMedium->setEnabled(connectStatus);

    if (!connectStatus)
    {
        ui->ctrlLineEditIPAddress->setEnabled(true);
        ui->ctrlLineEditPort->setEnabled(true);
    }
    else
    {
        ui->ctrlLineEditIPAddress->setDisabled(true);
        ui->ctrlLineEditPort->setDisabled(true);
    }
}

void JanomeControlForm::HandleJOGCtrlBtnPressed(int index)
{
    if (mptrJanome != nullptr)
    {
        mptrJanome->CmdJogStart(index);
    }
}

void JanomeControlForm::HandleJOGCtrlBtnReleased()
{
    if (mptrJanome != nullptr)
    {
        mptrJanome->CmdJogStop();
    }
}

void JanomeControlForm::HandlePositionChanged()
{
    if (mptrJanome == nullptr)
        return;
    double posX = 0.0, posY = 0.0, posZ = 0.0, posThetaInDegs;
    mptrJanome->GetCurrentPosition(posX, posY, posZ, posThetaInDegs);
    ui->xLabel->setText(QString::number(posX));
    ui->yLabel->setText(QString::number(posY));
    ui->zLabel->setText(QString::number(posZ));
    ui->rLabel->setText(QString::number(posThetaInDegs));
}

void JanomeControlForm::HandleConnectionStatusChanged(int connectionStatus)
{
    (void)connectionStatus;
    mtcl::Janome* bot = dynamic_cast<mtcl::Janome*>(sender());
    if (bot != nullptr)
    {
        connectStatus = (bot->GetConnectionStatus() == mtcl::RobotConnect_Connected) ? true : false;

        ui->ctrlLabelStatus->setText(connectionStatus ? "Connected to Robot" : "Disconnected to Robot");
        string ipAddress("");
        int port = 0;
        bot->GetConnectionAddress(ipAddress, port);
        //Update value of ipaddress and port of connection;
        ui->ctrlLineEditIPAddress->setText(QString::fromStdString(ipAddress));
        ui->ctrlLineEditPort->setText(QString::number(port));
        RefreshBtn();
    }
}

void JanomeControlForm::HandleReceivedMsg(const QByteArray &data)
{
    (void)data;
}

void JanomeControlForm::HandleRobotInformUpdated()
{
    mtcl::Janome* bot = dynamic_cast<mtcl::Janome*>(sender());
    if (bot != nullptr)
    {
        QString botSerialNumber = QString::fromStdString(bot->GetRobotSerialNumber());
        QString botName = QString::fromStdString(bot->GetRobotName());
        QString botVersion = QString::fromStdString(bot->GetRobotVersion());
        ui->deviceSNLabel->setText(botSerialNumber.size() == 0 ? "No Information" : botSerialNumber);
        ui->DeviceNameLabel->setText(botName.size() == 0 ? "No Information" : botName);
        ui->DeviceInfoLabel->setText(botVersion.size() == 0? "No Information" : botVersion);

        mNumberAxes = bot->GetNumberAxes();
        QString axesSupport("");
        switch (mNumberAxes) {
        case 2: axesSupport = "XY"; break;
        case 3: axesSupport = "XYZ"; break;
        case 4: axesSupport = "XYZR"; break;
        }
        ui->AxisInfoLabel->setText(axesSupport);
        RefreshBtn();
    }
}

void JanomeControlForm::HandleMechanicalInitializeClicked()
{
    if (mptrJanome)
    {
        mptrJanome->CmdMecaInitialize();
    }
}

void JanomeControlForm::HandleReturnHomeClicked()
{
    if (mptrJanome)
    {
        mptrJanome->CmdReturnToHome();
    }
}

void JanomeControlForm::HandleRobotReturnToHomeStatusChanged(int v)
{
    this->setEnabled(true);
    switch (v) {
    case mtcl::JRHS_None:
        break;
    case mtcl::JRHS_Failed:
        ui->ctrlLabelStatus->setText("Return Home Failed");
        break;
    case mtcl::JRHS_Moving:
        ui->ctrlLabelStatus->setText("Returning to Home");
        this->setDisabled(true);
        break;
    case mtcl::JRHS_Finished:
        ui->ctrlLabelStatus->setText("Return Home Success");
        break;
    }
}

void JanomeControlForm::HandleRobotMecaInitStatusChanged(int v)
{
    this->setEnabled(true);
    switch (v) {
    case mtcl::JMIS_None:
        break;
    case mtcl::JMIS_Failed:
        ui->ctrlLabelStatus->setText("Initialize Mechanical Failed");
        break;
    case mtcl::JMIS_Initializing:
        ui->ctrlLabelStatus->setText("Initializing Mechanical");
        this->setDisabled(true);
        break;
    case mtcl::JMIS_Finished:
        ui->ctrlLabelStatus->setText("Initialized Mechanical Success");
        break;
    }
}

void JanomeControlForm::HandlePTPsendBtnClicked()
{
    float x = ui->ctrlDsbPtpX->value();
    float y = ui->ctrlDsbPtpY->value();
    float z = ui->ctrlDsbPtpZ->value();
    float thetaInDegs = ui->ctrlDsbPtpR->value();

    if (mptrJanome != nullptr)
    {
        ui->sendBtn->setEnabled(false);
        mptrJanome->MovePosition(x, y, z, thetaInDegs);
        ui->sendBtn->setEnabled(true);
    }
}
