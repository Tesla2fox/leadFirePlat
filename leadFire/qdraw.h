#ifndef QDRAW_H
#define QDRAW_H
#include "qcustomplot.h"
#include "Robot.h"
#include "ScoutCar.h"
#include "maindata.h"
#include "stadfx.h"


struct RsPathPnt{
    double x[1000];
    double y[1000];
    double w[1000];
    size_t size = 0;
};
inline int sampleRsPath(double *q,void *usr_data)
{
    auto ptr = reinterpret_cast<RsPathPnt*>(usr_data);

    if(ptr->size>1000)
    {
        ptr->size = 999;
    }
    ptr->size += 1;
    ptr->x[ptr->size - 1] = q[0];
    ptr->y[ptr->size - 1] = q[1];
    ptr->w[ptr->size - 1] = q[2];
    return -1;
}

bool drawScoutCar(ScoutCar const & agent, QCustomPlot * qplot);


bool drawRing(bgeo::DRing const &ring, QCustomPlot * qplot);

bool drawRing(bgeo::DRing const &ring, QCustomPlot * qplot, QPen const &pen);


bool drawMainData(MainData const &_data,QCustomPlot * qplot);

bool drawVpnt(vPnt const &vpnt,QCustomPlot * qplot);

bool drawVpnt(const vPnt &vpnt, QCustomPlot *qplot,QPen const &pen);


bool drawvRSPath(std::vector<RSPathWithSE>const & vpath,double const &rad,QCustomPlot *qplot,QPen const &pen);


bool drawRSPath(const RSPathWithSE & path,double const &rad,QCustomPlot *qplot,QPen const &pen);


#endif // QDRAW_H
