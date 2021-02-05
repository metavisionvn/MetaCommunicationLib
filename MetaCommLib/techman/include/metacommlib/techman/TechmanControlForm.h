#ifndef TECHMANCONTROLFORM_H
#define TECHMANCONTROLFORM_H

#include <QWidget>
#include <metacommlib/techman/Techman.h>
#include <metacommlib/IRobotUserControl.h>
#include <metacommlib/MetaCommLib_global.h>
#include <iostream>

namespace Ui {
class TechmanControlForm;
}

class TechmanControlForm : public mtcl::IRobotUserControl
{
    Q_OBJECT

public:
    explicit TechmanControlForm(QWidget *parent = nullptr);
    ~TechmanControlForm();
    virtual bool SetRobot(shared_ptr<mtcl::IRobot> robot) override;
protected:
    virtual void InitControl() override;
    virtual void RefreshBtn() override;
private:
    Ui::TechmanControlForm *ui;
    shared_ptr<mtcl::Techman> mptrRobot;
};

#endif // TECHMANCONTROLFORM_H
