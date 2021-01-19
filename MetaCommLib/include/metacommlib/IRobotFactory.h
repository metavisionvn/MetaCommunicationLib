#ifndef IROBOTFACTORY_H
#define IROBOTFACTORY_H

#include <memory>
#include <QtWidgets>
#include <metacommlib/MetaCommLib_global.h>
#include <metacommlib/IRobot.h>
#include <metacommlib/IRobotUserControl.h>
using namespace std;

namespace mtcl {

class METACOMMLIB_EXPORT IRobotFactory
{
public:
    IRobotFactory();
    virtual ~IRobotFactory();
    virtual shared_ptr<IRobot> CreateRobot() = 0;
    virtual shared_ptr<IRobotUserControl> CreateRobotUserControl(QWidget* parent = nullptr) = 0;
};

}

#endif // IROBOTFACTORY_H
