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

string JanomeEncodedMsg::GetMechanicalInitMsg()
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

string JanomeEncodedMsg::GetMoveCmdMsg(double x, double y, double z)
{
    long posX = x / UnitIncrements;
    long posY = y / UnitIncrements;
    long posZ = z / UnitIncrements;
    long posR = 0;
    long mt1 = 0;
    long mt2 = 0;

    string posXStr = EncodedDataLength(posX);
    string posYStr = EncodedDataLength(posY);
    string posZStr = EncodedDataLength(posZ);
    string posRStr = EncodedDataLength(posR);
    string mt1Str = EncodedDataLength(mt1);
    string mt2Str = EncodedDataLength(mt2);


    string posXHexStr = StringToHexString(posXStr);
    string posYHexStr = StringToHexString(posYStr);
    string posZHexStr = StringToHexString(posZStr);
    string posRHexStr = StringToHexString(posRStr);
    string mt1HexStr = StringToHexString(mt1Str);
    string mt2HexStr = StringToHexString(mt2Str);

    //data length is fixed;
    int dataLength = 56 + 4 + 2 + 2;

    //Encode coordinate system
    string coordinateSystemStr = EncodedDataLength(1);
    string coordinateSystemHexStr = StringToHexString(coordinateSystemStr);

    string msg = EncodedDataLength(dataLength) + mFixedID  + StringToHexString("M1") + coordinateSystemHexStr + posXHexStr + posYHexStr + posZHexStr + posRHexStr + mt1HexStr + mt2HexStr;
    return HexStringToString(msg);
}

int JanomeEncodedMsg::GetCmdLength(const string &cmd)
{
    return (4 + 2 + cmd.size());
}

string JanomeEncodedMsg::EncodedDataLength(int dataLength)
{
    //Using 2 character to encode. Add one place for null '\0'
    int len = 2*4 + 1;
    char* str = new char[len];
    snprintf(str, len, "%08X", dataLength);
    string output(str);
    delete [] str;
    return output;
}

string JanomeEncodedMsg::EncodedCmd(const string &cmd)
{
    string hexCmd = StringToHexString(cmd);
    int length = GetCmdLength(cmd);
    //Encode
    string msg = EncodedDataLength(length) + mFixedID + hexCmd;
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
