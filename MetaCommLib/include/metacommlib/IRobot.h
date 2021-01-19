#ifndef IROBOT_H
#define IROBOT_H

#include <QObject>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobotPosition.h>

namespace mtcl {

typedef enum e_RobotConnectionStatus{
    RobotConnect_Undefined,
    RobotConnect_Connected,
    RobotConnect_DisConnected
}RobotConnectionStatus;

class METACOMMLIB_EXPORT IRobot : public QObject
{
    Q_OBJECT
public:
    IRobot();
    virtual ~IRobot();
    RobotConnectionStatus GetConnectionStatus();
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual bool GetPosition(double &x, double &y, double &thetaInDegs);
    virtual bool MovePosition(double x, double y, double thetaInDegs) = 0;
signals:
    void OnPositionChanged();
protected:
    virtual bool UpdateCurrentPosition() = 0;
    virtual bool GetCurrentPosition(double &x, double &y, double &thetaInDegs) = 0;
    IRobotPosition* mCurrentPosition;
    RobotConnectionStatus mConnectionStatus;
};

}

#endif // IROBOT_H
