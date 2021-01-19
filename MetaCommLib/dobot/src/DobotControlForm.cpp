//#include "DobotDll.h"
//#include "DobotType.h"

#include <metacommlib/dobot/DobotControlForm.h>
#include "ui_DobotControlForm.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>

DobotControlForm::DobotControlForm(QWidget *parent)
    : mtcl::IRobotUserControl(parent)
    , ui(new Ui::DobotController)
    , mIsSucking(false)
{
    ui->setupUi(this);

    connectStatus = false;
    //endtype change
    connect(ui->teachMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangedMode()));

    //connect dobot
    connect(ui->connectBtn, SIGNAL(clicked(bool)), this, SLOT(onConnectDobot()));

    //send PTP data
    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(onPTPsendBtnClicked()));

    //init JOG control
    initControl();

    connect(ui->ctrlSuck, SIGNAL(released()), this, SLOT(onSuck()));
}

DobotControlForm::~DobotControlForm()
{
    if (ui != nullptr)
    {
        delete ui;
        ui = nullptr;
    }
}

bool DobotControlForm::SetRobot(shared_ptr<mtcl::IRobot> robot)
{
    bool ret = false;
    shared_ptr<mtcl::Dobot> derived = dynamic_pointer_cast<mtcl::Dobot>(robot);
    if (derived != nullptr)
    {
        mptrDobot = derived;

        connect(mptrDobot.get(), &mtcl::Dobot::OnPositionChanged, this, &DobotControlForm::onGetPoseTimer);
        connect(mptrDobot.get(), &mtcl::Dobot::OnMovingStatusChanged, this, &DobotControlForm::onGetPoseTimer);
        connect(mptrDobot.get(), &mtcl::Dobot::OnConnectionStatusChanged, this, &DobotControlForm::HandleConnectionStatusChanged);

        ret = true;
    }
    return ret;
}

void DobotControlForm::onGetPoseTimer()
{
    if (mptrDobot == nullptr)
        return;
    double posX = 0.0, posY = 0.0, posT = 0.0, posZ = 0.0;
    mptrDobot->GetCurrentPosition(posX, posY, posT, posZ);
    double j1 = 0.0, j2 = 0.0, j3 = 0.0, j4 = 0.0;
    mptrDobot->GetCurrentJointAngle(j1, j2, j3, j4);
    ui->joint1Label->setText(QString::number(j1));
    ui->joint2Label->setText(QString::number(j2));
    ui->joint3Label->setText(QString::number(j3));
    ui->joint4Label->setText(QString::number(j4));

    ui->xLabel->setText(QString::number(posX));
    ui->yLabel->setText(QString::number(posY));
    ui->zLabel->setText(QString::number(posZ));
    ui->rLabel->setText(QString::number(posT));
}

void DobotControlForm::onClose()
{
    this->hide();
}

void DobotControlForm::onSuck()
{
    if (mptrDobot == nullptr)
        return;
    mptrDobot->CmdSuckOn();
}

void DobotControlForm::onHome()
{

}

void DobotControlForm::HandleConnectionStatusChanged(int status)
{
    mtcl::Dobot* bot = dynamic_cast<mtcl::Dobot*>(sender());
    if (bot != nullptr)
    {
        connectStatus = (bot->GetConnectionStatus() == mtcl::RobotConnect_Connected) ? true : false;
        QString botSerialNumber("");
        QString botName("");
        QString botVersion("");

        refreshBtn();
        if (connectStatus)
        {
            botSerialNumber = QString::fromStdString(bot->GetDobotSerialNumber());
            botName = QString::fromStdString(bot->GetDobotName());
            botVersion = QString::fromStdString(bot->GetDobotVersion());
        }
        ui->deviceSNLabel->setText(botSerialNumber);
        ui->DeviceNameLabel->setText(botName);
        ui->DeviceInfoLabel->setText(botVersion);
    }
}

void DobotControlForm::onChangedMode()
{
    if (ui->teachMode->currentIndex() == 1) {
        ui->baseAngleAddBtn->setText(tr("X+"));
        ui->baseAngleSubBtn->setText(tr("X-"));
        ui->longArmAddBtn->setText(tr("Y+"));
        ui->longArmSubBtn->setText(tr("Y-"));
        ui->shortArmAddBtn->setText(tr("Z+"));
        ui->shortArmSubBtn->setText(tr("Z-"));
        ui->rHeadAddBtn->setText(tr("R+"));
        ui->rHeadSubBtn->setText(tr("R-"));
    } else {
        ui->baseAngleAddBtn->setText(tr("J1+"));
        ui->baseAngleSubBtn->setText(tr("J1-"));
        ui->longArmAddBtn->setText(tr("J2+"));
        ui->longArmSubBtn->setText(tr("J2-"));
        ui->shortArmAddBtn->setText(tr("J3+"));
        ui->shortArmSubBtn->setText(tr("J3-"));
        ui->rHeadAddBtn->setText(tr("J4+"));
        ui->rHeadSubBtn->setText(tr("J4-"));
    }
}

void DobotControlForm::onConnectDobot()
{
    if (mptrDobot == nullptr)
    {
        QMessageBox::information(this, tr("error"), tr("No robot attached!!!"), QMessageBox::Ok);
        return;
    }

    if (mptrDobot->GetConnectionStatus() != mtcl::RobotConnect_Connected)
    {
        if (!mptrDobot->Start()) {
            QMessageBox::information(this, tr("error"), tr("Connect dobot failed!!!"), QMessageBox::Ok);
            return;
        }
    }
    else
    {
        mptrDobot->Stop();
    }
}

void DobotControlForm::refreshBtn()
{
    if (connectStatus) {
        ui->connectBtn->setText(tr("Disconnect"));

        ui->teachMode->setEnabled(true);
        ui->baseAngleAddBtn->setEnabled(true);
        ui->baseAngleSubBtn->setEnabled(true);
        ui->longArmAddBtn->setEnabled(true);
        ui->longArmSubBtn->setEnabled(true);
        ui->shortArmAddBtn->setEnabled(true);
        ui->shortArmSubBtn->setEnabled(true);
        ui->rHeadAddBtn->setEnabled(true);
        ui->rHeadSubBtn->setEnabled(true);

        ui->sendBtn->setEnabled(true);
        ui->ctrlDsbPtpX->setEnabled(true);
        ui->ctrlDsbPtpY->setEnabled(true);
        ui->ctrlDsbPtpZ->setEnabled(true);
        ui->ctrlDsbPtpR->setEnabled(true);

        ui->ctrlSuck->setEnabled(true);
    } else {
        ui->connectBtn->setText(tr("Connect"));

        ui->teachMode->setEnabled(false);
        ui->baseAngleAddBtn->setEnabled(false);
        ui->baseAngleSubBtn->setEnabled(false);
        ui->longArmAddBtn->setEnabled(false);
        ui->longArmSubBtn->setEnabled(false);
        ui->shortArmAddBtn->setEnabled(false);
        ui->shortArmSubBtn->setEnabled(false);
        ui->rHeadAddBtn->setEnabled(false);
        ui->rHeadSubBtn->setEnabled(false);

        ui->sendBtn->setEnabled(false);
        ui->ctrlDsbPtpX->setEnabled(false);
        ui->ctrlDsbPtpX->setEnabled(false);
        ui->ctrlDsbPtpX->setEnabled(false);
        ui->ctrlDsbPtpX->setEnabled(false);
        ui->ctrlSuck->setEnabled(false);
    }
}

void DobotControlForm::initControl()
{
    QSignalMapper *signalMapper  = new QSignalMapper(this);

    connect(ui->baseAngleAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->baseAngleSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->longArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->longArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->shortArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->shortArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->rHeadAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->rHeadSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->baseAngleAddBtn, 0);
    signalMapper->setMapping(ui->baseAngleSubBtn, 1);
    signalMapper->setMapping(ui->longArmAddBtn, 2);
    signalMapper->setMapping(ui->longArmSubBtn, 3);
    signalMapper->setMapping(ui->shortArmAddBtn, 4);
    signalMapper->setMapping(ui->shortArmSubBtn, 5);
    signalMapper->setMapping(ui->rHeadAddBtn, 6);
    signalMapper->setMapping(ui->rHeadSubBtn, 7);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onJOGCtrlBtnPressed(int)));

    connect(ui->baseAngleAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->baseAngleSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->longArmAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->longArmSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->shortArmAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->shortArmSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->rHeadAddBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
    connect(ui->rHeadSubBtn, SIGNAL(released()), this, SLOT(onJOGCtrlBtnReleased()));
}

void DobotControlForm::onJOGCtrlBtnPressed(int index)
{
    if (mptrDobot != nullptr)
        mptrDobot->CmdJogStart(index, ui->teachMode->currentIndex() == 0);
}

void DobotControlForm::onJOGCtrlBtnReleased()
{
    if (mptrDobot != nullptr)
        mptrDobot->CmdJogStop(ui->teachMode->currentIndex() == 0);
}

void DobotControlForm::onPTPsendBtnClicked()
{
    if (mptrDobot != nullptr)
    {
        ui->sendBtn->setEnabled(false);

        float x = ui->ctrlDsbPtpX->value();
        float y = ui->ctrlDsbPtpY->value();
        float z = ui->ctrlDsbPtpZ->value();
        float r = ui->ctrlDsbPtpR->value();
        mptrDobot->CmdPTP(x, y, z, r);
        ui->sendBtn->setEnabled(true);
    }
}