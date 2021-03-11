#ifndef JANOME_H
#define JANOME_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/TcpSocketBase.h>
#include <metacommlib/janome/JanomeEncodedMsg.h>
#include <metacommlib/janome/JanomeRobotInformation.h>
#include <QTimer>
#include <string>
using namespace std;

namespace mtcl {

/**
 * @brief The Janome class. Concrete robot of Janome
 */
class METACOMMLIB_EXPORT Janome : public IRobot
{
    Q_OBJECT
public:
    Janome();
    virtual ~Janome();
    /**
     * @brief SetConnectionAddress. Address of connection
     * @param ipAddress
     * @param port
     */
    void SetConnectionAddress(string ipAddress, int port);
    /**
     * @brief GetConnectionAddress
     * @param ipAddress
     * @param port
     */
    void GetConnectionAddress(string& ipAddress, int& port);
    /**
     * @brief GetCurrentPosition. With non-support axis, value set to zero
     * @param x
     * @param y
     * @param z
     * @param thetaInDegs
     * @return
     */
    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    /**
     * @brief MovePosition
     * @param x
     * @param y
     * @param z
     * @param thetaInDegs
     * @return
     */
    bool MovePosition(double x, double y, double z, double thetaInDegs);
    bool MovePosition(double x, double y) override;
    bool MovePosition(double x, double y, double thetaInDegrees) override;
    /**
     * @brief MovePosition
     * @param position
     * @return
     */
    bool MovePosition(unique_ptr<IRobotPosition> position) override;
    /**
     * @brief CmdMecaInitialize (Run Control)
     * @details Initialize mechanical. Very first time when turn on the robot
     *
     */
    bool CmdMecaInitialize();
    /**
     * @brief CmdGoToHome (Run Control)
     */
    bool CmdReturnToHome();
    /**
     * @brief CmdJogStart
     * @param
     * index = 0: X positive;
     * index = 1; X negative;
     * index = 2: Y positive;
     * index = 3: Y negative;
     * index = 4: Z positive;
     * index = 5: Z negative;
     * index = 6: R positive;
     * index = 7: R negative;
     */
    bool CmdJogStart(int index);
    /**
     * @brief CmdJogStop
     */
    void CmdJogStop();
    /**
     * @brief CmdSetSpeedLevel
     * @param speedLevel 0-low 1-medium 2-high
     * @return
     */
    bool CmdSetSpeedLevel(int speedLevel);
    /**
     * @brief GetSpeedLevel
     * @return speed level. 0(Slow) 1(Medium) 2(High)
     */
    int GetSpeedLevel() const;
    /**
     * @brief GetNumberAxes
     * @return 2(XY) 3(XYZ) 4(XYZR)
     */
    int GetNumberAxes();
    //Decoder (message handler)
    friend class JanomeDecodeMsg;
signals:
    //Get response from dobot handler
    void OnReceivedMsg(const QByteArray& message);
    //Internal Used. Send message to dobot handler (cross-thread with socket)
    void OnSendMsgChanged(const QByteArray& msg);
    //Robot Information updated
    void OnRobotInformUpdated();
    /**
     * @brief OnRobotReturnToHomeStatus. Status of robot on moving home position. Command takes time to complete
     * @param v 0-undefined 1-failed 2-moving 3-finished
     */
    void OnRobotReturnToHomeStatus(int v);
    //Status of robot on mechanical initialize.
    /**
     * @brief OnRobotMecaInitStatus
     * @param v 0-none 1-failed 2-initializing 3-finished
     */
    void OnRobotMecaInitStatus(int v);
protected:
    virtual bool OnStart() override;
    virtual void OnStop() override;
    virtual void OnDoWork() override;
    virtual bool UpdateCurrentPosition() override;
private slots:
    void HandleOnSocketConnectionChanged(bool isConnected);
    void HandleSocketErrorChanged(const QString& errorMsg);
    void HandleReceivedMsgChanged(const QByteArray &message);
    void HandleReconnectTimerChanged();
private:
    bool Initialize();
    //[Internal use only]Update position form decoder
    void SetPosition(double x, double y, double z, double thetaInDegs);
    //[Internal use only]Update information form decoder
    void SetRobotInformation(const JanomeRobotInformation& robotInformation);
    //[Internal use only]Robot start to jog
    void SetRobotJogStarting(bool isStart);
    //[Internal use only]Robot start/stop jogging
    void SetRobotJogMoving(bool isMoving);
    //[Internal use only]Keep sending jogging message every 100ms
    void JogMovingFnc();
    //[Internal use only]Status of robot on moving home, updated by decoder
    void SetRobotReturnHomeStatus(JanomeReturnHomeStatus status);
    //[Internal use only]Status of robot on mechanical initializing, updated by decoder
    void SetRobotMecaInitializingStatus(JanomeMecaInitializeStatus status);
    //[Internal use only]Check status of robot. Ex, when homing, no command is accepted
    bool IsBusy();
    string mIPAddress;  /*!< IP Address of robot */
    int mPort;          /*!< Port of robot */
    JanomeDecodeMsg mDecoder;   /*!< Decoder-Protocol Handler */
    JanomeEncodedMsg mEncoder;  /*!< Encoder-Protocol Handler */
    TcpSocketBase *mSocket;     /*!< Socket */
    QTimer* mAutoReconnectTimer;    /*!< Timer to re-connect automatically */
    JanomeRobotInformation mRobotInformation; /*!< Information of Janome Robot */
    bool mIsStopping;
    qintptr mSocketDescriptor;
    int mSpeedLevel;
    QThread* mJoggingThread;
    JanomeReturnHomeStatus mReturnHomeStatus;
    JanomeMecaInitializeStatus mMecaInitStatus;
};

}

#endif // JANOME_H
