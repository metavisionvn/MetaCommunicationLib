#ifndef TECHMAN_H
#define TECHMAN_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/TcpSocketBase.h>
#include <QTimer>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT Techman : public IRobot
{
    Q_OBJECT
public:
    Techman();
    virtual ~Techman();
    void SetConnectionAddress(string ipAddress, int port);
    void GetConnectionAddress(string &ipAddress, int &port);
    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    bool MovePosition(double x, double y, double z, double thetaInDegs);
    bool MovePosition(unique_ptr<IRobotPosition> position) override;

    bool CmdReturnToHome();
    bool CmdJogStart(int index);
    bool CmdJogStop();
signals:
    void OnReceivedMsg(const QByteArray &message);
    void OnSendMsgChanged(const QByteArray &message);
    void OnRobotInformUpdated();
private slots:
    void HandleOnSocketConnectionChanged(bool isConnected);
    void HandleSocketErrorChanged(const QString& errorMsg);
    void HandleReceivedMsgChanged(const QByteArray &message);
    void HandleReconnectTimerChanged();
protected:
    bool UpdateCurrentPosition() override;
    bool OnStart() override;
    void OnStop() override;
    void OnDoWork() override;
private:
    string mIPAddress;
    int mPort;
};

}

#endif // TECHMAN_H
