//#include "DobotDll.h"
//#include "DobotType.h"

#include "DobotControlForm.h"
#include "ui_DobotControlForm.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>

DobotControlForm::DobotControlForm(QWidget *parent)
    : QWidget(parent)
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

    //getPose Timer
    QTimer *getPoseTimer = new QTimer(this);
    getPoseTimer->setObjectName("getPoseTimer");
    connect(getPoseTimer, SIGNAL(timeout()), this, SLOT(onGetPoseTimer()));
    getPoseTimer->start(250);

    //not more than 1000
//    QRegExp regExp("500|0|[-]|[1-9][0-9]{0,2}[.][0-9]{1,3}");
//    QValidator *validator = new QRegExpValidator(regExp, this);
//    ui->xPTPEdit->setValidator(validator);
//    ui->yPTPEdit->setValidator(validator);
//    ui->zPTPEdit->setValidator(validator);
//    ui->rPTPEdit->setValidator(validator);

    connect(ui->ctrlSuck, SIGNAL(released()), this, SLOT(onSuck()));
}

DobotControlForm::~DobotControlForm()
{
    QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
    if (getPoseTimer)
        getPoseTimer->stop();
    delete ui;
}

void DobotControlForm::onGetPoseTimer()
{
    if (!connectStatus)
        return;
//    actuator::Position pose;
//    DobotIns().GetCurrentPose(pose);
//    ui->joint1Label->setText(QString::number(pose.jointAngle[0]));
//    ui->joint2Label->setText(QString::number(pose.jointAngle[1]));
//    ui->joint3Label->setText(QString::number(pose.jointAngle[2]));
//    ui->joint4Label->setText(QString::number(pose.jointAngle[3]));

//    ui->xLabel->setText(QString::number(pose.x));
//    ui->yLabel->setText(QString::number(pose.y));
//    ui->zLabel->setText(QString::number(pose.z));
//    ui->rLabel->setText(QString::number(pose.r));
}

void DobotControlForm::onClose()
{
    this->hide();
}

void DobotControlForm::onSuck()
{
//    static bool isSuck = true;
//    bool isSucking = DobotIns().Suck(isSuck);
//    if (!isSucking)
//    {
//        ui->ctrlSuck->setText("Suck");
//    }
//    else
//    {
//        ui->ctrlSuck->setText("UnSuck");
//    }
//    isSuck = !isSuck;
}

void DobotControlForm::onHome()
{

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
//    //connect dobot
//    if (!connectStatus) {
//        if (!DobotIns().Connect()) {
//            QMessageBox::information(this, tr("error"), tr("Connect dobot error!!!"), QMessageBox::Ok);
//            return;
//        }
//        connectStatus = true;
//        refreshBtn();
//        initDobot();
//    } else {
//        QTimer *getPoseTimer = findChild<QTimer *>("getPoseTimer");
//        getPoseTimer->stop();
//        connectStatus = false;
//        refreshBtn();
//        DobotIns().Disconnect();
//    }
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

void DobotControlForm::initDobot()
{
//    DobotIns().Initialize();
//    ui->deviceSNLabel->setText(QString::fromStdString(DobotIns().GetDobotSerialNumber()));
//    ui->DeviceNameLabel->setText(QString::fromStdString(DobotIns().GetDobotName()));
//    ui->DeviceInfoLabel->setText(QString::fromStdString(DobotIns().GetDobotVersion()));
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
//    DobotIns().JogStart(index, ui->teachMode->currentIndex() == 0);
}

void DobotControlForm::onJOGCtrlBtnReleased()
{
//    DobotIns().JogStop(ui->teachMode->currentIndex() == 0);
}

void DobotControlForm::onPTPsendBtnClicked()
{
    ui->sendBtn->setEnabled(false);

    float x = ui->ctrlDsbPtpX->value();
    float y = ui->ctrlDsbPtpY->value();
    float z = ui->ctrlDsbPtpZ->value();
    float r = ui->ctrlDsbPtpR->value();

//    DobotIns().PTP(x, y, z, r);
    ui->sendBtn->setEnabled(true);
}

void DobotControlForm::closeEvent(QCloseEvent *)
{

}
