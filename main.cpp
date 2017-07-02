#include <iostream>
#include <iomanip>
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
    bool depression;
};

bool IsCongruence(polygon_i polygon1,polygon_i polygon2){
    auto sumSquare=[](point_i point1,point_i point2)
    {
        int x_2=pow(point1.x()-point2.x(),2.0);
        int y_2=pow(point1.y()-point2.y(),2.0);
        return x_2+y_2;
    };

    auto computeAngle=[](point_i pointA,point_i pointB,point_i pointC){
       int a1,a2,b1,b2;
       a1=pointA.x()-pointB.x();
       a2=pointA.y()-pointB.y();
       b1=pointC.x()-pointB.x();
       b2=pointC.y()-pointB.y();
       double bunshi=a1*b1+a2*b2;
       double bunbo=sqrt(pow(a1,2)+pow(a2,2))*sqrt(pow(b1,2)+pow(b2,2));
       return acos(bunshi/bunbo);
   };

    auto isDepression=[](polygon_i polygonA,int number){
        point_i removepoint=polygonA.outer().at(number);
        polygonA.outer().erase(polygonA.outer().begin()+number);
        if(number==0){
            polygonA.outer().erase(polygonA.outer().end()-1);
            polygonA.outer().push_back(polygonA.outer().at(0));
        }
        return !disjoint(polygonA,removepoint);
    };

    auto getTriangles=[sumSquare,computeAngle,isDepression](polygon_i polygonA){
        std::vector<point_i> polygonAvector=polygonA.outer();
        int serchNumber=polygonAvector.size()-2;

        std::vector<Triangle> v;
        for(int i=0;i<=serchNumber;i++){
            int a=i-1,b=i,c=i+1;
            if(a==-1) a=serchNumber;
            v.push_back(
                {
                    sumSquare(polygonAvector.at(a),polygonAvector.at(b)),
                    sumSquare(polygonAvector.at(b),polygonAvector.at(c)),
                    computeAngle(polygonAvector.at(a),polygonAvector.at(b),polygonAvector.at(c)),
                    isDepression(polygonA,b)
                }
            );
        }
        for(Triangle t:v){
            std::cout<<"line1:"<<std::setw(2)<<t.line1<<" line2:"<<std::setw(2)<<t.line2<<" angle:"<<t.angle<<" depression:"<<t.depression<<std::endl;
        }
        return v;
    };

    auto equalsTriangles=[](Triangle triangleA,Triangle triangleB){
        bool a=triangleA.line1==triangleB.line1;
        bool b=triangleA.line2==triangleB.line2;
        bool c=triangleA.angle==triangleB.angle;
        bool d=triangleA.depression==triangleB.depression;
        return a&&b&&c&&d;
    };

    auto equalsTriangles2=[](Triangle triangleA,Triangle triangleB){
        bool a=triangleA.line1==triangleB.line2;
        bool b=triangleA.line2==triangleB.line1;
        bool c=triangleA.angle==triangleB.angle;
        bool d=triangleA.depression==triangleB.depression;
        return a&&b&&c&&d;
    };

    //vectorAの中にtriangleAと同じものは何番目に入っているか(戻り値std::vector<int>)
    auto countSameTriangles1=[equalsTriangles](Triangle triangleA,std::vector<Triangle> vectorA){
        bool et1;
        int count=0;
        std::vector<int> v;
        for(Triangle t:vectorA){
            et1=equalsTriangles(triangleA,t);
            if(et1) v.push_back(count);
            count++;
        }
        return v;
    };

    //vectorAの中にtriangleAと同じものは何番目に入っているか(戻り値std::vector<int>)
    auto countSameTriangles2=[equalsTriangles2](Triangle triangleA,std::vector<Triangle> vectorA){
        bool et2;
        int count=0;
        std::vector<int> v;
        for(Triangle t:vectorA){
            et2=equalsTriangles2(triangleA,t);
            if(et2) v.push_back(count);
            count++;
        }
        return v;
    };

    auto equalsTriangleVectors=[equalsTriangles,equalsTriangles2,countSameTriangles1,countSameTriangles2](std::vector<Triangle> vectorA,std::vector<Triangle> vectorB){
        std::vector<int> v=countSameTriangles1(vectorA.at(0),vectorB);
        std::vector<int> v2=countSameTriangles2(vectorA.at(0),vectorB);
        for(int i=0;i<v.size()-1;i++){
            int a=v.at(i);
            int count=0;
            for(int j=0;j<vectorA.size()-1;j++,a++){
                if(vectorB.size()<=a) a=0;
                if(equalsTriangles(vectorA.at(j),vectorB.at(a))) count++;
            }
            if(count==vectorA.size()-1)return true;
        }
        for(int i=0;i<v2.size()-1;i++){
            int a=v2.at(i);
            int count=0;
            for(int j=0;j<vectorA.size()-1;j++,a++){
                if(vectorB.size()<=a) a=0;
                if(equalsTriangles2(vectorA.at(j),vectorB.at(a))) count++;
            }
            if(count==vectorA.size()-1)return true;
        }
        return false;
    };

    std::vector<Triangle> v1=getTriangles(polygon1);
    std::vector<Triangle> v2=getTriangles(polygon2);
    bool answer=equalsTriangleVectors(v1,v2);
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
            (5, 2)
            (3, 4)
            (-4, 5)
            (1, -2)
            (3, 1)
            (5, 2)
            ;
    polygon_i poly3;
    exterior_ring(poly3) = boost::assign::list_of<point_i>
            (5, 2)
            (3, 1)
            (1, -2)
            (-4, 5)
            (3, 4)
            (5, 2)
            ;
    bool a=IsCongruence(poly,poly3);
    std::cout << a << std::endl;
    return 0;
}
