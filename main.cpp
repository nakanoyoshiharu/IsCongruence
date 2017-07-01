#include <iostream>
#include <math.h>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost::geometry;
typedef boost::geometry::model::d2::point_xy<int> point_i;
typedef boost::geometry::model::linestring<point_i> linestring;
typedef boost::geometry::model::polygon<point_i> polygon_i;

struct Triangle{
    int line1;
    int line2;
    double angle;
};

bool IsCongruence(polygon_i polygon1,polygon_i polygon2){
    auto sumSquare=[](point_i point1,point_i point2)
    {
        int x_2=pow(point1.x()-point2.x(),2.0);
        int y_2=pow(point1.y()-point2.y(),2.0);
        return x_2+y_2;
    };
    auto getTriangles=[](polygon_i polygonA){
        std::vector<Triangle> v;
        for(point_i x : polygonA.outer()) {

        }
    };

    return false;
}

int main()
{
    polygon_i poly;
    exterior_ring(poly) = boost::assign::list_of<point_i>
            (-4, 5)
            (3, 4)
            (5, 2)
            (3, 1)
            (1, -2)
            (-4, 5)
            ;
    bool a=IsCongruence(poly,poly);
    std::cout << a << std::endl;
    return 0;
}
