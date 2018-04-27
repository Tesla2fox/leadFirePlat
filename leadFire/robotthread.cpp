#include "robotthread.h"

std::vector<ScoutCar> ex_ScoutCar;
std::vector<bgeo::DPoint> ex_TargetPnt;

RobotThread::RobotThread()
{

}

void scoutCarThread(const size_t &id)
{
    //TaskPlanner planner()
    TaskPlanner planner(ex_ScoutCar.at(id));
    planner.setScoutCar(ex_ScoutCar);

    // set the task pnt
    std::vector<TaskPnt> vpnt;
    for(size_t i = 0 ; i < ex_TargetPnt.size() ; i++)
    {
        TaskPnt tpnt;
        tpnt.pnt = ex_TargetPnt[i];
        tpnt.initState = 20;
        vpnt.push_back(tpnt);
    }
    planner.setVTaget(vpnt);

    planner.generatePop(vpnt.size());


    cout << "decode success "<< endl;
    //
    planner.initConnect();
    //TaskPnt pnt;
    //planner.generateChrom();
    std::this_thread::sleep_for (std::chrono::seconds(5));
    std::cout << "pause of "  << " 5 seconds ended\n";

}
