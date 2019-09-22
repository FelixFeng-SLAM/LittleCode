#ifndef TRAJ_H
#define TRAJ_H

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <list>
#include <math.h>

#include "filerecord.h"

using namespace std;

const double ra = 6378137;
const double pi = 3.14159265358979;
const double e2 = 0.00669437999013;

namespace Trajectory {
struct TrajPoint{
    time_t         time;
    double        longitude;
    double        latitude;
    double        speed;
    double        azimuth;
    int                status;
};

class Traj
{
public:
    int                         id;
    string                    company;
    list<TrajPoint>     trajPoints;
public:
    Traj();
    Traj(vector<Trajectory::FileRecord> frs);
    void BL2xy(double lon, double lat, double &x, double &y);
    double calDist(TrajPoint tp1, TrajPoint tp2);
    double calVel(TrajPoint tp1, TrajPoint tp2);
    int eraseError();
    void sortTrajPoints();
    double calTotalDist(time_t tstart, time_t tend);


};


}


#endif // TRAJ_H
