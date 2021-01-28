#include <metacommlib/janome/JanomeControlForm.h>
#include "ui_JanomeControlForm.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QHostAddress>

JanomeControlForm::JanomeControlForm(QWidget *parent)
    : mtcl::IRobotUserControl(parent)
    , ui(new Ui::JanomeControlForm)
{
    ui->setupUi(this);

    connect(ui->connectBtn, SIGNAL(clicked(bool)), this, SLOT(HandleConnectJanome()));
    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(HandlePTPsendBtnClicked()));
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
        ret = true;
    }
    return ret;
}

void JanomeControlForm::HandleConnectJanome()
{
    bool isValidAddress = false;
    QString ipAddress = ui->ctrlLineEditIPAddress->text();
    QHostAddress address(ipAddress);
    if (QAbstractSocket::IPv4Protocol == address.protocol())
    {
        bool isOK = false;
        int port = ui->ctrlLineEditPort->text().toInt(&isOK);
        if (isOK)
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

void JanomeControlForm::InitControl()
{
    QSignalMapper *signalMapper  = new QSignalMapper(this);

    connect(ui->ctrlBtnXAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnXSub, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnYAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnYSub, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnZAdd, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(ui->ctrlBtnZSub, SIGNAL(pressed()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->ctrlBtnXAdd, 0);
    signalMapper->setMapping(ui->ctrlBtnXSub, 1);
    signalMapper->setMapping(ui->ctrlBtnYAdd, 2);
    signalMapper->setMapping(ui->ctrlBtnYSub, 3);
    signalMapper->setMapping(ui->ctrlBtnZAdd, 4);
    signalMapper->setMapping(ui->ctrlBtnZSub, 5);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(HandleJOGCtrlBtnPressed(int)));
    connect(ui->ctrlBtnXAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnXSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnYAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnYSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnZAdd, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
    connect(ui->ctrlBtnZSub, SIGNAL(released()), this, SLOT(HandleJOGCtrlBtnReleased()));
}

void JanomeControlForm::HandleJOGCtrlBtnPressed(int index)
{

}

void JanomeControlForm::HandleJOGCtrlBtnReleased()
{

}

void JanomeControlForm::HandlePositionChanged()
{
    if (mptrJanome == nullptr)
        return;
    double posX = 0.0, posY = 0.0, posZ = 0.0;
    mptrJanome->GetCurrentPosition(posX, posY, posZ);
    ui->xLabel->setText(QString::number(posX));
    ui->yLabel->setText(QString::number(posY));
    ui->zLabel->setText(QString::number(posZ));
}

void JanomeControlForm::HandleConnectionStatusChanged(int connectionStatus)
{

}

void JanomeControlForm::HandlePTPsendBtnClicked()
{
    float x = ui->ctrlDsbPtpX->value();
    float y = ui->ctrlDsbPtpY->value();
    float z = ui->ctrlDsbPtpZ->value();

//    if (mptrDobot != nullptr)
//    {
//        ui->sendBtn->setEnabled(false);
//        mptrDobot->CmdPTP(x, y, z, r);
//        ui->sendBtn->setEnabled(true);
//    }
}
