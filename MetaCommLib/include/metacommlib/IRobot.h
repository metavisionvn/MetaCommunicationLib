#ifndef IROBOT_H
#define IROBOT_H

#include <QObject>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobotPosition.h>
#include <QThread>
#include <QMutex>
#include <memory>
using namespace std;

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
    IRobotPosition* GetCurrentPosition();
    virtual bool MovePosition(unique_ptr<IRobotPosition> position) = 0;

    string GetRobotSerialNumber() const;
    string GetRobotName() const;
    string GetRobotVersion() const;
signals:
    void OnConnectionStatusChanged(int status);
    void OnMovingStatusChanged(int status);
    void OnPositionChanged();
protected:
    virtual void StartBackgroundThread();
    virtual bool UpdateCurrentPosition() = 0;
    virtual bool OnStart() = 0;
    virtual void OnStop() = 0;
    virtual void OnDoWork() = 0;
    void SetConnectionStatus(RobotConnectionStatus status);
    void SetMovingStatus(RobotMovingStatus status);
    IRobotPosition* mCurrentPosition;
    string mDeviceSerialNumber;
    string mDeviceName;
    string mDeviceVersion;
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
