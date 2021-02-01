#ifndef JANOMEENCODEDMSG_H
#define JANOMEENCODEDMSG_H

#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/janome/JanomeDecodeMsg.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/TcpSocketBase.h>
#include <iostream>
#include <string>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT JanomeEncodedMsg
{
public:
    JanomeEncodedMsg();
    virtual ~JanomeEncodedMsg();
    string GetRobotInformationMsg();
    string GetRobotStatusMsg();
    string GetMechanicalInitMsg();
    string GetAxisPositionRequestMsg();
    string GetToolTipPositionMsg();
    string GetMoveCmdMsg(double x, double y, double z);
private:
    int GetCmdLength(const string& cmd);
    string EncodedDataLength(int dataLength);
    string EncodedCmd(const string& cmd);
    string HexStringToString(const string inputString);
    string StringToHexString(string input);

    string mFixedID;
    char mDataLength[4];
    double UnitIncrements;
};

}

#endif // JANOMEENCODEDMSG_H
