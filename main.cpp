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


bool IsCongruence(polygon_i polygon1,polygon_i polygon2){
    auto sumSquare=[](point_i point1,point_i point2)
    {
        int x_2=pow(point1.x()-point2.x(),2.0);
        int y_2=pow(point1.y()-point2.y(),2.0);
        return x_2+y_2;
    };

    std::vector<int> polygon1Lines;
    for(int i=0;i<polygon1.outer().size()-1;i++){
        polygon1Lines.push_back(
                        sumSquare(polygon1.outer().at(i),polygon1.outer().at(i+1))
                    );
    }
    for(int a : polygon1Lines){
        std::cout<<polygon1Lines.size()<<std::endl;
    }
    return false;
}

int main()
{
    polygon_i poly;
    exterior_ring(poly) = boost::assign::list_of<point_i>
            (0, 0)
            (3, 4)
            (3, 1)
            (0, 0)
            ;
//    linestring line=linestring(poly.outer().at(0),poly.outer().at(1));
    bool a=IsCongruence(poly,poly);
//    std::cout << a << std::endl;
    return 0;
}
