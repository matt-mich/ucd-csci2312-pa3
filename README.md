## CSCI 2312: Programming Assignment 3

_exceptions, inner classes, k-means clustering_

* * *

### README

    FEATURES FROM PA2:
        This program has the ability to create clusters of multidimensional points, preform basic arithmetic on
    those points, sort the points within the cluster from least to greatest, preform comparisons on both the
    points and the clusters, find the distance between points, and stream both clusters and points in from a
    stream, and out to a stream.

    NEW FEATURES FOR PA3:
        The program now utilizes the KMeans algorithm to group points into clusters, each with it's own
    centroid which is recalculated every iteration.

    Most tests are passed consistently.

        Created by Matthew Michaelis, based on the Programming Assignment 3 source files
    in C++ using CLion 1.2.4, cmake 3.3.2, GCC 4.9.3, and Cygwin 2.4.1 on Windows 10.
    Had problems being compiled directly from Cygwin having to do with stoi, but I believe
    that is a documented glitch while using certain non-native GNU terminals on Windows as it
    compiles fine directly from CLion.