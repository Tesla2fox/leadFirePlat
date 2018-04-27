#pragma once

#include <string>
#include <vector>

#include "bGeo.hpp"


class Robot
{
public:
	Robot();
	~Robot();

	//clear
	void clearNeighbor();
	 
	//setFunction
	void setRobotId(size_t const & val);
	void setHumanId(size_t const & val);
	void setVel(double const &vel);
	void setCostLoad(double const &costLoad);
	void setName(std::string const &name);
	void setAccel(double const &al);
	void setOrient(double const &ori);
	void pushNeighbor(size_t const &nei_id);
	virtual void setPos(double const &x, double const&y);
	virtual void setPos(bgeo::DPoint const &pnt);
    void setTurnRad(double const &rad) { this->_d_turnRad =rad;}
	
	//getFunction
	size_t getRobotId();
	size_t getHumanId();
	double getVel();
	double getCostLoad();
	std::string getName();
    double getOrient() const;
	double getAccel();
	std::vector<size_t> getNeighbor();
    bgeo::DPoint getPos() const ;
    void getPos(double &x, double &y) const;
    double getTurnRad() const {return _d_turnRad;}



protected:
	size_t _ui_Id;
	size_t _ui_HumanId;
	double _d_vel;
	double _d_costLoad;
	std::string _str_name;
	// orientation of the robot
	double _d_orient;
	//  the acceleration of the robot 
	double _d_accel;
	// the id of  all neighbors 
	std::vector<size_t> _vui_NghId;
	bgeo::DPoint _pos;
	std::vector<bgeo::DPoint> _path;
    double _d_turnRad;

};

