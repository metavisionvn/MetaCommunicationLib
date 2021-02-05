#ifndef JANOMEROBOTINFORMATION_H
#define JANOMEROBOTINFORMATION_H

#include <metacommlib/MetaCommLib_global.h>
#include <string>
#include <ostream>
using namespace std;

namespace mtcl {

typedef enum e_JanomeMechanicalType{
    JMT_DesktopRobotJR3000Series,
    JMT_CartersianRobotJC3Series
}JanomeMechanicalType;

typedef enum e_JanomeReturnHomeStatus{
    JRHS_None,
    JRHS_Failed,
    JRHS_Moving,
    JRHS_Finished
}JanomeReturnHomeStatus;

typedef enum e_JanomeMecaInitializeStatus
{
    JMIS_None,
    JMIS_Failed,
    JMIS_Initializing,
    JMIS_Finished
}JanomeMecaInitializeStatus;

class METACOMMLIB_EXPORT JanomeRobotInformation
{
public:
    JanomeRobotInformation();
    virtual ~JanomeRobotInformation();
    void SetMechanicalType(JanomeMechanicalType type);
    JanomeMechanicalType GetMechanicalType();

    void SetSeries(string series);
    string GetSeries();

    /**
     * @brief SetNumberAxisMechanism
     * @param no = 2 : support XY
     * no = 3: support XYZ
     * no = 4: support XYZR
     */
    void SetNumberAxisMechanism(int no);
    int GetNumberAxisMechanism();

    void SetXAxisStroke(double v);
    double GetXAxisStroke();

    void SetYAxisStroke(double v);
    double GetYAxisStroke();

    void SetZAxisStroke(double v);
    double GetZAxisStroke();

    void SetRAxisStroke(double v);
    double GetRAxisStroke();

    void SetSoftwareVersion(string softwareVersion);
    string GetSoftwareVersion();

    JanomeRobotInformation& operator= (const JanomeRobotInformation &robotInform);
    friend METACOMMLIB_EXPORT std::ostream &operator<<(std::ostream &os, const JanomeRobotInformation &robotInform);
private:
    JanomeMechanicalType mMechanicalType;
    string mRobotSeries;
    int mNumberAxisMechanism;
    double mXAxisStroke;
    double mYAxisStroke;
    double mZAxisStroke;
    double mRAxisStroke;
    string mSoftwareVersion;
};

}

#endif // JANOMEROBOTINFORMATION_H
