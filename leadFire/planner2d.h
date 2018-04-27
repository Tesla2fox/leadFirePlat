#ifndef PLANNER2D_H
#define PLANNER2D_H
#include "ScoutCar.h"
#include "stadfx.h"
#include "maindata.h"


class Planner2D
{
public:
    //Planner2D();
    Planner2D(MainData &data,ScoutCar &car) :_data(data),_scoutAgent(car){}

    //void SetVRing(std::shared_ptr<std::vector<bgeo::DRing>> ptr) {_vRingPtr = ptr;}
    //void pushRing(bgeo::DRing const &ring ){_vRingPtr->push_back(ring);}
    //std::vector<bgeo::DRing> getVRing() const {return *_vRingPtr;}

    bool plan();
    void init();
    vPnt getPath() const{return _path;}

private:
    //std::shared_ptr<std::vector<bgeo::DRing>> _vRingPtr = nullptr;
    ScoutCar _scoutAgent;
    MainData  &_data;
    bgeo::DLineString _m_path;
    og::SimpleSetupPtr p_ss_;

    bool isStateValid(const ob::State *state) const;

    std::vector<bgeo::DPoint> _path;

};

#endif // PLANNER2D_H
