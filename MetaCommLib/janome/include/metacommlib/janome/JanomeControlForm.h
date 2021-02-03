#ifndef JANOMECONTROLFORM_H
#define JANOMECONTROLFORM_H

#include <QWidget>
#include <metacommlib/janome/Janome.h>
#include <metacommlib/IRobotUserControl.h>
#include <metacommlib/MetaCommLib_global.h>
#include <iostream>
using namespace std;

namespace Ui {
class JanomeControlForm;
}

class JanomeControlForm : public mtcl::IRobotUserControl
{
    Q_OBJECT

public:
    explicit JanomeControlForm(QWidget *parent = nullptr);
    ~JanomeControlForm();
    virtual bool SetRobot(shared_ptr<mtcl::IRobot> robot) override;
protected:

private slots:
    void HandleConnectJanome();
    void HandlePTPsendBtnClicked();
    void HandleJOGCtrlBtnPressed(int);
    void HandleJOGCtrlBtnReleased();
    void HandlePositionChanged();
    void HandleConnectionStatusChanged(int connectionStatus);
    void HandleReceivedMsg(const QByteArray& data);
    void HandleRobotInformUpdated();
    void HandleMechanicalInitializeClicked();
    void HandleReturnHomeClicked();
protected:
    virtual void InitControl() override;
    virtual void RefreshBtn() override;
private:
    Ui::JanomeControlForm *ui;
    bool mConnectStatus;
    shared_ptr<mtcl::Janome> mptrJanome;
};

#endif // JANOMECONTROLFORM_H
