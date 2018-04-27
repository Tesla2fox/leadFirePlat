#ifndef BGEOMETRY_H
#define BGEOMETRY_H
//#include "boos

#define _M_PI 3.1415926


#include <math.h>
//几何head——files
#include "boost/geometry/geometry.hpp"
#include "boost/geometry/geometries/point_xy.hpp"
#include "boost/geometry/geometries/linestring.hpp"
#include "boost/geometry/geometries/linestring.hpp"
#include "boost/geometry/geometries/box.hpp"
#include "boost/geometry/geometries/ring.hpp"
#include "boost/geometry/geometries/polygon.hpp"

//graph  head files
#include "boost/graph/graph_traits.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include "boost/graph/kruskal_min_spanning_tree.hpp"


//std
#include <vector>
#include <map>
namespace bg = boost::geometry;
namespace bt = boost;


/*geometry lib */
namespace bgeo {
	//点
	typedef bg::model::d2::point_xy<double> DPoint;
	//线段
	typedef bg::model::segment<DPoint> DSegment;
	//折线段
	typedef bg::model::linestring<DPoint> DLineString;
	//
	typedef bg::model::ring<DPoint, false> DRing;

	typedef bg::model::polygon<DPoint, false> DPolygon;

    using   vPoint = std::vector<DPoint>;

	//
	class EqTri
	{
	public:
		EqTri();
		~EqTri();
		void setPos(DPoint const &pnt,double const &ori);
		bool inScout(DPoint const &pnt) const;
		void setEdge(double const &edge);

        DRing getTriangle() const {return this->_triangle;}
        double getEdge() const{return this->_d_edge;}

	private:
		DPoint  _vertex;
		DRing _triangle;
		double _d_edge;
		double _d_orient;

	};

}

#endif // BGEOMETRY_H
