#ifndef IROBOT_H
#define IROBOT_H

#include <QObject>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobotPosition.h>
#include <QThread>
#include <QMutex>

namespace mtcl {

typedef enum e_RobotConnectionStatus{
    RobotConnect_Undefined = 0,
    RobotConnect_Connected = 1,
    RobotConnect_DisConnected = 2
}RobotConnectionStatus;

typedef enum e_RobotMovingStatus{
    RobotMoving_Starting,
    RobotMoving_Done
}RobotMovingStatus;

class METACOMMLIB_EXPORT IRobot : public QObject
{
    Q_OBJECT
public:
    IRobot();
    virtual ~IRobot();
    RobotConnectionStatus GetConnectionStatus();
    bool Start();
    bool Stop();
    void SetRefreshTimeInterval(double intervalInMilisecs);
    virtual bool GetCurrentPosition(double &x, double &y, double &thetaInDegs) = 0;
    virtual bool MovePosition(double x, double y, double thetaInDegs) = 0;
signals:
    void OnConnectionStatusChanged(int status);
    void OnMovingStatusChanged(int status);
    void OnPositionChanged();
protected:
    virtual bool UpdateCurrentPosition() = 0;
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;
    virtual void OnDoWork() = 0;
    void SetConnectionStatus(RobotConnectionStatus status);
    void SetMovingStatus(RobotMovingStatus status);
    IRobotPosition* mCurrentPosition;
private:
    void InterruptBackgroundThread();
    void DoWork();
    RobotConnectionStatus mConnectionStatus;
    RobotMovingStatus mMovingStatus;
    QThread *mBackgroundThread;
    double mRefreshTimeIntervalInMilisecs;
};

}

#endif // IROBOT_H
