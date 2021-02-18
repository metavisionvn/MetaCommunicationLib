#ifndef DOBOT_H
#define DOBOT_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/dobot/DobotType.h>
#include <metacommlib/dobot/DobotDll.h>
#include <metacommlib/IRobot.h>
#include <string>
using namespace std;

namespace mtcl {

/**
 * @brief The Dobot class. Concrete Dobot
 */
class METACOMMLIB_EXPORT Dobot : public IRobot
{
public:
    Dobot();
    virtual ~Dobot();
    /**
     * @brief GetCurrentPosition
     * @param x
     * @param y
     * @param z
     * @param thetaInDegs
     * @return
     */
    bool GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs);
    /**
     * @brief GetCurrentJointAngle
     * @param j1
     * @param j2
     * @param j3
     * @param j4
     * @return
     */
    bool GetCurrentJointAngle(double &j1, double &j2, double &j3, double &j4);
    /**
     * @brief MovePosition
     * @param x
     * @param y
     * @param z
     * @param thetaInDegs
     * @return
     */
    bool MovePosition(double x, double y, double z, double thetaInDegs);
    /**
     * @brief MovePosition
     * @param position
     * @return
     */
    bool MovePosition(unique_ptr<IRobotPosition> position) override;
    /**
     * @brief CmdJogStart
     * @param index
     * @param isJoint
     */
    void CmdJogStart(int index, bool isJoint);
    /**
     * @brief CmdJogStop
     * @param isJoint
     */
    void CmdJogStop(bool isJoint);
    /**
     * @brief CmdPTP. Move to position using PTP
     * @param x
     * @param y
     * @param z
     * @param r
     */
    void CmdPTP(float x, float y, float z, float r);
    /**
     * @brief CmdSuckOn. Turn on sucker
     */
    void CmdSuckOn();
    /**
     * @brief CmdSuckOff. Turn off sucker
     */
    void CmdSuckOff();
    /**
     * @brief CmdMoveToHome. Move to Home position
     */
    void CmdMoveToHome();
protected:
    bool OnStart() override;
    void OnStop() override;
    //Update current position.
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
