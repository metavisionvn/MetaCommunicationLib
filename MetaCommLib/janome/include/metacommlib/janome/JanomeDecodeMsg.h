#ifndef JANOMEDECODEMSG_H
#define JANOMEDECODEMSG_H

#include <metacommlib/MetaCommLib_global.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <QByteArray>
using namespace std;

namespace mtcl {

class Janome;

class METACOMMLIB_EXPORT JanomeDecodeMsg
{
public:
    JanomeDecodeMsg(Janome *robot);
    virtual ~JanomeDecodeMsg();
    bool Execute(const char *payload, const int size);
private:
    bool DecodeCmd(char commandCode, char subCommandCode, const char *cmdMsg);
    int GetDataLength(const char *dataLengthStr);
    long HexToDecimal(string num);

    void AcquireRobotInformation(const char *cmdMsg);
    vector<char> mBuffer;
    const double UnitIncrements;
    Janome *mptrRobot;
};

}

#endif // JANOMEDECODEMSG_H
