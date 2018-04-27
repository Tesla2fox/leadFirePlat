#ifndef MAINDATA_H
#define MAINDATA_H
#include <vector>
#include <memory>
#include "ScoutCar.h"
#include "bGeo.hpp"
#include "obmap.h"
#include "xml/tinystr.h"
#include "xml/tinyxml.h"

#include <iostream>


extern std::vector<ScoutCar> ex_ScoutCar;
extern std::vector<bgeo::DPoint> ex_TargetPnt;
using vPnt = std::vector<bgeo::DPoint>;

class MainData
{
public:
    MainData();
    void pushTarget(bgeo::DPoint const &pnt) {_vTarget.push_back(pnt);}
    void setVTaget(std::vector<bgeo::DPoint> const &vpnt) {_vTarget.clear();
                                                          _vTarget.assign(vpnt.begin(),vpnt.end());}
    //std::vector<>
    bgeo::vPoint getVTarget() const {return _vTarget;}

    void loadMap (std::shared_ptr<obmap> pMap) { _p_mainMap = pMap;}

    void writeXml();

    void loadXml(const char *pFileName);

    //loadMap(obmap & const m_map){}

    void setAttackedPnt(bgeo::DPoint const &pnt) { this->_attackedPnt = pnt;}
    bgeo::DPoint getAttackedPnt() const {return this->_attackedPnt;}


    void setWSpace (bgeo::DRing const & ring) {this->_wspace = ring;}
    bgeo::DRing getWSpace () const { return this->_wspace;}
    void wSPushback(bgeo::DPoint const &pnt) { this->_wspace.push_back(pnt);}



    void SetVRing(std::shared_ptr<std::vector<bgeo::DRing>> ptr) {_vRingPtr = ptr;}
    void pushRing(bgeo::DRing const &ring ){_vRingPtr->push_back(ring);}
    std::vector<bgeo::DRing> getVRing() const {return *_vRingPtr;}

    std::shared_ptr<std::vector<bgeo::DRing>> getVRingPtr() {return _vRingPtr;}

    void pushScoutCar(ScoutCar const &car) {_vScoutCar.push_back(car);}
    std::vector<ScoutCar> getScoutCar() const {return _vScoutCar;}
    ScoutCar getScoutCar(size_t const m_id)  {return _vScoutCar.at(m_id);}
    void setScoutCar(std::vector<ScoutCar> const &_vCar) {_vScoutCar = _vCar;}

    void initScoutCarEncode();

private:
    //obmap _mainMap;
    std::shared_ptr<obmap> _p_mainMap;
    std::vector<bgeo::DPoint> _vTarget;
    std::vector<ScoutCar> _vScoutCar;
    bgeo::DPoint _attackedPnt;
    //work space
    bgeo::DRing   _wspace;
    std::shared_ptr<std::vector<bgeo::DRing>> _vRingPtr = nullptr;

};

#endif // MAINDATA_H
