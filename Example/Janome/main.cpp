#include "MainWindow.h"

#include <QApplication>
#include <metacommlib/janome/JanomeFactory.h>
#include <metacommlib/IRobotFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    shared_ptr<mtcl::IRobotFactory> robotFactory = shared_ptr<mtcl::IRobotFactory>(new mtcl::JanomeFactory());
    shared_ptr<mtcl::IRobot> robot = robotFactory->CreateRobot();
    shared_ptr<mtcl::IRobotUserControl> userControl = robotFactory->CreateRobotUserControl(&w);
    userControl->SetRobot(robot);

    w.AttachRobotController(userControl.get());

    w.show();
    return a.exec();
}
