#include "planner2d.h"

//Planner2D::Planner2D()
//{

//}

bool Planner2D::plan()
{
    this->init();
    if(!p_ss_)
    {
        return false;
    }

    ob::ScopedState<> start(p_ss_->getStateSpace());
    start[0] = _scoutAgent.getPos().x();
    start[1] = _scoutAgent.getPos().y();
    start[2] = _scoutAgent.getOrient();

    ob::ScopedState<> goal(p_ss_->getStateSpace());
    goal[0] = _data.getAttackedPnt().x();
    goal[1] = _data.getAttackedPnt().y();
    goal[2] = 0;

    p_ss_->setStartAndGoalStates(start,goal);

    p_ss_->setup();
    p_ss_->print();

    ob::PlannerStatus solved = p_ss_->solve(2.0);

    std::shared_ptr<ob::PlannerData> _p_plData;
    _p_plData = std::make_shared<ob::PlannerData>(p_ss_->getSpaceInformation());
    p_ss_->getPlannerData(*_p_plData);


    _p_plData->printGraphML();

    og::PathGeometric path(p_ss_->getSpaceInformation());
    if (p_ss_->haveSolutionPath())
    {
        path = p_ss_->getSolutionPath();
    }
    else
    {
        std::cout<<" failed "<<std::endl;
    };
    //path.smoothness();
    auto vstate = path.getStates();
    for (auto it:vstate)
    {
        bgeo::DPoint pnt;
        //pnt.x(it->as<ob::RealVectorStateSpace::StateType>()->values[0]);
        //pnt.y(it->as<ob::RealVectorStateSpace::StateType>()->values[1]);
        const auto *s = it->as<ob::SE2StateSpace::StateType>();
        //            auto  ReedsShepp_path
        //           = p_ss_->getStateSpace()->reedsShepp(vstate.at(0),vstate.at(1));
        //ob::ReedsSheppStateSpace.reedsShepp()
        double x = s->getX();
        double y = s->getY();
        pnt.x(x);
        pnt.y(y);

        //vPnt.push_back(pnt);
        //this->_m_path.push_back(pnt);
        this->_path.push_back(pnt);
    }

}

void Planner2D::init()
{
    //auto space(std::make_shared<ob::RealVectorStateSpace>());

    ob::StateSpacePtr space(std::make_shared<ob::ReedsSheppStateSpace>(2));
    //space->addDimension(0.0,320);
    //space->addDimension(0.0,180);

    ob::ReedsSheppStateSpace wtf(2);

    ob::ReedsSheppStateSpace::StateType q0;
    ob::ReedsSheppStateSpace::StateType q1;

    //q0[]

    q0.setX(1);
    q0.setXY(0,0);
    q0.setYaw(0);

    q1.setXY(10,0);
    q1.setYaw(0);

    //auto dis = wtf.distance(q0,q1);
    //q0.operator [](0) = 0;
    //q0.v
    //q0[0] = 0;

    //    const auto * s = q0->as<ob::SE2StateSpace::StateType>();


    //    double x = s->getX();
    //    double y = s->getY();

    //       = wtf.distance()


    ob::RealVectorBounds bounds(2);

    bounds.setLow(0);

    bounds.setHigh(0,320);
    bounds.setHigh(1,180);

    //space->as<ob::SE2StateSpace>()->setBounds(bounds);

    space->as<ob::SE2StateSpace>()->setBounds(bounds);
    space->setup();

    p_ss_ = std::make_shared<og::SimpleSetup>(space);


    p_ss_->setStateValidityChecker
            ([=](const ob::State *state) { return isStateValid(state); });
    //    p_ss_->setPlanner(std::make_shared<og::PRM>(p_ss_->getSpaceInformation()));

    //    p_ss_->setPlanner(std::make_shared<og::PRM>(p_ss_->getSpaceInformation()));


    p_ss_->setPlanner(std::make_shared<og::BITstar>(p_ss_->getSpaceInformation()));


    _path.clear();
}

bool Planner2D::isStateValid(const ompl::base::State *state) const
{
    //    auto &x = state->as<ob::RealVectorStateSpace::StateType>()->values[0];
    //    auto &y = state->as<ob::RealVectorStateSpace::StateType>()->values[1];

    //return true;
    const auto * s = state->as<ob::SE2StateSpace::StateType>();

    double x = s->getX();
    double y = s->getY();

    bgeo::DPoint pnt(x,y);
    //auto &vRing = this->_data.getVRing();

    auto vRingPtr =  this->_data.getVRingPtr();
    for(auto &it : *vRingPtr)
    {
        bool inOb = bg::within(pnt,it);
        if(inOb)
        {
            return false;
        }
    }
    return true;
}
