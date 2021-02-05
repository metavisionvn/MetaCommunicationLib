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

class METACOMMLIB_EXPORT Janome : public IRobot
{
    Q_OBJECT
public:
    Janome();
    virtual ~Janome();
    void SetConnectionAddress(string ipAddress, int port);
    void GetConnectionAddress(string& ipAddress, int& port);
    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    bool MovePosition(double x, double y, double z, double thetaInDegs);
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
    void CmdJogStop();
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

    friend class JanomeDecodeMsg;
signals:
    //Get response from dobot handler
    void OnReceivedMsg(const QByteArray& message);
    //Internal Used. Send message to dobot handler (cross-thread with socket)
    void OnSendMsgChanged(const QByteArray& msg);
    void OnRobotInformUpdated();
    void OnRobotReturnToHomeStatus(int v);
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
    //update position form decoder
    void SetPosition(double x, double y, double z, double thetaInDegs);
    void SetRobotInformation(const JanomeRobotInformation& robotInformation);
    void SetRobotJogStarting(bool isStart);
    void SetRobotJogMoving(bool isMoving);
    void JogMovingFnc();
    void SetRobotReturnHomeStatus(JanomeReturnHomeStatus status);
    void SetRobotMecaInitializingStatus(JanomeMecaInitializeStatus status);
    bool IsBusy();
    string mIPAddress;
    int mPort;
    JanomeDecodeMsg mDecoder;
    JanomeEncodedMsg mEncoder;
    TcpSocketBase *mSocket;













    QTimer* mAutoReconnectTimer;
    JanomeRobotInformation mRobotInformation;
    bool mIsStopping;
    qintptr mSocketDescriptor;
    int mSpeedLevel;
    QThread* mJoggingThread;
    JanomeReturnHomeStatus mReturnHomeStatus;
    JanomeMecaInitializeStatus mMecaInitStatus;
};

}

#endif // JANOME_H
