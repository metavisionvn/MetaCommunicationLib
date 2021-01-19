#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/dobot/DobotPosition.h>

#include <metacommlib/dobot/DobotType.h>
#include <metacommlib/dobot/DobotDll.h>

#include <sstream>
#include <stdio.h>
#include <string.h>
using namespace std;

namespace mtcl {

Dobot::Dobot()
    : IRobot()
    , mDeviceSerialNumber("")
    , mDeviceName("")
    , mDeviceVersion("")
    , mIsSucking(false)
{
    mCurrentPosition = new DobotPosition();
}

Dobot::~Dobot()
{

}

bool Dobot::Start()
{
    if (mConnectionStatus != RobotConnect_Connected)
    {
        if (ConnectDobot(0, 115200,0,0) != DobotConnect_NoError) {
            mConnectionStatus = RobotConnect_DisConnected;
        }
        else
            mConnectionStatus = RobotConnect_Connected;
    }
    return (mConnectionStatus == RobotConnect_Connected);
}

bool Dobot::Stop()
{
    DisconnectDobot();
    return true;
}

bool Dobot::MovePosition(double x, double y, double thetaInDegs)
{
    //Not change z
    double mCurPosX = 0.0, mCurPosY = 0.0, mCurPosT = 0.0, mCurPosZ = 0.0;
    GetPosition(mCurPosX, mCurPosY, mCurPosT, mCurPosZ);
    return MovePosition(x, y, thetaInDegs, mCurPosZ);
}

bool Dobot::GetPosition(double &x, double &y, double &thetaInDegs, double &z)
{
    if (UpdateCurrentPosition())
    {
        DobotPosition* position = dynamic_cast<DobotPosition*>(mCurrentPosition);
        if (position != nullptr)
        {
            position->GetPosition(x, y, thetaInDegs, z);
            return true;
        }
    }
    return false;
}

bool Dobot::MovePosition(double x, double y, double thetaInDegs, double z)
{
    PTPCmd ptpCmd;
    ptpCmd.ptpMode = PTPMOVJXYZMode;
    ptpCmd.x = x;
    ptpCmd.y = y;
    ptpCmd.z = z;
    ptpCmd.r = thetaInDegs;

    while (SetPTPCmd(&ptpCmd, true, NULL) != DobotCommunicate_NoError) {
    }
    return true;
}

string Dobot::GetDobotSerialNumber() const
{
    return mDeviceSerialNumber;
}

string Dobot::GetDobotName() const
{
    return mDeviceName;
}

string Dobot::GetDobotVersion() const
{
    return mDeviceVersion;
}

void Dobot::CmdJogStart(int index, bool isJoint)
{
    JOGCmd jogCmd;

    jogCmd.isJoint = isJoint;
    jogCmd.cmd = index + 1;
    while (SetJOGCmd(&jogCmd, false, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::CmdJogStop(bool isJoint)
{
    JOGCmd jogCmd;

    jogCmd.isJoint = isJoint;
    jogCmd.cmd = JogIdle;
    while (SetJOGCmd(&jogCmd, false, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::CmdPTP(float x, float y, float z, float r)
{
    PTPCmd ptpCmd;
    ptpCmd.ptpMode = PTPMOVJXYZMode;
    ptpCmd.x = x;
    ptpCmd.y = y;
    ptpCmd.z = z;
    ptpCmd.r = r;

    while (SetPTPCmd(&ptpCmd, true, NULL) != DobotCommunicate_NoError) {
    }
}

void Dobot::CmdSuckOn()
{
    mIsSucking = true;
    SetEndEffectorSuctionCup(true, true, false, NULL);
}

void Dobot::CmdSuckOff()
{
    mIsSucking = false;
    SetEndEffectorSuctionCup(true, false, false, NULL);
}

void Dobot::CmdMoveToHome()
{
    HOMECmd homeCmd;
    SetHOMECmd(&homeCmd, false, NULL);
}

bool Dobot::UpdateCurrentPosition()
{
    Pose pose;
    while (GetPose(&pose) != DobotCommunicate_NoError) {
    }

}

bool Dobot::GetCurrentPosition(double &x, double &y, double &thetaInDegs)
{
    if (mCurrentPosition != nullptr)
    {
        mCurrentPosition->GetPosition(x, y, thetaInDegs);
        return true;
    }
    return false;
}

bool Dobot::Initialize()
{
    //Command timeout
    SetCmdTimeout(3000);
    //clear old commands and set the queued command running
    SetQueuedCmdClear();
    SetQueuedCmdStartExec();

    char deviceSN[64];
    GetDeviceSN(deviceSN, sizeof(deviceSN));
    mDeviceSerialNumber = string(deviceSN);

    char deviceName[64];
    GetDeviceName(deviceName, sizeof(deviceName));
    mDeviceName = string(deviceName);

    uint8_t majorVersion, minorVersion, revision;
    GetDeviceVersion(&majorVersion, &minorVersion, &revision);
    stringstream version;
    version << (int)majorVersion << "." << (int)minorVersion << "." << (int)revision;
    mDeviceVersion = version.str();

    //set the end effector parameters
    EndEffectorParams endEffectorParams;
    memset(&endEffectorParams, 0, sizeof(endEffectorParams));
    endEffectorParams.xBias = 71.6f;
    SetEndEffectorParams(&endEffectorParams, false, NULL);

    JOGJointParams jogJointParams;
    for (int i = 0; i < 4; i++) {
        jogJointParams.velocity[i] = 100;
        jogJointParams.acceleration[i] = 100;
    }
    SetJOGJointParams(&jogJointParams, false, NULL);

    JOGCoordinateParams jogCoordinateParams;
    for (int i = 0; i < 4; i++) {
        jogCoordinateParams.velocity[i] = 100;
        jogCoordinateParams.acceleration[i] = 100;
    }
    SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);

    JOGCommonParams jogCommonParams;
    jogCommonParams.velocityRatio = 50;
    jogCommonParams.accelerationRatio = 50;
    SetJOGCommonParams(&jogCommonParams, false, NULL);

    PTPJointParams ptpJointParams;
    for (int i = 0; i < 4; i++) {
        ptpJointParams.velocity[i] = 100;
        ptpJointParams.acceleration[i] = 100;
    }
    SetPTPJointParams(&ptpJointParams, false, NULL);

    PTPCoordinateParams ptpCoordinateParams;
    ptpCoordinateParams.xyzVelocity = 100;
    ptpCoordinateParams.xyzAcceleration = 100;
    ptpCoordinateParams.rVelocity = 100;
    ptpCoordinateParams.rAcceleration = 100;
    SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);

    PTPJumpParams ptpJumpParams;
    ptpJumpParams.jumpHeight = 20;
    ptpJumpParams.zLimit = 150;
    SetPTPJumpParams(&ptpJumpParams, false, NULL);
    return true;
}

}
