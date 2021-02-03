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

    string posXStr = EncodeDataWith2Byte(posX, 4);
    string posYStr = EncodeDataWith2Byte(posY, 4);
    string posZStr = EncodeDataWith2Byte(posZ, 4);
    string posRStr = EncodeDataWith2Byte(posR, 4);
    string mt1Str = EncodeDataWith2Byte(mt1, 4);
    string mt2Str = EncodeDataWith2Byte(mt2, 4);

    string posXHexStr = StringToHexString(posXStr);
    string posYHexStr = StringToHexString(posYStr);
    string posZHexStr = StringToHexString(posZStr);
    string posRHexStr = StringToHexString(posRStr);
    string mt1HexStr = StringToHexString(mt1Str);
    string mt2HexStr = StringToHexString(mt2Str);

    //data length is fixed;
    int dataLength = 56 + 4 + 2 + 2;
    string dataLengthStr = EncodeDataWith2Byte(dataLength, 4);
    //Encode coordinate system
    string coordinateSystemStr = EncodeDataWith2Byte(1, 4);
    string coordinateSystemHexStr = StringToHexString(coordinateSystemStr);

    string msg = EncodeDataWith2Byte(dataLength, 4) + mFixedID  + StringToHexString("M1") + coordinateSystemHexStr + posXHexStr + posYHexStr + posZHexStr + posRHexStr + mt1HexStr + mt2HexStr;
    string ret = HexStringToString(msg);

    return ret;
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
    return "";
}

int JanomeEncodedMsg::GetCmdLength(const string &cmd)
{
    return (4 + 2 + cmd.size());
}

string JanomeEncodedMsg::EncodeDataWith2Byte(long value, int numBytes)
{
    //Using 2 character to encode. Add one place for null '\0'
    int len = 2*numBytes + 1;
    char* str = new char[len];
    string formattedStr = "%0" + std::to_string(2*numBytes) + "X";
    snprintf(str, len, formattedStr.c_str(), value);
    string output(str);
    delete [] str;
    return output;
}

string JanomeEncodedMsg::EncodedCmd(const string &cmd)
{
    string hexCmd = StringToHexString(cmd);
    int length = GetCmdLength(cmd);
    //Encode
    string msg = EncodeDataWith2Byte(length, 4) + mFixedID + hexCmd;
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
