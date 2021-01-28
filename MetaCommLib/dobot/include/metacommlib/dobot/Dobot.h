#ifndef DOBOT_H
#define DOBOT_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/dobot/DobotType.h>
#include <metacommlib/dobot/DobotDll.h>
#include <metacommlib/IRobot.h>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT Dobot : public IRobot
{
public:
    Dobot();
    virtual ~Dobot();

    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    bool GetCurrentJointAngle(double &j1, double &j2, double &j3, double &j4);
    bool MovePosition(double x, double y, double z, double thetaInDegs);
    bool MovePosition(unique_ptr<IRobotPosition> position) override;

    void CmdJogStart(int index, bool isJoint);
    void CmdJogStop(bool isJoint);
    void CmdPTP(float x, float y, float z, float r);
    void CmdSuckOn();
    void CmdSuckOff();
    void CmdMoveToHome();
protected:
    void OnStart() override;
    void OnStop() override;
    bool UpdateCurrentPosition() override;

private:
    bool Initialize();
    void ConvertDobotPoseToRobotPose(const Pose &dobotPose);
    bool mIsSucking;

protected:
    void OnDoWork() override;
};

}

#endif // DOBOT_H
