#ifndef DOBOTCONTROLLER_H
#define DOBOTCONTROLLER_H

#include <QWidget>
#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/IRobotUserControl.h>
#include <metacommlib/MetaCommLib_global.h>

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
    void onChangedMode();
    void onConnectDobot();
    void onJOGCtrlBtnPressed(int index);
    void onJOGCtrlBtnReleased();
    void onPTPsendBtnClicked();
    void onGetPoseTimer();

    void onClose();
    void onSuck();
    void onHome();
    void HandleConnectionStatusChanged(int connectionStatus);
private:
    void refreshBtn();
    void initControl();
    shared_ptr<mtcl::Dobot> mptrDobot;

    Ui::DobotController *ui;
    bool connectStatus;
    bool mIsSucking;
};

#endif // DOBOTCONTROLLER_H
