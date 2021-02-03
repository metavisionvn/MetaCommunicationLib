#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::AttachDobotWidget(QWidget *userControl)
{
    ui->dobotLayout->addWidget(userControl);
}

void MainWindow::AttachJanomeWidget(QWidget *userControl)
{
    ui->janomeLayout->addWidget(userControl);
}

