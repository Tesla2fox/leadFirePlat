#ifndef ROBOTTHREAD_H
#define ROBOTTHREAD_H

#include <thread>

#include "taskplanner.h"
#include "maindata.h"

//creat a robot thread
void Robot_thread(size_t  const &id);

void scoutCarThread(size_t const &id);

class RobotThread
{
public:
    RobotThread();
};

#endif // ROBOTTHREAD_H
