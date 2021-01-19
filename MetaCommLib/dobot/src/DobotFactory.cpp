#include <metacommlib/dobot/DobotFactory.h>
#include <metacommlib/dobot/Dobot.h>
#include <metacommlib/dobot/DobotControlForm.h>

namespace mtcl {

DobotFactory::DobotFactory()
{

}

DobotFactory::~DobotFactory()
{

}

shared_ptr<IRobot> DobotFactory::CreateRobot()
{
    return shared_ptr<Dobot>(new Dobot());
}

shared_ptr<IRobotUserControl> DobotFactory::CreateRobotUserControl(QWidget* parent)
{
    return shared_ptr<DobotControlForm>(new DobotControlForm(parent));
}

}
