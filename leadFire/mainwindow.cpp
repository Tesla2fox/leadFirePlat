#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->QGraph->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChange(int)));
    connect(ui->QGraph, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));


    //ScoutCar xx;
    //xx.setOrient(0);
    //xx.setPos(bgeo::DPoint(0,0));
    //ScoutCar xx(0,0,4,0);

    //    drawScoutCar(xx,ui->QGraph);


    ui->QGraph->xAxis->setRange(0,320);
    ui->QGraph->yAxis->setRange(0,180);
    //ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);
    ui->QGraph->replot();

    //    _data.pushTarget(bgeo::DPoint(0,50));
    //    _data.pushTarget(bgeo::DPoint(100,100));
    //    _data.pushTarget(bgeo::DPoint(100,50));
    //    _data.pushTarget(bgeo::DPoint(50,50));

    //    _data.setAttackedPnt(bgeo::DPoint(75,75));


    //    _data.wSPushback(bgeo::DPoint(0,0));
    //    _data.wSPushback(bgeo::DPoint(320,0));
    //    _data.wSPushback(bgeo::DPoint(320,180));
    //    _data.wSPushback(bgeo::DPoint(0,180));
    //    _data.wSPushback(bgeo::DPoint(0,0));

    //    ScoutCar car1(15,15,4,0);
    //    ScoutCar car2(200,100,5,M_PI);
    //    ScoutCar car3(150,125,7,M_PI_2);

    //    _data.pushScoutCar(car1);
    //    _data.pushScoutCar(car2);
    //    _data.pushScoutCar(car3);

    _pvRing = std::make_shared<std::vector<bgeo::DRing>>();

    _pvRing->clear();
    this->_data.SetVRing(_pvRing);
    this->_data.loadXml("/home/robot/qt_code/leadFirePlat/data/data.xml");
    drawMainData(_data,ui->QGraph);



    this->staItemNum = ui->QGraph->itemCount();
    this->staPloNum = ui->QGraph->plottableCount();
    this->staGraphNum = ui->QGraph->graphCount();

    qDebug()<<"Item = "<<staItemNum<< "  Plot= "<<staPloNum << "  graph "<< staGraphNum;


    //_data.loadMap();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);
    ui->QGraph->replot();
}


void MainWindow::modeChange(int p)
{
    this->planEvent = p;
}


void MainWindow::mousePress(QMouseEvent *event)
{
    ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);

    if (qApp->mouseButtons() == Qt::LeftButton)
    {
        switch (this->planEvent) {
        case QPlanEvent::SetRing:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            vx.push_back(x_val);
            vy.push_back(y_val);
            break;
        }
        default:
            break;
        }
    }

    if (qApp->mouseButtons() == Qt::RightButton)
    {
        switch (this->planEvent)
        {
        case QPlanEvent::SetRing:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            vx.push_back(x_val);
            vy.push_back(y_val);

            vx.push_back(vx.front());
            vy.push_back(vy.front());

            bgeo::DRing ring;
            for(size_t i = 0; i < vx.size(); i++)
            {
                ring.push_back(bgeo::DPoint(vx.at(i),vy.at(i)));
            }
            _pvRing->push_back(ring);
            vx.clear();
            vy.clear();
        }
        }
    }
}


///save the _MainData
void MainWindow::on_pushButton_2_clicked()
{
    this->_data.SetVRing(_pvRing);
    drawMainData(_data,ui->QGraph);
    this->_data.writeXml();
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{

    while (this->staPloNum<ui->QGraph->plottableCount())
    {
        ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
    }
    while (this->staItemNum<ui->QGraph->itemCount())
    {
        ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
    }
    auto car = _data.getScoutCar(1);
    Planner2D planner(_data,car);
    planner.plan();
    auto path = planner.getPath();
    drawVpnt(path,ui->QGraph);
}

//init the
void MainWindow::on_pushButton_4_clicked()
{
    //_data.getScoutCar().at()
    for(size_t j = 0; j<1;j++)
    {
        _data.initScoutCarEncode();
        for(size_t p = 0;p<1;p++)
        {
            ScoutCar car1 = _data.getScoutCar().at(p);
            //car1.generateAllPath();
            for(size_t i = 0; i<_data.getVTarget().size();i++)
            {
                car1.generatePath(i);
                drawScoutCar(car1,ui->QGraph);
            }
            auto vpath = car1.getRSpath();
            QPen pen;
            pen.setColor(Qt::blue);
            //drawvRSPath(vpath,car1.getTurnRad(),ui->QGraph,pen);
            double rad = car1.getTurnRad();
            for(auto &it:vpath)
            {
                drawRSPath(it,rad,ui->QGraph,pen);
            }
        }
        std::cout<<j<<std::endl;
    }
}

//init plan
void MainWindow::on_pushButton_5_clicked()
{
    for(auto &it :ex_ScoutCar)
    {
        it.setVTaget(ex_TargetPnt);
    }

    std::thread t0(scoutCarThread,0);
    //    std::thread t1(scoutCarThread,1);
    //    std::thread t2(scoutCarThread,2);

    t0.join();
    //    t1.join();
    //    t2.join();
    //for(size_t  i = 0; i<ex_ScoutCar.size();i++)
    //    {
    //        std::thread t1(scoutCarThread,i);
    //    }
    std::cout<< "wtf" <<std::endl;
}
