#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/janome/Janome.h>

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

bool mtcl::JanomeDecodeMsg::Execute(const char *payload, const int size)
{
    if (size == 0)
        return false;
    int length = GetDataLength(payload);
    if (length != size)
        return false;
    return DecodeCmd(payload + 6);
}

bool JanomeDecodeMsg::DecodeCmd(const char *cmdMsg)
{
    bool ret = false;
    if (cmdMsg[0] == 'e')
    {
        ret = false;
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
            double x = 0.0, y = 0.0, z = 0.0;
            try {
                uint64_t xPos = HexToDecimal(xPosStr);
                uint64_t yPos = HexToDecimal(yPosStr);
                uint64_t zPos = HexToDecimal(zPosStr);
                x = xPos * UnitIncrements;
                y = yPos * UnitIncrements;
                z = zPos * UnitIncrements;
                mptrRobot->SetPosition(x, y, z);
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



}
