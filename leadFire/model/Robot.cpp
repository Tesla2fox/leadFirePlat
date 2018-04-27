#include "Robot.h"


Robot::Robot()
{
}


Robot::~Robot()
{
}

void Robot::clearNeighbor()
{
	this->_vui_NghId.clear();
}

void Robot::setRobotId(size_t const & val)
{
	this->_ui_Id = val;
}

void Robot::setHumanId(size_t const & val)
{
	this->_ui_HumanId = val;

}

void Robot::setVel(double const & vel)
{
	this->_d_vel = vel;
}

void Robot::setCostLoad(double const & costLoad)
{
	this->_d_costLoad = costLoad;
}

void Robot::setName(std::string const & name)
{
	this->_str_name = name;
}

void Robot::setAccel(double const & al)
{
	this->_d_accel = al;
}

void Robot::setOrient(double const & ori)
{
	this->_d_orient = ori;
}

void Robot::pushNeighbor(size_t const & nei_id)
{
	this->_vui_NghId.push_back(nei_id);
}

void Robot::setPos(double const & x, double const & y)
{
	this->_pos.x(x);
	this->_pos.y(y);
}

void Robot::setPos(bgeo::DPoint const & pnt)
{
	this->_pos = pnt;
}

size_t Robot::getRobotId()
{
	return _ui_Id;
}

size_t Robot::getHumanId()
{
	return _ui_HumanId;
}

double Robot::getVel()
{
	return _d_vel;
}

double Robot::getCostLoad()
{
	return _d_costLoad;
}

std::string Robot::getName()
{
	return _str_name;
}

double Robot::getOrient() const
{
	return this->_d_orient;
}

double Robot::getAccel()
{
	return this->_d_accel;
}

std::vector<size_t> Robot::getNeighbor()
{
	return this->_vui_NghId;
}

bgeo::DPoint Robot::getPos() const
{
	return this->_pos;
}

void Robot::getPos(double & x, double & y) const
{
	x = this->_pos.x();
	y = this->_pos.y();
}

