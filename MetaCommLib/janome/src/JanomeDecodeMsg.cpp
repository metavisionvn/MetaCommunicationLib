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
    size_t length = GetDataLength(msg);
    if (length <= mBuffer.size())
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
            ret = AcquireRobotInformation(cmdMsg);
        }
    }
    else if (commandCode == 'n')
    {
        if (subCommandCode == '1')
        {
            ret = AcquireRobotPosition(cmdMsg);
        }
    }
    else if (commandCode == 'r')
    {
        if (subCommandCode == '0')
        {
            string valueStr(cmdMsg, 4);
        }
    }
    else if (commandCode == 'm')
    {
        if (subCommandCode == '4')
        {
            string valueStr(cmdMsg, 4);
            long value = HexToDecimal2sComplement(valueStr);
            if (value == 0)
                mptrRobot->SetRobotJogStarting(true);
            else
                mptrRobot->SetRobotJogStarting(false);
        }
        else if (subCommandCode == '5')
        {
            string valueStr(cmdMsg, 4);
            long value = HexToDecimal2sComplement(valueStr);
            if (value == 0)
                mptrRobot->SetRobotJogMoving(true);
            else
                mptrRobot->SetRobotJogMoving(false);
        }
    }

    return ret;
}

int JanomeDecodeMsg::GetDataLength(const char *dataLengthStr)
{
    int ret = (int(dataLengthStr[0]))*pow(16, 6) + (int(dataLengthStr[1]))*pow(16, 4) +
            (int(dataLengthStr[2]))*pow(16, 2) + (int(dataLengthStr[3]))*pow(16, 0);
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

long JanomeDecodeMsg::HexToDecimal2sComplement(string num)
{
    char* p;
    uint64_t n = strtoul(num.c_str(), &p, 16);
    if (*p != 0)
    {
        throw std::runtime_error("Not a number");
    }
    return n;
}

bool JanomeDecodeMsg::AcquireRobotInformation(const char *cmdMsg)
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

    {
        //get software version
        string majorNumberStr(cmdMsg + 60, 2);
        string minorNumberStr(cmdMsg + 62, 2);
        string subMinorNumberStr(cmdMsg + 64, 2);
        int majorNumber = HexToDecimal(majorNumberStr);
        int minorNumber = HexToDecimal(minorNumberStr);
        int subMinorNumber = HexToDecimal(subMinorNumberStr);
        char buffer[100];
        snprintf(buffer, 100, "%d.%d.%d", majorNumber, minorNumber, subMinorNumber);
        robotInformation.SetSoftwareVersion(buffer);
    }

    if (mptrRobot)
        mptrRobot->SetRobotInformation(robotInformation);
    return true;
}

bool JanomeDecodeMsg::AcquireRobotPosition(const char *cmdMsg)
{
    bool ret = false;
    //Get Position
    int offset = 0;
    string coordinateSystemStr(cmdMsg + offset, 8);
    offset += 8;
    string xPosStr(cmdMsg + offset, 8);
    offset += 8;
    string yPosStr(cmdMsg + offset, 8);
    offset += 8;
    string zPosStr(cmdMsg + offset, 8);
    offset += 8;
    string thetaPosStr(cmdMsg + offset, 8);
    double x = 0.0, y = 0.0, z = 0.0, thetaInDegs = 0.0;
    try {
        long xPos = HexToDecimal(xPosStr);
        long yPos = HexToDecimal(yPosStr);
        long zPos = HexToDecimal(zPosStr);
        long thetaPos = HexToDecimal2sComplement(thetaPosStr);
        x = xPos * UnitIncrements;
        y = yPos * UnitIncrements;
        z = zPos * UnitIncrements;
        thetaInDegs = thetaPos * UnitIncrements;
        mptrRobot->SetPosition(x, y, z, thetaInDegs);
        ret = true;
    } catch (exception) {

    }
    return ret;
}

}
