#include <metacommlib/techman/TechmanFactory.h>
#include <metacommlib/techman/Techman.h>
#include <metacommlib/techman/TechmanControlForm.h>

namespace mtcl {

TechmanFactory::TechmanFactory()
{

}

TechmanFactory::~TechmanFactory()
{

}

shared_ptr<IRobot> TechmanFactory::CreateRobot()
{
    return shared_ptr<Techman>(new Techman());
}

shared_ptr<IRobotUserControl> TechmanFactory::CreateRobotUserControl(QWidget *parent)
{
    return shared_ptr<TechmanControlForm>(new TechmanControlForm(parent));
}

}
