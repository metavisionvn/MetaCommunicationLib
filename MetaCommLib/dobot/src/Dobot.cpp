#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/dobot/DobotPosition.h>

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

void Dobot::OnStart()
{
    if (GetConnectionStatus() != RobotConnect_Connected)
    {
        RobotConnectionStatus status = RobotConnect_Undefined;
        if (ConnectDobot(0, 115200, 0, 0) != DobotConnect_NoError) {
            status = RobotConnect_DisConnected;
        }
        else
            status = RobotConnect_Connected;
        SetConnectionStatus(status);
    }
}

void Dobot::OnStop()
{
    DisconnectDobot();
    SetConnectionStatus(RobotConnect_DisConnected);
}

bool Dobot::MovePosition(double x, double y, double thetaInDegs)
{
    //Not change z
    double mCurPosX = 0.0, mCurPosY = 0.0, mCurPosT = 0.0, mCurPosZ = 0.0;
    GetCurrentPosition(mCurPosX, mCurPosY, mCurPosT, mCurPosZ);
    return MovePosition(x, y, thetaInDegs, mCurPosZ);
}

bool Dobot::GetCurrentPosition(double &x, double &y, double &thetaInDegs, double &z)
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

bool Dobot::GetCurrentJointAngle(double &j1, double &j2, double &j3, double &j4)
{
    DobotPosition* position = dynamic_cast<DobotPosition*>(mCurrentPosition);
    if (position != nullptr)
    {
        position->GetJointAngle(j1, j2, j3, j4);
        return true;
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
    ConvertDobotPoseToRobotPose(pose);
    OnPositionChanged();
    return true;
}

bool Dobot::GetCurrentPosition(double &x, double &y, double &thetaInDegs)
{
    double z = 0.0;
    return GetCurrentPosition(x, y, thetaInDegs, z);
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

void Dobot::ConvertDobotPoseToRobotPose(const Pose &dobotPose)
{
    //Update current position
    DobotPosition *pos = dynamic_cast<DobotPosition*>(mCurrentPosition);
    pos->SetPosition(dobotPose.x, dobotPose.y, dobotPose.r, dobotPose.z);
    double jointAngle[4];
    for (int index = 0; index < 4; index++)
        jointAngle[index] = dobotPose.jointAngle[index];
    pos->SetJointAngle(jointAngle);
}

void Dobot::OnDoWork()
{
    UpdateCurrentPosition();
}

}
