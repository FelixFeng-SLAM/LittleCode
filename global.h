#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

static double PI = 3.141592654;
static double FOV = 20.0/180*PI;
static double IMAGE_SIZE = 1024;

struct sphereCoor {
    double lat;
    double lon;
};

struct sphereCoorXYZ {
    double x;
    double y;
    double z;
};

struct CoorXY {
    double x;
    double y;
};

struct mstar2D {
    int id;
    CoorXY coorXY;
};

struct mstarSphere {
    int id;
    sphereCoor sCoor;
};


struct mfeature {
    double centerDist;
    double maxDist;
    double secDist;
    double minDist;
    double angleMaxSec;
    double angleMaxMin;
    int dist_PD[5];
    int angle_PD[2];
    int angle_PD2[2];
    int angle_PD3[2];
};






#endif // GLOBAL_H
