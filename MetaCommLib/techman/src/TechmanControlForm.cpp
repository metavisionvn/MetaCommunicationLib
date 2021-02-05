#include <metacommlib/techman/TechmanControlForm.h>
#include "ui_TechmanControlForm.h"

#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <QTimer>
#include <QHostAddress>
#include <sstream>
#include <iomanip>

TechmanControlForm::TechmanControlForm(QWidget *parent)
    : mtcl::IRobotUserControl(parent)
    , ui(new Ui::TechmanControlForm)
{
    ui->setupUi(this);
}

TechmanControlForm::~TechmanControlForm()
{
    delete ui;
}

bool TechmanControlForm::SetRobot(shared_ptr<mtcl::IRobot> robot)
{
    bool ret = false;
    shared_ptr<mtcl::Techman> derived = dynamic_pointer_cast<mtcl::Techman>(robot);
    if (derived != nullptr)
    {
        mptrRobot = derived;
        ret = true;
    }
    return ret;
}

void TechmanControlForm::InitControl()
{

}

void TechmanControlForm::RefreshBtn()
{

}
