#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost::geometry;
typedef boost::geometry::model::d2::point_xy<int> point_i;
typedef boost::geometry::model::linestring<point_i> linestring;
typedef boost::geometry::model::polygon<point_i> polygon_i;

bool IsCongruence(polygon_i polygon1 , polygon_i polygon2){

    struct Triangle{
        int line1;
        int line2;
        double angle;
        bool depression;
    };

    auto sumSquare=[](point_i point1 , point_i point2)
    {
        int x_2=pow(point1.x() - point2.x(),2.0);
        int y_2=pow(point1.y() - point2.y(),2.0);
        return x_2 + y_2;
    };

    auto computeAngle = [](point_i pointA , point_i pointB , point_i pointC){
       int a1,a2,b1,b2;
       a1=pointA.x() - pointB.x();
       a2=pointA.y() - pointB.y();
       b1=pointC.x() - pointB.x();
       b2=pointC.y() - pointB.y();
       double bunshi = a1 * b1 + a2 * b2;
       double bunbo=sqrt(pow(a1 , 2) + pow(a2 , 2)) * sqrt(pow(b1 , 2) + pow(b2 , 2));
       return acos(bunshi/bunbo);
   };

    auto isDepression = [](polygon_i polygonA,int number){
        point_i removepoint = polygonA.outer().at(number);
        polygonA.outer().erase(polygonA.outer().begin() + number);
        if(number == 0){
            polygonA.outer().erase(polygonA.outer().end() - 1);
            polygonA.outer().push_back(polygonA.outer().at(0));
        }
        return !disjoint(polygonA , removepoint);
    };

    auto getTriangles = [sumSquare , computeAngle , isDepression](polygon_i polygonA){
        std::vector<point_i> polygonAvector = polygonA.outer();
        int serchNumber = polygonAvector.size() - 2;

        std::vector<Triangle> v;
        for(int i=0 ; i <= serchNumber ; i++){
            int a = i - 1 , b = i , c = i + 1;
            if(a==-1) a = serchNumber;
            v.push_back(
                {
                    sumSquare(polygonAvector.at(a) , polygonAvector.at(b)),
                    sumSquare(polygonAvector.at(b) , polygonAvector.at(c)),
                    computeAngle(polygonAvector.at(a) , polygonAvector.at(b) , polygonAvector.at(c)),
                    isDepression(polygonA , b)
                }
            );
        }
        return v;
    };

    auto equalsTriangles1 = [](Triangle triangleA,Triangle triangleB){
        bool a = triangleA.line1 == triangleB.line1;
        bool b = triangleA.line2 == triangleB.line2;
        bool c = triangleA.angle == triangleB.angle;
        bool d = triangleA.depression == triangleB.depression;
        return a && b && c && d;
    };

    auto equalsTriangles2=[](Triangle triangleA,Triangle triangleB){
        bool a = triangleA.line1 == triangleB.line2;
        bool b = triangleA.line2 == triangleB.line1;
        bool c = triangleA.angle == triangleB.angle;
        bool d = triangleA.depression == triangleB.depression;
        return a && b && c && d;
    };

    auto isThereTriangle = [equalsTriangles1 , equalsTriangles2](Triangle triangle , std::vector<Triangle> vectorA){
        std::vector<std::pair<int , bool>> v;
        int size = vectorA.size();
        for(int i = 0 ; i < size ; i++){
            bool et1 = equalsTriangles1(triangle , vectorA.at(i));
            bool et2 = equalsTriangles2(triangle , vectorA.at(i));
            if(et1||et2) v.push_back(std::pair<int , bool>(i , et1));
        }
        return v;
    };

    auto equalsVector = [equalsTriangles1,equalsTriangles2](std::vector<Triangle> vectorA , std::vector<Triangle> vectorB , bool forward){
        if(vectorA.size() != vectorB.size()) return false;
        int size = vectorA.size();
        for(int i = 0 ; i < size ; i++){
            if(forward){
                if(!equalsTriangles1(vectorA.at(i),vectorB.at(i))) return false;
            }else{
                if(!equalsTriangles2(vectorA.at(i),vectorB.at(size-i-1))) return false;
            }
        }
        return true;
    };

    auto equalsTriangleVectors = [equalsTriangles1 , equalsTriangles2 , isThereTriangle , equalsVector](std::vector<Triangle> vectorA , std::vector<Triangle> vectorB){
        if(vectorA.size() != vectorB.size()) return false;
        std::vector<std::pair<int , bool>> v = isThereTriangle(vectorA.at(0) , vectorB);
        for(int i=0 ; i < v.size() ; i++){
            std::pair<int , bool> intandbool = v.at(i);
            int a = intandbool.first;
            bool b = intandbool.second;
            if(!b) a++;
            for(int j = 0 ; j < a ; j++){
                Triangle temporary = vectorB.at(0);
                vectorB.erase(vectorB.begin());
                vectorB.push_back(temporary);
            }
            if(equalsVector(vectorA , vectorB , b)) return true;
        }
        return false;
    };

    std::vector<Triangle> v1 = getTriangles(polygon1);
    std::vector<Triangle> v2 = getTriangles(polygon2);
    bool answer = equalsTriangleVectors(v1 , v2);
    return answer;
}

int main()
{
    polygon_i poly;
    exterior_ring(poly) = boost::assign::list_of<point_i>
            (3, 4)
            (5, 2)
            (3, 1)
            (1, -2)
            (-4, 5)
            (3, 4)
            ;
    polygon_i poly2;
    exterior_ring(poly2) = boost::assign::list_of<point_i>
            (1, 2)
            (-4, -5)
            (3, -4)
            (5, -2)
            (3, -1)
            (1, 2)
            ;
    reverse(poly2);
    bool a = IsCongruence(poly,poly2);
    std::cout << a << std::endl;
    return 0;
}
