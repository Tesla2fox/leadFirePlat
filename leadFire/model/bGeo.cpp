

#include "bGeo.hpp"

namespace bgeo {

	EqTri::EqTri()
	{
	}

	EqTri::~EqTri()
	{
	}

	void EqTri::setPos(DPoint const & pnt, double const & ori)
	{
		this->_vertex = pnt;
		this->_d_orient = ori;
        auto angel_1 = ori -  _M_PI / 6;
        auto angel_2 = ori +  _M_PI / 6;
		double vert1_x, vert1_y;
		double vert2_x, vert2_y;
        vert1_x = this->_vertex.x() + cos(angel_1)*_d_edge;
        vert1_y = this->_vertex.y() + sin(angel_1)*_d_edge;

        vert2_x = this->_vertex.x() + cos(angel_2)*_d_edge;
        vert2_y = this->_vertex.y() + sin(angel_2)*_d_edge;
		_triangle.clear();
		_triangle.push_back(_vertex);
		DPoint pnt1(vert1_x, vert1_y);
		_triangle.push_back(pnt1);

		DPoint pnt2(vert2_x, vert2_y);
		_triangle.push_back(pnt2);
		_triangle.push_back(_vertex);
	}

	bool EqTri::inScout(DPoint const & pnt) const
	{
		return boost::geometry::within(pnt, _triangle);
	}

	void EqTri::setEdge(double const & edge)
	{
		this->_d_edge = edge;
	}

}
