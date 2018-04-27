#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ScoutCar.h>
#include "qdraw.h"
#include "maindata.h"
#include "planner2d.h"
#include "robotthread.h"


#include <thread>

namespace Ui {
class MainWindow;
}

enum QPlanEvent {noplan,SetRing};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void mousePress(QMouseEvent *);
    void modeChange(int);
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    MainData _data;
    std::shared_ptr<std::vector<bgeo::DRing>> _pvRing;

    std::vector<double> vx;
    std::vector<double> vy;

    size_t planEvent;

     size_t staItemNum = 0;
     size_t staPloNum = 0;
     size_t staGraphNum = 0;


};

#endif // MAINWINDOW_H
