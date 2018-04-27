#pragma once
#include "stadfx.h"
#include "Robot.h"
#include "reeds_shepp.h"

namespace  rs = ReedsShepp;
using  rs::ReedsSheppStateSpace;
using  std::tuple;
using DirPnt = tuple<bgeo::DPoint,double>;
//using



enum generatorType
{
    randType,
    greedType
};
//extern std::default_random_engine enge();
class RSPathWithSE
{
public:
    RSPathWithSE() {}
    ReedsSheppStateSpace::ReedsSheppPath _path;
    DirPnt _start;
    DirPnt _end;
};


class Encode
{
public:
    Encode();
    Encode(size_t id, double angle,double dur):_id(id),_angle(angle),_dur(dur)  {}
    //double _x;
    size_t _id;
    double _angle;
    //during time
    double _dur;

};
inline std::ostream& operator<<(std::ostream& os,const Encode & data)
{
    return os<<"id = "<<data._id << " ,dur = "<<data._dur
            <<", angle = "<<data._angle << std::endl;
}



///
/// \brief randN  random arrangement
/// \param n
/// \return
///
std::vector<size_t> randN(size_t const &n );

class ScoutCar :
	public Robot
{
public:
    //ScoutCar();
    ScoutCar(double const &c_x,double const &c_y,double const &c_edge,double const & c_ori,double const &rad);
	~ScoutCar();

    void setPos(bgeo::DPoint const &pnt);
    bgeo::EqTri getEqTri()  const {return _camRegion;}
    // lead fire attack the target
	bool leadFire();
	//Scout an area 
	bool scoutRegion();
	// scout a target
	bool scoutTarget();
    double getEdge() {return _camRegion.getEdge();}

    //void setTaskNum()

    void pushTarget(bgeo::DPoint const &pnt) {_vTarget.push_back(pnt);}
    void setVTaget(std::vector<bgeo::DPoint> const &vpnt) {_vTarget.clear();
                                                          _vTarget.assign(vpnt.begin(),vpnt.end());}
    bgeo::vPoint getVTarget() const {return _vTarget;}

    // Generation coding
    bool initEncode(size_t const & engType);

    std::vector<RSPathWithSE>getRSpath() const{return this->_vRSPath;}


    bool generateAllPath();
    bool generatePath(size_t const &encodeIndex);


    double   getArriveTime(Encode const &encode);


    ///
    /// \brief encode2DirPoint
    /// \param encode
    /// \return  x,y,angle
    ///
    DirPnt encode2DirPoint(Encode const & encode);

private:

    std::shared_ptr<ReedsSheppStateSpace> _rsSpacePtr;

    //ReedsSheppStateSpace  _rsSpace(getTurnRad());

    std::function<bool()> _generator = nullptr;

    /// greed
    bool greedInitEncode();
    bool randInitEncode();

    ///
    double getEncodeLength(const Encode & en_code);

    double getEncodeLength(const Encode &en_code1,const Encode & en_code2);

    double getListEncode (const std::list<Encode> &lencode);

    // scout  region
	bgeo::EqTri _camRegion;
    std::vector<bgeo::DPoint> _vTarget;
    std::vector<Encode> _vEncode;
    //std::vector<ReedsSheppStateSpace::ReedsSheppPath> _vPath;
    std::vector<RSPathWithSE> _vRSPath;


    //std::vector<>
};

