#ifndef DOBOTCONTROLLER_H
#define DOBOTCONTROLLER_H

#include <QWidget>
//#include "dobot.h"

namespace Ui {
class DobotController;
}

class DobotControlForm : public QWidget
{
    Q_OBJECT
public:
    explicit DobotControlForm(QWidget *parent = nullptr);
    ~DobotControlForm();
protected:
    void closeEvent(QCloseEvent *);

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
private:
    void refreshBtn();
    void initDobot();
    void initControl();
//    actuator::Dobot mDobot;

    Ui::DobotController *ui;
    bool connectStatus;
    bool mIsSucking;
};

#endif // DOBOTCONTROLLER_H
