//
// Created by Matthw Michaelis on 3/14/2016.
//
#include "KMeans.h"
#include "Exceptions.h"
#include <limits>

namespace Clustering{
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter){
        if(filename == ""){
            throw DataFileOpenEx(filename);
        }
        __iFileName = filename;
        if(k == 0){
            throw ZeroClustersEx();
        }
        if(dim == 0){
            throw ZeroDimensionsEx();
        }
        __numNonempty = 0;
        __numIter = 0;
        __k = k;
        __dimensionality = dim;
        __maxIter = maxIter;
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

        data >> (*__clusters)[0];
        data.close();

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
        return *__clusters[u];
    }
    const Cluster &KMeans::operator[](unsigned int u) const{
        return *__clusters[u];
    }

// write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans){
        for(int i = 0; i < kmeans.__k; i++){
            os << kmeans[i];
        }
        return os;
    }

// clustering functions
    void KMeans::run() {
        for (int i = 0; i < __maxIter; i++) {
            __numMovesLastIter = 0;
            __numNonempty = 0;
            for (int i = 0; i < __k; i++) {
                if (__clusters[i]->getSize() > 0) {
                    __numNonempty++;
                }
            }
            int closest;

            for (int j = 0; j < __k; j++) {  // For every cluster
                if(__clusters[j]->getSize() > 0){
                    for (int l = 0; l < (*__clusters[j]).getSize(); l++) { // For each point per cluster
                        closest = j;
                        for(int m = 0; m < __k; m++){
                            if (((*__clusters[j])[l].distanceTo(__clusters[closest]->centroid.get()) > (*__clusters[j])[l].distanceTo(__clusters[m]->centroid.get()))) {
                                closest = m;
                            }
                        }
                        if(closest != j){
                            Clustering::Cluster::Move *temp = new Clustering::Cluster::Move((*__clusters[j])[l],(*__clusters[j]),(*__clusters[closest]));
                            __numMovesLastIter++;
                            temp->perform();
                            delete temp;
                        }
                    }
                    if (!((*__clusters[j]).centroid.isValid())) {
                        (*__clusters[j]).centroid.compute();
                    }
                }
            }
        __numIter++;
        }
    }

    KMeans::~KMeans(){
        for(int i = __k-1; i > -1; --i){
            delete (__clusters[i]);
            delete (__initCentroids[i]);
        }

        delete [] __clusters;
        delete [] __initCentroids;

    }
}
