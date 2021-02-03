#include <metacommlib/janome/JanomeEncodedMsg.h>

namespace mtcl {

JanomeEncodedMsg::JanomeEncodedMsg()
    : UnitIncrements(0.0005)
{
    mFixedID = "0303";
}

JanomeEncodedMsg::~JanomeEncodedMsg()
{

}

string JanomeEncodedMsg::GetRobotInformationMsg()
{
    string ret = EncodedCmd("B0");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetRobotStatusMsg()
{
    string ret = EncodedCmd("B1");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetRunControlMechanicalInitMsg()
{
    string ret = EncodedCmd("R0");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetAxisPositionRequestMsg()
{
    string ret = EncodedCmd("N0");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetToolTipPositionMsg()
{
    string ret = EncodedCmd("N1");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetMoveCmdMsg(double x, double y, double z, double thetaInDegs)
{
    long posX = x / UnitIncrements;
    long posY = y / UnitIncrements;
    long posZ = z / UnitIncrements;
    long posR = thetaInDegs/UnitIncrements;
    long mt1 = 0;
    long mt2 = 0;

    string posXStr = EncodeDataWith2Byte(posX, 8);
    string posYStr = EncodeDataWith2Byte(posY, 8);
    string posZStr = EncodeDataWith2Byte(posZ, 8);
    string posRStr = EncodeDataWith2Byte(posR, 8);
    string mt1Str = EncodeDataWith2Byte(mt1, 8);
    string mt2Str = EncodeDataWith2Byte(mt2, 8);
    string posXHexStr = StringToHexString(posXStr);
    string posYHexStr = StringToHexString(posYStr);
    string posZHexStr = StringToHexString(posZStr);
    string posRHexStr = StringToHexString(posRStr);
    string mt1HexStr = StringToHexString(mt1Str);
    string mt2HexStr = StringToHexString(mt2Str);

    //data length is fixed;
    int dataLength = 56 + 4 + 2 + 2;
    string dataLengthStr = EncodeDataLength(dataLength);
    //Encode coordinate system
    string coordinateSystemStr = EncodeDataWith2Byte(1, 8);
    string coordinateSystemHexStr = StringToHexString(coordinateSystemStr);

    string msg = dataLengthStr + mFixedID  + StringToHexString("M1") + coordinateSystemHexStr + posXHexStr + posYHexStr + posZHexStr + posRHexStr + mt1HexStr + mt2HexStr;
    return  HexStringToString(msg);
}

string JanomeEncodedMsg::GetRunControlReturnWorkHomeMsg()
{
    string ret = EncodedCmd("R2");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetRunControlStartMsg()
{
    string ret = EncodedCmd("R3");
    return HexStringToString(ret);
}

string JanomeEncodedMsg::GetJogStartMsg(int movingAxis, int movingDirection, int speedLevel)
{
    //1. command name
    long reversed = 0;
    string reversedStr = EncodeDataWith2Byte(reversed, 2);
    string movingAxisStr = EncodeDataWith2Byte(movingAxis, 2);
    string movingDirectionStr = EncodeDataWith2Byte(movingDirection, 2);
    string speedLevelStr = EncodeDataWith2Byte(speedLevel, 2);
    string toolDataStr = EncodeDataWith2Byte(0, 56);

    string reversedHexStr = StringToHexString(reversedStr);
    string movingAxisHexStr = StringToHexString(movingAxisStr);
    string movingDirectionHexStr = StringToHexString(movingDirectionStr);
    string speedLevelHexStr = StringToHexString(speedLevelStr);
    string toolDataHexStr = StringToHexString(toolDataStr);


    //data length is fixed: data length (4); fixed (2); command (2); reversed (2); moving axis(2); moving direction(2); speed (2); tool data(56)
    int dataLength = 4 + 2 + 2 + 2 + 2 + 2 + 2 + 56;
    string dataLengthStr = EncodeDataLength(dataLength);

    string msg = dataLengthStr + mFixedID  + StringToHexString("M4") + reversedHexStr + movingAxisHexStr + movingDirectionHexStr + speedLevelHexStr + toolDataHexStr;
    return  HexStringToString(msg);
}

string JanomeEncodedMsg::GetJogMovingMsg(int speedLevel)
{
    string speedLevelStr = EncodeDataWith2Byte(speedLevel, 2);
    string speedLevelHexStr = StringToHexString(speedLevelStr);

    //data length is fixed: data length (4); fixed (2); command (2); speed (2);
    int dataLength = 4 + 2 + 2 + 2;
    string dataLengthStr = EncodeDataLength(dataLength);

    string msg = dataLengthStr + mFixedID  + StringToHexString("M5") + speedLevelHexStr;
    return  HexStringToString(msg);
}

string JanomeEncodedMsg::GetJogStopMsg()
{
    string ret = EncodedCmd("M6");
    return HexStringToString(ret);
}

int JanomeEncodedMsg::GetCmdLength(const string &cmd)
{
    return (4 + 2 + cmd.size());
}

string JanomeEncodedMsg::EncodeDataLength(int value)
{
    int len = 8 + 1;
    char* str = new char[len];
    snprintf(str, len, "%08X", value);
    string output(str);
    delete [] str;
    return output;
}

string JanomeEncodedMsg::EncodeDataWith2Byte(long value, int numBytes)
{
    //Using 2 character to encode. Add one place for null '\0'
    int len = numBytes + 1;
    char* str = new char[len];
    string formattedStr = "%0" + std::to_string(numBytes) + "X";
    snprintf(str, len, formattedStr.c_str(), value);
    string output(str);
    delete [] str;
    return output;
}

string JanomeEncodedMsg::EncodedCmd(const string &cmd)
{
    string hexCmd = StringToHexString(cmd);
    int length = GetCmdLength(cmd);
    string dataLengthStr = EncodeDataLength(length);
    string msg = dataLengthStr + mFixedID + hexCmd;
    return msg;
}

string JanomeEncodedMsg::HexStringToString(const string inputString)
{
    string ascii = "";
    for (size_t i = 0; i < inputString.length(); i += 2){
        //taking two characters from hex string
        string part = inputString.substr(i, 2);
        //changing it into base 16
        char ch = stoul(part, nullptr, 16);
        //putting it into the ASCII string
        ascii += ch;
    }
    return ascii;
}

string JanomeEncodedMsg::StringToHexString(string input)
{
    int loop = 0;
    int i = 0;

    char* temp = new char[2*input.size() + 1];
    while(loop < (int)input.size())
    {
        sprintf((char*)(temp + i), "%02X", input[loop]);
        loop += 1;
        i += 2;
    }
    string output(temp);
    delete [] temp;
    return output;
}

}
