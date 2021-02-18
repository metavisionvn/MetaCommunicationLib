#ifndef IROBOTFACTORY_H
#define IROBOTFACTORY_H

#include <memory>
#include <QtWidgets>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/IRobotUserControl.h>
using namespace std;

namespace mtcl {

/**
 * @brief The IRobotFactory class
 * @details Abstract factory to generate concrete robot and user control.
 * Concrete robot-factory need to be implemented when novel robot integrated.
 */
class METACOMMLIB_EXPORT IRobotFactory
{
public:
    IRobotFactory();
    virtual ~IRobotFactory();
    /**
     * @brief CreateRobot. Create concrete robot
     * @return
     */
    virtual shared_ptr<IRobot> CreateRobot() = 0;
    /**
     * @brief CreateRobotUserControl. Create concrete robot user control (UI)
     * @param parent
     * @return
     */
    virtual shared_ptr<IRobotUserControl> CreateRobotUserControl(QWidget* parent = nullptr) = 0;
};

}

#endif // IROBOTFACTORY_H
