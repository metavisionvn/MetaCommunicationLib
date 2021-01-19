#include <metacommlib/IRobot.h>

namespace mtcl {

IRobot::IRobot()
    : mCurrentPosition(nullptr)
    , mConnectionStatus(RobotConnect_Undefined)
{

}

IRobot::~IRobot()
{
    if (mCurrentPosition != nullptr)
    {
        delete mCurrentPosition;
        mCurrentPosition = nullptr;
    }
}

RobotConnectionStatus IRobot::GetConnectionStatus()
{
    return mConnectionStatus;
}

bool IRobot::GetPosition(double &x, double &y, double &thetaInDegs)
{
    if (UpdateCurrentPosition())
        return GetCurrentPosition(x, y, thetaInDegs);
    return false;
}

}
