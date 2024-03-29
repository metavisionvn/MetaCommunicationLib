#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AttachDobotWidget(QWidget* userControl);
    void AttachJanomeWidget(QWidget* userControl);
    void AttachTechmanWidget(QWidget* userControl);
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
