#include <metacommlib/IRobot.h>
#include <iostream>
using namespace std;

namespace mtcl {

IRobot::IRobot()
    : mCurrentPosition(nullptr)
    , mConnectionStatus(RobotConnect_Undefined)
    , mMovingStatus(RobotMoving_Done)
    , mBackgroundThread(nullptr)
    , mRefreshTimeIntervalInMilisecs(50.0)
{

}

IRobot::~IRobot()
{
    if (mBackgroundThread != nullptr)
    {
        delete mBackgroundThread;
        mBackgroundThread = nullptr;
    }

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

bool IRobot::Start()
{
    OnStart();

    if (mConnectionStatus == RobotConnect_Connected)
    {
        InterruptBackgroundThread();
        mBackgroundThread = QThread::create(std::bind(&IRobot::DoWork, this));
        connect(mBackgroundThread, &QThread::finished, this, [ = ]() {
            delete mBackgroundThread;
            mBackgroundThread = nullptr;
        });
        mBackgroundThread->start();
    }
    return (mConnectionStatus == RobotConnect_Connected);
}

bool IRobot::Stop()
{
    OnStop();

    if (mConnectionStatus != RobotConnect_Connected)
    {
        InterruptBackgroundThread();
    }
    return (mConnectionStatus != RobotConnect_Connected);
}

void IRobot::SetRefreshTimeInterval(double intervalInMilisecs)
{
    mRefreshTimeIntervalInMilisecs = intervalInMilisecs;
}

void IRobot::SetConnectionStatus(RobotConnectionStatus status)
{
    mConnectionStatus = status;
    emit OnConnectionStatusChanged((int)status);
}

void IRobot::SetMovingStatus(RobotMovingStatus status)
{
    mMovingStatus = status;
    OnMovingStatusChanged((int)status);
}

void IRobot::InterruptBackgroundThread()
{
    if (mBackgroundThread) {
        if (mBackgroundThread->isRunning()) {
            mBackgroundThread->requestInterruption();
            mBackgroundThread->quit();
            mBackgroundThread->wait();
        }
    }
}

void IRobot::DoWork()
{
    while (mBackgroundThread != nullptr && !mBackgroundThread->isInterruptionRequested())
    {
        OnDoWork();
        QThread::msleep(mRefreshTimeIntervalInMilisecs);
    }
}

}
