#ifndef DOBOT_H
#define DOBOT_H

#include <metacommlib/IRobot.h>
#include <string>
using namespace std;

namespace mtcl {

class Dobot : public IRobot
{
public:
    Dobot();
    virtual ~Dobot();
    bool Start() override;
    bool Stop() override;
    bool MovePosition(double x, double y, double thetaInDegs) override;
    bool GetPosition(double &x, double &y, double &thetaInDegs, double &z);
    bool MovePosition(double x, double y, double thetaInDegs, double z);

    string GetDobotSerialNumber() const;
    string GetDobotName() const;
    string GetDobotVersion() const;

    void CmdJogStart(int index, bool isJoint);
    void CmdJogStop(bool isJoint);
    void CmdPTP(float x, float y, float z, float r);
    void CmdSuckOn();
    void CmdSuckOff();
    void CmdMoveToHome();
protected:
    bool UpdateCurrentPosition() override;
    bool GetCurrentPosition(double &x, double &y, double &thetaInDegs) override;
private:
    bool Initialize();

    string mDeviceSerialNumber;
    string mDeviceName;
    string mDeviceVersion;
    bool mIsSucking;
};

}

#endif // DOBOT_H
