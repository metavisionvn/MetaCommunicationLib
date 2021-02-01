#ifndef DOBOTCONTROLLER_H
#define DOBOTCONTROLLER_H

#include <QWidget>
#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/IRobotUserControl.h>
#include <metacommlib/MetaCommLib_global.h>
#include <iostream>
using namespace std;

namespace Ui {
class DobotController;
}

class METACOMMLIB_EXPORT DobotControlForm : public mtcl::IRobotUserControl
{
    Q_OBJECT
public:
    explicit DobotControlForm(QWidget *parent = nullptr);
    virtual ~DobotControlForm();
    bool SetRobot(shared_ptr<mtcl::IRobot> robot) override;
protected:

private slots:
    void HandleChangedMode();
    void HandleConnectDobot();
    void onJOGCtrlBtnPressed(int index);
    void onJOGCtrlBtnReleased();
    void HandlePTPsendBtnClicked();
    void HandlePositionChanged();

    void onClose();
    void HandleSuckOn();
    void HandleSuckOff();
    void onHome();
    void HandleConnectionStatusChanged(int connectionStatus);
protected:
    void RefreshBtn() override;
    void InitControl() override;
private:
    shared_ptr<mtcl::Dobot> mptrDobot;

    Ui::DobotController *ui;
    bool mIsSucking;
};

#endif // DOBOTCONTROLLER_H
