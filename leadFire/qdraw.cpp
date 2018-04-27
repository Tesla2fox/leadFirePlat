#include "qdraw.h"


bool drawScoutCar(const ScoutCar &agent, QCustomPlot *qplot)
{
    const bgeo::EqTri &camRegion = agent.getEqTri();
    auto carTriangle = camRegion.getTriangle();
    drawRing(carTriangle,qplot);

    auto  mRobDir = new QCPItemLine(qplot);

    mRobDir->start->setType(QCPItemPosition::ptPlotCoords);
    mRobDir->end->setType(QCPItemPosition::ptPlotCoords);

    //agent.getPos().x();
    mRobDir->start->setCoords(agent.getPos().x(),agent.getPos().y());
    mRobDir->setHead(QCPLineEnding::esDisc);
    double mdx = cos(agent.getOrient())*2 + agent.getPos().x();
    double mdy = sin(agent.getOrient())*2 + agent.getPos().y();
    mRobDir->end->setCoords(mdx,mdy);


    return false;
}

bool drawRing(const bgeo::DRing &ring, QCustomPlot *qplot)
{
    QCPCurve *RingCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
    ////QVector
    QVector<QCPCurveData> RingCurveData;


    size_t i = 0;
    for(auto &it :ring)
    {
        auto e_x  = it.x();
        auto e_y  = it.y();
        RingCurveData.push_back(QCPCurveData(i++, e_x,e_y));
    }
    RingCurve->data()->set(RingCurveData, false);

    qplot->xAxis->setScaleRatio(qplot->yAxis);
    QPen pen;
    pen.setColor(Qt::GlobalColor::magenta);
    //pen.setColor
    pen.setWidth(1);
    RingCurve->setPen(pen);
    qplot->replot();
    return true;
}

bool drawMainData(const MainData &_data, QCustomPlot *qplot)
{

    auto vTarget = _data.getVTarget();
    for(auto &it :vTarget)
    {
        QCPItemEllipse *circle = new QCPItemEllipse(qplot);
        double rad = 2;
        circle->topLeft->setCoords(it.x() - rad, it.y() + rad);
        circle->bottomRight->setCoords(it.x() + rad, it.y() - rad);
        QPen pen;
        pen.setColor(Qt::GlobalColor::darkMagenta);
        circle->setPen(pen);
    }

    {
        QCPItemEllipse *circle = new QCPItemEllipse(qplot);
        double rad = 2;
        auto it = _data.getAttackedPnt();
        circle->topLeft->setCoords(it.x() - rad, it.y() + rad);
        circle->bottomRight->setCoords(it.x() + rad, it.y() - rad);
        QPen pen;
        pen.setColor(Qt::GlobalColor::darkRed);
        QBrush brush;
        brush.setColor(Qt::GlobalColor::darkRed);
        brush.setStyle(Qt::Dense1Pattern);
        circle->setBrush(brush);
    }

    {
        auto wspace = _data.getWSpace();
        QPen pen;
        pen.setColor(Qt::GlobalColor::darkRed);
        pen.setWidth(4);
        drawRing(wspace,qplot,pen);
    }

    if(false)
    {
        auto vring  =  _data.getVRing();
        for(size_t i = 0; i< vring.size();i++)
        {
            QPen pen;
            pen.setColor(Qt::GlobalColor::blue);
            pen.setWidth(2);
            drawRing(vring.at(i),qplot,pen);
        }
    }
    {
        auto vCar = _data.getScoutCar();
        for(auto &it :vCar)
        {
            drawScoutCar(it,qplot);
        }
    }
    qplot->replot();
    return true;
}



bool drawRing(const bgeo::DRing &ring, QCustomPlot *qplot, const QPen &pen)
{
    QCPCurve *RingCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
    ////QVector
    QVector<QCPCurveData> RingCurveData;

    for (size_t i = 0; i < ring.size(); i++)
    {
        //
        RingCurveData.push_back(QCPCurveData(i, ring.at(i).x(), ring.at(i).y()));
    }
    RingCurve->data()->set(RingCurveData, false);

    qplot->xAxis->setScaleRatio(qplot->yAxis);
    RingCurve->setPen(pen);
    qplot->replot();
    return true;
}

bool drawVpnt(const vPnt &vpnt, QCustomPlot *qplot)
{
    QCPCurve *vpntCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
    ////QVector
    QVector<QCPCurveData> vpntCurveData;

    for (size_t i = 0; i < vpnt.size(); i++)
    {
        //
        vpntCurveData.push_back(QCPCurveData(i, vpnt.at(i).x(), vpnt.at(i).y()));
    }
    vpntCurve->data()->set(vpntCurveData, false);

    qplot->xAxis->setScaleRatio(qplot->yAxis);

    QPen pen;
    pen.setColor(Qt::GlobalColor::magenta);
    //pen.setColor
    pen.setWidth(1);
    //RingCurve->setPen(pen);
    vpntCurve->setPen(pen);
    qplot->replot();

    return false;
}

bool drawVpnt(const vPnt &vpnt, QCustomPlot *qplot, const QPen &pen)
{
    QCPCurve *vpntCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
    ////QVector
    QVector<QCPCurveData> vpntCurveData;

    for (size_t i = 0; i < vpnt.size(); i++)
    {
        //
        vpntCurveData.push_back(QCPCurveData(i, vpnt.at(i).x(), vpnt.at(i).y()));
    }
    vpntCurve->data()->set(vpntCurveData, false);

    qplot->xAxis->setScaleRatio(qplot->yAxis);
    vpntCurve->setPen(pen);
    qplot->replot();
    return false;
}

bool drawRSPath(const RSPathWithSE &path, const double &rad, QCustomPlot *qplot, const QPen &pen)
{
    ReedsSheppStateSpace space(rad);
    double q0[3],q1[3];
    q0[0] = std::get<0>(path._start).x();
    q0[1] = std::get<0>(path._start).y();
    q0[2] = std::get<1>(path._start);

    q1[0] = std::get<0>(path._end).x();
    q1[1] = std::get<0>(path._end).y();
    q1[2] = std::get<1>(path._end);
    RsPathPnt data;
    space.sample(q0,q1,0.5,sampleRsPath,&data);


    QCPCurve *vpntCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
    ////QVector
    QVector<QCPCurveData> vpntCurveData;

    for (size_t i = 0; i < data.size; i++)
    {
        //
        vpntCurveData.push_back(QCPCurveData(i, data.x[i], data.y[i]));
    }
    vpntCurve->data()->set(vpntCurveData, false);

    qplot->xAxis->setScaleRatio(qplot->yAxis);
    vpntCurve->setPen(pen);
    qplot->replot();

    return false;

}

bool drawvRSPath(const std::vector<RSPathWithSE> &vpath, const double &rad, QCustomPlot *qplot, const QPen &pen)
{
    for(auto &it :vpath)
    {
        drawRSPath(it,rad,qplot,pen);
    }
    return false;
}
