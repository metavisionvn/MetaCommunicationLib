#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/janome/Janome.h>
#include <metacommlib/janome/JanomeRobotInformation.h>

namespace mtcl {

JanomeDecodeMsg::JanomeDecodeMsg(Janome *robot)
    : UnitIncrements(0.0005)
    , mptrRobot(robot)
{

}

JanomeDecodeMsg::~JanomeDecodeMsg()
{
    mptrRobot = nullptr;
}

bool JanomeDecodeMsg::Execute(const char *payload, const int size)
{
    if (size == 0)
        return false;
    mBuffer.insert(mBuffer.end(), payload, payload + size);
    if (mBuffer.size() < 4)
        return false;
    char* msg = &mBuffer[0];
    int length = GetDataLength(msg);
    if (length <= size)
    {
        //Decode message, ignore lenght and fixed identifier

        char commandCode = *(msg + 6);
        char subCommandCode = *(msg + 7);
        bool isSuccess = DecodeCmd(commandCode, subCommandCode, msg + 8);
        mBuffer.erase(mBuffer.begin(), mBuffer.begin() + length);
        return isSuccess;
    }
    return false;
}

bool JanomeDecodeMsg::DecodeCmd(char commandCode, char subCommandCode, const char *cmdMsg)
{
    bool ret = false;
    if (commandCode == 'e')
    {
        //Error
        ret = false;
    }
    else if (commandCode == 'b')
    {
        if (subCommandCode == '0')
        {
            //Acquire robot information
            AcquireRobotInformation(cmdMsg);
        }
    }
    else if (cmdMsg[0] == 'n')
    {
        if (cmdMsg[1] == '1')
        {
            //Get Position
            int offset = 2;
            string coordinateSystemStr(cmdMsg + offset, 8);
            offset += 8;
            string xPosStr(cmdMsg + offset, 8);
            offset += 8;
            string yPosStr(cmdMsg + offset, 8);
            offset += 8;
            string zPosStr(cmdMsg + offset, 8);
            double x = 0.0, y = 0.0, z = 0.0, thetaInDegs = 0.0;
            try {
                uint64_t xPos = HexToDecimal(xPosStr);
                uint64_t yPos = HexToDecimal(yPosStr);
                uint64_t zPos = HexToDecimal(zPosStr);
                x = xPos * UnitIncrements;
                y = yPos * UnitIncrements;
                z = zPos * UnitIncrements;

                mptrRobot->SetPosition(x, y, z, thetaInDegs);
                ret = true;
            } catch (exception) {
                ret = false;
            }
        }
    }
    return ret;
}

int JanomeDecodeMsg::GetDataLength(const char *dataLengthStr)
{
    int ret = (int(dataLengthStr[0]))*pow(16, 3) + (int(dataLengthStr[1]))*pow(16, 2) +
            (int(dataLengthStr[2]))*pow(16, 1) + (int(dataLengthStr[3]))*pow(16, 0);
    return ret;
}

long JanomeDecodeMsg::HexToDecimal(string num)
{
    char* p;
    long n = strtol(num.c_str(), &p, 16);
    if (*p != 0)
    {
        throw std::runtime_error("Not a number");
    }

    return n;
}

void JanomeDecodeMsg::AcquireRobotInformation(const char *cmdMsg)
{
    //1. 4+5 mechanical type
    int index = 0;
    JanomeRobotInformation robotInformation;
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '0')
    {
        robotInformation.SetMechanicalType(JMT_DesktopRobotJR3000Series);
    }
    else if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '2')
    {
        robotInformation.SetMechanicalType(JMT_CartersianRobotJC3Series);
    }
    index += 2;

    //size classification
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '2')
    {
        robotInformation.SetSeries("JR3200");
    }
    else if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '3')
    {
        robotInformation.SetSeries("JR3300");
    }
    else if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '4')
    {
        robotInformation.SetSeries("JR3400");
    }
    else if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '5')
    {
        robotInformation.SetSeries("JR3500");
    }
    else if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '6')
    {
        robotInformation.SetSeries("JR3600");
    }
    index += 2;

    //Motor classification
    index += 2;

    //Axis support
    int numberAxisMechanism = 0;
    //X axis
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '0')
    {
        numberAxisMechanism++;
    }
    index += 2;
    //Y axis
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '1')
    {
        numberAxisMechanism++;
    }
    index += 2;

    //Z axis
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '2')
    {
        numberAxisMechanism++;
    }
    index += 2;

    //R axis
    if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '3')
    {
        numberAxisMechanism++;
    }
    index += 2;
    robotInformation.SetNumberAxisMechanism(numberAxisMechanism);

    //axis function 5th
    index += 2;
    //axis function 6th
    index += 2;
    //not use
    index += 4;

    //specialized axis configuration. Single side or double side
    index += 2;

    //custom stroke flag
    index += 2;

    //X-Axis stroke
    {
        string stroke;
        stroke += cmdMsg[index];
        stroke += cmdMsg[index + 1];

        double strokeValue = HexToDecimal(stroke) * 10.0;
        robotInformation.SetXAxisStroke(strokeValue);
        index += 2;
    }

    //Y-Axis stroke
    {
        string stroke;
        stroke += cmdMsg[index];
        stroke += cmdMsg[index + 1];
        double strokeValue = HexToDecimal(stroke) * 10.0;
        robotInformation.SetYAxisStroke(strokeValue);
        index += 2;
    }
    //Z-Axis stroke
    {
        string stroke;
        stroke += cmdMsg[index];
        stroke += cmdMsg[index + 1];
        double strokeValue = HexToDecimal(stroke) * 10.0;
        robotInformation.SetZAxisStroke(strokeValue);
        index += 2;
    }

    //R-Axis stroke
    {
        if (cmdMsg[index] == '0' && cmdMsg[index + 1] == '2')
            robotInformation.SetRAxisStroke(360.0);
        index += 2;
    }

    if (mptrRobot)
        mptrRobot->SetRobotInformation(robotInformation);
}

}
