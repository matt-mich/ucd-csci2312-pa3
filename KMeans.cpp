//
// Created by Matthw Michaelis on 3/14/2016.
//
#include "KMeans.h"
#include "Exceptions.h"
#include <fstream>
namespace Clustering{
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter){
        if(k == 0){
            throw ZeroClustersEx();
        }
        if(dim == 0){
            throw ZeroDimensionsEx();
        }

        __k = k;
        __dimensionality = dim;
        __maxIter = maxIter;
        __iFileName = filename;
        std::ifstream data;
        data.open(filename);
        if(!data){
            throw DataFileOpenEx(filename);
        }

        __clusters = new Cluster*[k];
        for(int i = 0; i < __k; ++i){
        __clusters[i] = new Cluster(__dimensionality);
        }

        __initCentroids = new Point*[k];
        for(int i = 0; i < __k; ++i){
            __initCentroids[i] = new Point(__dimensionality);
        }

        data >> *__clusters[0];
        __clusters[0]->pickCentroids(k,__initCentroids);
        for(int i = 0; i < __k; ++i){
            __clusters[i]->centroid.set(*__initCentroids[i]);
        }
    }

    unsigned int KMeans::getMaxIter(){
        return __maxIter;
    }
    unsigned int KMeans::getNumIters(){
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters(){
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter(){
        return __numMovesLastIter;
    }

// element access (for testing, no bounds checking)
    Cluster &KMeans::operator[](unsigned int u){

    }
    const Cluster &KMeans::operator[](unsigned int u) const{

    }

// write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans){
        return os;
    }

// clustering functions
    void KMeans::run(){

    }

    KMeans::~KMeans(){

    }
}
