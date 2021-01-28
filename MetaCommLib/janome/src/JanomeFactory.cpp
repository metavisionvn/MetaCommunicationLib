#include <metacommlib/janome/JanomeFactory.h>
#include <metacommlib/janome/Janome.h>
#include <metacommlib/janome/JanomeControlForm.h>

namespace mtcl {

JanomeFactory::JanomeFactory()
{

}

JanomeFactory::~JanomeFactory()
{

}

shared_ptr<IRobot> JanomeFactory::CreateRobot()
{
    return shared_ptr<Janome>(new Janome());
}

shared_ptr<IRobotUserControl> JanomeFactory::CreateRobotUserControl(QWidget *parent)
{
    return shared_ptr<JanomeControlForm>(new JanomeControlForm(parent));
}

}
