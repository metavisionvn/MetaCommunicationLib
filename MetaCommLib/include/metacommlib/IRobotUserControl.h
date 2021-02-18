#ifndef IROBOTUSERCONTROL_H
#define IROBOTUSERCONTROL_H

#include <QWidget>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobot.h>
#include <memory>
using namespace std;

namespace mtcl {

/**
 * @brief The IRobotUserControl class. Abstract class of robot user control
 */
class METACOMMLIB_EXPORT IRobotUserControl : public QWidget
{
    Q_OBJECT
public:
    IRobotUserControl(QWidget* parent = nullptr);
    virtual ~IRobotUserControl();
    /**
     * @brief SetRobot. Set instantiate of robot.
     * @param robot
     * @return
     */
    virtual bool SetRobot(shared_ptr<mtcl::IRobot> robot) = 0;
protected:
    /**
     * @brief InitControl. Initialize control components.
     */
    virtual void InitControl() = 0;
    /**
     * @brief RefreshBtn. Set status of control components (enable/disable even hide) depends on status of robot
     */
    virtual void RefreshBtn() = 0;
    bool mConnectStatus;
};

}

#endif // IROBOTUSERCONTROL_H
