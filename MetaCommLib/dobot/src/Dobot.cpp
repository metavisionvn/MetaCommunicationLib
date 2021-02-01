#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/dobot/DobotPosition.h>

#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

namespace mtcl {

Dobot::Dobot()
    : IRobot()
    , mIsSucking(false)
{
    mCurrentPosition = new DobotPosition();
}

Dobot::~Dobot()
{
    if (GetConnectionStatus() == RobotConnect_Connected)
    {
        Stop();
    }
}

void Dobot::OnStart()
{
    if (GetConnectionStatus() != RobotConnect_Connected)
    {
        RobotConnectionStatus status = RobotConnect_Undefined;
        if (ConnectDobot(0, 115200, 0, 0) != DobotConnect_NoError) {
            status = RobotConnect_DisConnected;
        }
        else{
            status = RobotConnect_Connected;
            Initialize();
        }
        SetConnectionStatus(status);
    }
}

void Dobot::OnStop()
{
    DisconnectDobot();
    SetConnectionStatus(RobotConnect_DisConnected);
}

bool Dobot::GetCurrentPosition(double &x, double &y, double &z, double &thetaInDegs)
{
    DobotPosition* position = dynamic_cast<DobotPosition*>(mCurrentPosition);
    if (position != nullptr)
    {
        position->GetPosition(x, y, z, thetaInDegs);
        return true;
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

bool Dobot::MovePosition(double x, double y, double z, double thetaInDegs)
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

bool Dobot::MovePosition(unique_ptr<IRobotPosition> position)
{
    DobotPosition* pos = dynamic_cast<DobotPosition*>(position.get());
    if (pos != nullptr)
    {
        return MovePosition(pos->GetPosX(), pos->GetPosY(), pos->GetPosZ(), pos->GetPosThetaInDegs());
    }
    return false;
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
    emit OnPositionChanged();
    return true;
}

bool Dobot::Initialize()
{
    cout << "[Dobot::Initialize]" << endl;
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
    if (pos != nullptr)
    {
        pos->SetPosition(dobotPose.x, dobotPose.y, dobotPose.z, dobotPose.r);
        double jointAngle[4];
        for (int index = 0; index < 4; index++)
            jointAngle[index] = dobotPose.jointAngle[index];
        pos->SetJointAngle(jointAngle);
    }
}

void Dobot::OnDoWork()
{
    UpdateCurrentPosition();
}

}
