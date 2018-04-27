#ifndef STADFX_H
#define STADFX_H


#include "tuple"
#include "functional"
#include "random"
#include "time.h"
#include "algorithm"
#include <math.h>
#include <iostream>
#include <map>
#include <chrono>
#include <list>
#include "ssconfig.hpp"

using std::get;

#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/bitstar/BITstar.h>
#include <ompl/util/PPM.h>
#include <ompl/base/spaces/DubinsStateSpace.h>
#include <ompl/base/spaces/ReedsSheppStateSpace.h>
#include <ompl/geometric/planners/PlannerIncludes.h>


#include <ompl/base/PlannerDataGraph.h>
#include <ompl/config.h>

//#include <boost/filesystem.hpp>
#include <iostream>
//#include "bgeometry.h"

namespace ob = ompl::base;
namespace og = ompl::geometric;

extern  std::default_random_engine enge;

#endif // STADFX_H
