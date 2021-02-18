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

/**
 * @brief The IRobot class
 * @details Interface of Robot (abstract class). To instantiate, use concreate factory.
 */
class METACOMMLIB_EXPORT IRobot : public QObject
{
    Q_OBJECT
public:
    IRobot();
    virtual ~IRobot();
    /**
     * @brief GetConnectionStatus.
     * @details 0 undefined; 1 connected; 2 disconnected
     * @return
     */
    RobotConnectionStatus GetConnectionStatus();
    /**
     * @brief Start. Initialize, establish connection with robot
     * @return
     */
    bool Start();
    bool Stop();
    /**
     * @brief SetRefreshTimeInterval: time to update robot's status. Default value is 200ms
     * @param intervalInMilisecs
     */
    void SetRefreshTimeInterval(double intervalInMilisecs);
    /**
     * @brief GetCurrentPosition. Position of robot which updated time-by-time
     * @return
     */
    IRobotPosition* GetCurrentPosition();
    /**
     * @brief MovePosition. Move robot to the pre-set position. When robot reaches to that position, one signal is emitted
     * @param position
     * @return
     */
    virtual bool MovePosition(unique_ptr<IRobotPosition> position) = 0;

    /**
     * @brief GetRobotSerialNumber
     * @return
     */
    string GetRobotSerialNumber() const;
    /**
     * @brief GetRobotName
     * @return
     */
    string GetRobotName() const;
    /**
     * @brief GetRobotVersion
     * @return
     */
    string GetRobotVersion() const;
signals:
    /**
     * @brief OnConnectionStatusChanged.
     * @param status: 0 (undefined); 1(connected); 2(disconnected)
     */
    void OnConnectionStatusChanged(int status);
    /**
     * @brief OnMovingStatusChanged
     * @param status: 0 (moving); 1 (move done)
     */
    void OnMovingStatusChanged(int status);
    /**
     * @brief OnPositionChanged
     */
    void OnPositionChanged();
protected:
    /**
     * @brief StartBackgroundThread. Start worker-thread to update position of robot.
     */
    virtual void StartBackgroundThread();
    /**
     * @brief UpdateCurrentPosition. For Internal use
     * @return
     */
    virtual bool UpdateCurrentPosition() = 0;
    virtual bool OnStart() = 0;
    virtual void OnStop() = 0;
    virtual void OnDoWork() = 0;
    void SetConnectionStatus(RobotConnectionStatus status);
    void SetMovingStatus(RobotMovingStatus status);

    IRobotPosition* mCurrentPosition;   /*!< Current position of robot */
    string mDeviceSerialNumber;         /*!< Serial number of robot */
    string mDeviceName;                 /*!< Name of robot */
    string mDeviceVersion;              /*!< Version of robot */
private:
    //Stop worker-thread when robot stop
    void InterruptBackgroundThread();
    //Scheduled job which manipulated by worker-thread
    void DoWork();
    RobotConnectionStatus mConnectionStatus;
    RobotMovingStatus mMovingStatus;
    QThread *mBackgroundThread;
    double mRefreshTimeIntervalInMilisecs;
};

}

#endif // IROBOT_H
