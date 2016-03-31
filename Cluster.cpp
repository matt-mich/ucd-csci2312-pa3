//
// Created by Matthew Michaelis on 2/16/2016.
//

#include <algorithm>
#include "Cluster.h"
#include <sstream>
#include "Exceptions.h"
#include <cstdio>
namespace Clustering {

    unsigned int Cluster::__idGenerator = 0;

    // C E N T R O I D

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __p(d), __c(c){
        __valid = false;
        __dimensions = d;
    }
    const Point Cluster::Centroid::get() const{
        return __p;
    } // doesn't check for validity
    void Cluster::Centroid::set(const Point &p){
        __p = p;
        setValid(true);
    } // sets to valid
    bool Cluster::Centroid::isValid() const{
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid){
        __valid = valid;
    }

    // functions
    void Cluster::Centroid::compute() {

        LNodePtr temp = __c.__points;

        if (__c.__points == nullptr) {
            toInfinity();
            return;
        }

        double sum = 0;
        int i = 0;
        while( i < __dimensions) {
            for (int j = 0; j < __c.__size; ++j) {
                if(temp != nullptr){
                    sum += temp->point[i];
                    temp = temp->next;
                }
            }
            __p.setValue(i, sum / (__c.getSize()));
            sum = 0;
            temp = __c.__points;
            i++;
        }
            setValid(true);
        }

    bool Cluster::Centroid::equal(const Point &P2) const{
        if(__p.getDims() != P2.getDims()){
            throw DimensionalityMismatchEx(__p.getDims(),P2.getDims());
        }
        bool pass = true;

        for (int i = 0; i < __p.getDims(); ++i){
            if(__p.getValue(i) != P2.getValue(i)){
                pass = false;
            }
        }
        return pass;
    }
    void Cluster::Centroid::toInfinity(){
        for(int i = 0; i < __p.getDims(); ++i){
            __p[i] = std::numeric_limits<double>::max();
        }
        setValid(true);
    }
   LNode::LNode(const Point &p, LNodePtr n) : point(p){
       next = n;
    }

    Cluster::Cluster(unsigned int d) : centroid(d,*this){
        if(d == 0){
            throw ZeroDimensionsEx();
        }
        __size = 0;
        __points = nullptr;
        __dimensionality = d;
        __id = __idGenerator++;
    }

    Cluster::Cluster(const Cluster &C1) : centroid(C1.__dimensionality,C1){
        __id = C1.__id;
        __dimensionality = C1.__dimensionality;
        __size = C1.getSize();
        if(C1.__points != nullptr){
            __cpy(C1.__points);
        }
        else{
            __points = nullptr;
        }
    }

    Cluster& Cluster::operator=(const Cluster & other){
        if(__dimensionality != other.__dimensionality){
            throw DimensionalityMismatchEx(__dimensionality,other.__dimensionality);
        }
        if(this != &other){
            //delete & copy
            __id = other.__id;
            __size = other.__size;
            __del();
            if(other.__points != nullptr){
                __cpy(other.__points);
            }
            else{
                __points = nullptr;
            }
        }
        return *this;
    }

    Cluster::~Cluster(){
        __del();
    }


    unsigned int Cluster::getDimensionality() const{
        return __dimensionality;
    }
    unsigned int Cluster::getId() const{
        return __id;
    }

    unsigned int Cluster::getSize() const{
        return __size;
    }

    void Cluster::__del(){
        LNodePtr curr = __points;

        LNodePtr next;

        for(int i = 0; i < __size; ++i){
            if(curr != nullptr){
                next = curr->next;
                delete curr;
                curr = next;
            }
        }
        __points = nullptr;
    }

    void Cluster::__cpy(LNodePtr pts){
        if(pts != nullptr){
            LNodePtr null = nullptr;
            LNode *head = new LNode(pts->point,null);
            LNodePtr ptsCurr = pts->next;
            __points = head;
            LNodePtr curr = __points;

            while(ptsCurr != nullptr){
                curr->next = new LNode(ptsCurr->point,null);
                curr = curr->next;
                ptsCurr = ptsCurr->next;
            }
        }
    }

    bool Cluster::__in(const Point &p) const{

    }

    void Cluster::add(const Point & pNew) {

        if (__points == nullptr) {
            LNode *N = new LNode(pNew, nullptr);
            __points = N;
            ++__size;
            centroid.setValid(false);
            return;
        }

        if(__dimensionality != pNew.getDims()){
            throw DimensionalityMismatchEx(__dimensionality, pNew.getDims());
        }

        if (__points->next == nullptr) {
            LNode *N = new LNode(pNew, nullptr);
            if(N->point < __points->point){
                N->next = __points;
                __points = N;
                ++__size;
                centroid.setValid(false);
                return;
            } else {
                __points->next = N;
                ++__size;
                centroid.setValid(false);
                return;
            }
        }

        LNode *N = new LNode(pNew, nullptr);
        LNodePtr temp = __points->next;
        LNodePtr prev = __points;

        if(N->point < __points->point){
            __points = N;
            N->next = prev;
            ++__size;
            centroid.setValid(false);
            return;
        }

        if(N->point < temp->point){
            prev->next = N;
            N->next = temp;
            ++__size;
            centroid.setValid(false);
            return;
        }

        while (temp->next != nullptr) {
            if(N->point < temp->point){
                prev->next = N;
                N->next = temp;
                ++__size;
                centroid.setValid(false);
                return;
            }
            temp = temp->next;
            prev = prev->next;
        }
        temp->next = N;
        ++__size;
        centroid.setValid(false);
    }


    const Point & Cluster::remove(const Point &P1) {
        // Check if the given point exists in the cluster.

            LNodePtr temp = __points;

            if (temp->point == P1) {

                temp = __points;
                if (__size > 0) {
                    __points = __points->next;
                    --__size;
                }
                delete temp;
                centroid.setValid(false);
                return P1;
            } else {
                int i = 0;
                LNodePtr prev = temp;
                temp = temp->next;

                for (; i < __size; ++i) {
                    if (temp->point == P1) {
                        if (temp->next == nullptr) {
                            prev->next = nullptr;
                            delete temp;
                            __size--;
                            centroid.setValid(false);
                            return P1;
                        } else {
                            prev->next = temp->next;
                            delete temp;
                            __size--;
                            centroid.setValid(false);
                            return P1;
                        }
                    }
                    temp = temp->next;
                    prev = prev->next;
                }
            }
        centroid.setValid(false);
        return P1;
    }

    bool Cluster::contains(const Point & P1) const{
        if(__points == nullptr){
            return false;
        }
        LNodePtr temp;
        if(P1 == __points->point){
            return true;
        }
        if(__points->next == nullptr){
            return false;
        }
        temp = __points->next;
            for(int i = 1; i < __size; ++i){
                if(temp->point == P1){
                    return true;
                }
                temp = temp->next;
            }
        return false;
    }

    const Point &Cluster::operator[](unsigned int index) const{
        LNodePtr temp;
        temp = __points;
        if(getSize() == 0){
            throw EmptyClusterEx();
        }
        if(index > getSize()-1){
            throw OutOfBoundsEx(getSize()-1,index);
        }

        if(index == 0){
            return __points->point;
        }

        for(int i = 0; i < index; ++i){
            temp = temp->next;
        }
        return temp->point;
    }

    bool operator==(const Cluster & C1, const Cluster &C2){

        if(C1.__dimensionality != C2.__dimensionality){
            throw DimensionalityMismatchEx(C1.__dimensionality, C2.__dimensionality);
        }

        if((C1.__points == nullptr && C2.__points == nullptr)){
            return true;
        }
        if(C1.__points == nullptr || C2.__points == nullptr){
            return false;
        }
        if(C1.__size != C2.__size){
            return false;
        }

        for(int i = 0; i < C1.__size; ++i){
            if(C1[i] != C2[i]){
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Cluster & C1, const Cluster & C2){
        return !(C1 == C2);
    }

    Cluster & Cluster::operator+=(const Point & P1){
        if(__dimensionality != P1.getDims()){
            throw DimensionalityMismatchEx(__dimensionality,P1.getDims());
        }
        this->add(P1);
        return *this;
    }
    Cluster & Cluster::operator-=(const Point & P1){

        if(__dimensionality != P1.getDims()){
            throw DimensionalityMismatchEx(__dimensionality,P1.getDims());
        }
        this->remove(P1);
        return *this;
    }

    // Members: Compound assignment (Cluster argument)
    Cluster &Cluster::operator+=(const Cluster & C2) { // union
        if(__dimensionality != C2.getDimensionality()){
            throw DimensionalityMismatchEx(__dimensionality,C2.__dimensionality);
        }
        for(int i = 0; i < C2.getSize(); ++i){
            if(!(this->contains(C2[i]))){
                this->add(C2[i]);
            }
        }

        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster & C2){ // (asymmetric) difference
        for(int i = 0; i < C2.getSize(); ++i){
            if(this->contains(C2[i])){
                this->remove(C2[i]);
            }
        }
        return *this;
    }
    // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &C1, const Point &P1){
        if(C1.__dimensionality != P1.getDims()){
            throw DimensionalityMismatchEx(C1.__dimensionality,P1.getDims());
        }
        Cluster *C2 = new Cluster(C1);
        if(!(C2->contains(P1))){
            C2->add(P1);
        }else{
            return C1;
        }
        return *C2;
    }
    const Cluster operator-(const Cluster &C1, const Point &P1){
        if(C1.__dimensionality != P1.getDims()){
            throw DimensionalityMismatchEx(C1.__dimensionality,P1.getDims());
        }
        Cluster *C2 = new Cluster(C1);
        if(C2->contains(P1)){
            C2->remove(P1);
        }else{
            return C1;
        }
        return *C2;
    }

    // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &C1, const Cluster &C2){
        Cluster *C3 = new Cluster(C1);
        Cluster *C4 = new Cluster(C2);

        for(int i = 0; i < C4->getSize(); ++i){
            if(!(C3->contains(C2[i]))){
                C3->add(C2[i]);
            }
        }
        delete C4;
        return *C3;
    }

    const Cluster operator-(const Cluster &C1, const Cluster &C2){
        Cluster *C3 = new Cluster(C1);
        Cluster *C4 = new Cluster(C2);

        for(int i = 0; i < C3->getSize(); ++i){
            if(C3->contains(C2[i])){
                C3->remove(C2[i]);
            }
        }
        delete C4;
        return *C3;
    }

    std::ostream &operator<<(std::ostream & out, const Cluster & C1){

        LNodePtr temp = C1.__points;
        for(int i = 0; i < C1.__size; ++i){
            out << temp->point << " : " << C1.__id << std::endl;
            temp = temp->next;
        }
        return out;
    }

    // Function is messy, and probably has a lot of unnecessary stuff, but works.
    std::istream &operator>>(std::istream & in, Clustering::Cluster& C1) {

        std::string temp;
        std::getline(in, temp);
        std::stringstream s;
        s.str(temp);
        try{
            Clustering::Point *test = new Clustering::Point(C1.__dimensionality);
            s >> *test;
            C1.add(*test);
            delete test;
        }catch(Clustering::DimensionalityMismatchEx &ex){
            std::cerr << "Caught exception: " << ex << std::endl;
            Clustering::Point::rewindIdGen();
        }

        while(in.peek() != EOF){
            try{
                Clustering::Point *test = new Clustering::Point(C1.__dimensionality);
                in >> *test;
                C1.add(*test);
                delete test;
            }catch(Clustering::DimensionalityMismatchEx &ex){
                std::cerr << "Caught exception: " << ex << std::endl;
                Clustering::Point::rewindIdGen();
                 }

        }

        return in;

        // Old, messy code below.
        /*
        int dim = 0;
        double tempD = 0;
        /*
        while (!s.eof()) {
            s >> tempD;
            s.ignore(10, ',');
            dim++;
        }

        dim = C1.__dimensionality;
        bool run = false;
        int index = 0;
        while (in.peek() != EOF) {
            Clustering::Point *nPoint = new Clustering::Point(dim);
            if (run) {
                temp = "";
                std::getline(in, temp);
                temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
            }
                s.str("");
                s.clear();
                s.str(temp);

                while ((s.peek() != '\n') || (s.peek() != '\r') || (s.peek() != EOF)) {
                    s >> tempD;
                    try{
                        nPoint->setValue(index, tempD);
                    }catch(Clustering::)


                    if ((s.peek() == '\n') || (s.peek() == '\r') || (s.peek() == EOF)) {

                        run = true;
                        C1.add(*nPoint);
                        index = 0;
                        delete nPoint;
                        break;
                    } else {
                        s.ignore();
                        index++;
                    }
                }
        }
        return in;
    */
        }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray){
        if(k <= __size){
            for(int i = 0; i < k; i++) {
                *pointArray[i] = (*this)[i];
            }
        }else{
            for(int i = 0; i < __size; i++){
                *pointArray[i] = (*this)[i];
            }
            for(int i = __size; i < k; ++i){
                for(unsigned int j = 0; j < __dimensionality; ++j){
                    pointArray[i]->setValue(j, std::numeric_limits<double>::max());
                }
            }
        }
    }
    // M O V E

    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __from(from), __to(to), __p(p){

    }
    void Cluster::Move::perform(){
        Point *p = new Point(__p);
        __from.remove(__p);
        __to.add(*p);
        delete p;

    }
}

