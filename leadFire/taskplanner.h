#ifndef TASKPLANNER_H
#define TASKPLANNER_H
#include "ScoutCar.h"
#include "Agent.hpp"
#include "ssconfig.hpp"
#include "algorithm"


using  Chrom = std::vector<Encode>;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::list;
using std::tuple;

class CarTime{
    double _pathDur;
    double _actDur;
    double _leaveForwardTime;
    double _leaveBackTime;
    double _arriveTime;
};

class TaskPnt
{
public:
    TaskPnt() {}
    bgeo::DPoint pnt;
    double initState;
    double increaseRatio;
};

class TaskPlanner
{
public:
    TaskPlanner(ScoutCar &car): _agent(car){
        _mID = car.getRobotId();
        string str_deg = "Deg";
        str_deg += std::to_string(_mID);
        conf_debug.open(str_deg,std::ios::trunc);
        //_deTxtPtr = std::make_shared<std::ofstream>(/*str_deg.c_str(),std::ios::trunc*/);
    }

    void generatePop(size_t const & mpop_num);
    void pushTarget(TaskPnt const &pnt) {_vTarget.push_back(pnt);}
    void setVTaget(std::vector<TaskPnt> const &vpnt) {_vTarget.clear();
                                                          _vTarget.assign(vpnt.begin(),vpnt.end());}
    std::vector<TaskPnt> getVTarget() const {return _vTarget;}


    bool initConnect();


    bool sendMessage(const size_t myid);

    void pushScoutCar(ScoutCar const &car) {_vScoutCar.push_back(car);}
    std::vector<ScoutCar> getScoutCar() const {return _vScoutCar;}
    ScoutCar getScoutCar(size_t const m_id)  {return _vScoutCar.at(m_id);}
    void setScoutCar(std::vector<ScoutCar> const &_vCar) {_vScoutCar = _vCar;}


private:
    ScoutCar &_agent;
    size_t _popNum;
    vector<Chrom> _pop;
    std::vector<TaskPnt> _vTarget;
    std::vector<ScoutCar> _vScoutCar;
    Chrom generateChrom();
    std::map<int,Chrom> data;

    std::shared_ptr<netAgent::Agent> _cntPtr;
    size_t _mID;
    int portPubBase = 4455;
    int portRepBase = 5555;
    //std::ofstream conf_debug;
    //std::ofstream conf_debug("d",std::ios::trunc);
    //std::shared_ptr<std::ofstream> _deTxtPtr;
    std::ofstream conf_debug;

    void onBroadcast(int ID, int type, void* data, size_t size);
    void onRequest(int ID, int type, void* data, size_t size, netAgent::ReplyInterface reply);

    double decode(vector<Chrom> const &mpop);

    double rationalize(vector<Chrom>  &mpop);

};

#endif // TASKPLANNER_H
