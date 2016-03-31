//
// Created by Matthew Michaelis on 2/16/2016.
//

#include "Point.h"
#include <cmath>
#include <sstream>
#include "Exceptions.h"
#include <cstdio> // Needed for EOF
namespace Clustering {

    unsigned int Point::__idGen = 0;

    void Point::rewindIdGen(){
        --__idGen;
    }

    const char POINT_VALUE_DELIM = ',';

    Point::Point(unsigned int dim){
        if(dim <= 0){
            throw ZeroDimensionsEx();
        }
        __id = __idGen++;
        __values = new double[dim];
        __dim = dim;

        for(int i = 0; i < dim; ++i){
            __values[i] = 0.0;
        }
    }

    Point::Point(unsigned int dim, double * value){
        __id = __idGen++;
        __values = value;
        __dim = dim;
    }

    Point::Point(const Point & P1){
        __id = P1.__id;
        __dim = P1.getDims();
        __values = new double[__dim];
        for(int i = 0; i < __dim; ++i){
            __values[i] = P1.__values[i];
        }

    }

    Point& Point::operator=(const Point & other){
        if(this != &other){
            //delete & copy
            //double *newVal = new double{other.getDims()};
            if(__dim != other.__dim){
                throw DimensionalityMismatchEx(__dim,other.__dim);
            }
            __id = other.__id;
            delete[] __values;
            __dim = other.getDims();
            __values = new double[__dim];
            for(int i = 0; i < __dim; ++i){
                __values[i] = other.__values[i];
            }
        }
        return *this;
    }
    // Should automatically delete __values
    Point::~Point(){
            delete []__values;
    }

    int Point::getId() const{
        return __id;
    }
    unsigned int Point::getDims() const{
        return __dim;
    }


    void Point::setValue(unsigned int dim, double val){
        if(dim > __dim-1){
            throw OutOfBoundsEx(__dim,dim);
        }
        __values[dim] = val;
    }

    double Point::getValue(unsigned int dim) const{
        if(dim > __dim-1){
            throw OutOfBoundsEx(__dim,dim);
        }
        return __values[dim];
    }

    double Point::distanceTo(const Point & other) const{

        if(__dim != other.__dim){
            throw DimensionalityMismatchEx(__dim,other.__dim);
        }

        double dist = 0.0;
        for (int i = 0; i < __dim; ++i){
            dist = dist + ((__values[i]-other.getValue(i)) * (__values[i]-other.getValue(i)));
        }
        return sqrt(dist);
    }

    const Point Point::operator*(double mult) const { // prevent (p1 * 2) = p2;
        Point *newP = new Point(__dim);

        for (int i = 0; i < __dim; ++i) {
            newP->__values[i] = __values[i] * mult;
        }

        return *newP;
        }


    const Point Point::operator/(double div) const{ // p3 = p2 / 2;
        Point *newP = new Point(__dim);

        for (int i = 0; i < __dim; ++i) {
            newP->__values[i] = __values[i] / div;
        }

        return *newP;
    }

    double& Point::operator[](unsigned int index){
        if(__dim-1 < index){
            throw OutOfBoundsEx(__dim-1,index);
        }
        return __values[index];
    }
    const double& Point::operator[](unsigned int index) const{
        return __values[index];
    }

    bool operator==(const Point & P1, const Point & P2){
        if(P1.__dim != P2.__dim){
            throw DimensionalityMismatchEx(P1.__dim,P2.__dim);
        }
        bool pass = true;

        if(P1.__id != P2.__id){
            return false;
        }

        if (P1.__dim != P2.__dim){
            return false;
        }

        for (int i = 0; i < P1.getDims(); ++i){
            if(P1.__values[i] != P2.__values[i]){
                pass = false;
            }
        }
        return pass;
    }

    bool operator!=(const Point & P1, const Point & P2){
        return !(P1 == P2);
    }

    bool operator<(const Point & P1, const Point & P2) {
        if(P1.__dim != P2.__dim){
            throw DimensionalityMismatchEx(P1.__dim,P2.__dim);
        }
        if (P1.__dim < P2.__dim) {
            return true;
        }
        if (P1.__dim > P2.__dim) {
            return false;
        }

            for (int i = 0; i < P1.getDims(); ++i) {
                if (P1.__values[i] < P2.__values[i]) {
                    return true;
                }else if(P1.__values[i] > P2.__values[i]){
                    return false;
                }
            }
        return false;

    }

    bool operator>(const Point & P1, const Point &P2){
        if(P1.__dim != P2.__dim){
            throw DimensionalityMismatchEx(P1.__dim,P2.__dim);
        }
        if(P1.__dim > P2.__dim){
            return true;
        }
        if(P1.__dim < P2.__dim){
            return false;
        }

        for(int i = 0; i < P1.getDims(); ++i){
            if(P1.__values[i] > P2.__values[i]){
                return true;
            }else if(P1.__values[i] < P2.__values[i]){
                return false;
            }
        }
        return false;
    }

    bool operator<=(const Point & P1, const Point &P2){
        if (P1<P2) {
            return true;
        }
        if (P1>P2) {
            return false;
        }
        return true;
    }

    bool operator>=(const Point & P1, const Point &P2){
        if (P1>P2) {
            return true;
        }
        if (P1<P2) {
            return false;
        }
        return true;
    }

    Point& operator+=(Point & P1, const Point & P2){
        if(P1.__dim != P2.__dim){
            throw DimensionalityMismatchEx(P1.__dim,P2.__dim);
        }
            Point *newP = new Point(P2);

            for (int i = 0; i < P1.__dim; ++i){
                P1.__values[i] = P1.__values[i] + newP->getValue(i);
            }
        delete newP;
        return P1;
    }

    Point &operator-=(Point & P1, const Point & P2){
        if(P1.__dim != P2.__dim){
            throw DimensionalityMismatchEx(P1.__dim,P2.__dim);
        }

        Point *newP = new Point(P2);

        for (int i = 0; i < P1.__dim; ++i){
            P1.__values[i] = P1.__values[i] - newP->getValue(i);
        }
        delete newP;
        return P1;
    }

    Point & Point::operator*=(double mult){ // p *= 6; p.operator*=(6);
        for (int i = 0; i < __dim; ++i){
            __values[i] = __values[i] * mult;
        }
        return *this;
    }

    Point & Point::operator/=(double div){
        for (int i = 0; i < __dim; ++i){
            __values[i] = __values[i] / div;
        }
        return *this;
    }
    const Point operator+(const Point & P1, const Point & P2){
        Point *newP = new Point(P1);
        *newP += P2;
        return *newP;
    }
    const Point operator-(const Point & P1, const Point & P2){
        Point *newP = new Point(P1);
        *newP -= P2;
        return *newP;
    }

    std::istream &operator>>(std::istream &is, Clustering::Point &p){
        unsigned int index = 0;
        double temp = 0;
        char delim = ',';
        std::string str;
        std::stringstream s;
        getline(is,str);
        s.str(str);
        while ( s.peek() != EOF /*(is.peek() != '\n') || (is.peek() != '\r') <-- OLD METHOD*/){
            s >> temp;
            try{
                p.setValue(index, temp);
            }catch(Clustering::OutOfBoundsEx &ex){
                throw Clustering::DimensionalityMismatchEx(p.__dim,index);
            }

            //if((is.peek() == '\n') || (is.peek() == '\r') || (is.eof())){  <-- OLD METHOD
            if(s.peek() == EOF){
                if(index != p.__dim-1){
                    throw Clustering::DimensionalityMismatchEx(p.__dim,index);
                }
                return is;
            }
            s.ignore(100, delim);
            index++;
        }
        index--;
        if(index != p.__dim-1){
            throw Clustering::DimensionalityMismatchEx(p.__dim,index);
        }
        return is;
    }

    std::ostream &operator<<(std::ostream &out, const Point &P1){
        int i = 0;
        for( ; i < P1.getDims()-1; ++i){
            out << P1.getValue(i);
                out << POINT_VALUE_DELIM << " ";
        }
        out << P1.getValue(i);

        return out;
    }


}
