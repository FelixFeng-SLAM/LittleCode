#include "traj.h"

namespace Trajectory {

Traj::Traj(){}
Traj::Traj(vector<Trajectory::FileRecord> frs){
    this->id = frs[0].id;
    this->company = frs[0].company;
    for(int i=0;i<frs.size();i++){
        Trajectory::TrajPoint trajp;
        trajp.time = frs[i].time;
        trajp.longitude = frs[i].longitude;
        trajp.latitude = frs[i].latitude;
        trajp.speed = frs[i].speed;
        trajp.azimuth = frs[i].azimuth;
        trajp.status = frs[i].status;
        this->trajPoints.push_back(trajp);
    }
}

void df(double b0, double l0, double b, double l, double &s, double &a){
    double e12, db, dl, bm, ym, tm, vm, nm, s1, s2;
    e12 = e2 / (1 - e2);
    db = b - b0;
    dl = l - l0;
    bm = (b + b0) / 2;
    ym = sqrt(e12 * pow((cos(bm)), 2));
    tm = tan(bm);
    vm = sqrt(1 + pow(ym, 2));
    nm = ra / sqrt(1 - e2 * pow((sin(bm)), 2));
    s1 = nm / pow(vm, 2) * db - nm * pow((cos(bm)), 2) * (2 + 3 * pow(tm, 2) - 3 * pow(tm, 2) * pow(ym, 2)) * db * dl * dl / 24 - nm * (pow(tm, 2) * pow(ym, 2) - pow(ym, 2)) * pow(db, 3) / 8;
    s2 = nm * (cos(bm)) * dl + nm * (cos(bm)) * (1 - pow(ym, 2) - 9 * pow(tm, 2) * pow(ym, 2)) * db * db * dl / 24 - nm * (cos(bm)) * pow((sin(bm)), 2) * pow(dl, 3) / 24;
    s = sqrt(pow(s1, 2) + pow(s2, 2));
    double am;
    if (s1 < 0)
        am = pi + atan(s2 / s1);
    else
    {
        if (s1 > 0)
            am = atan(s2 / s1);
        else
            am = pi / 2;
    }
    a = tm / nm * s * sin(am) * (1 + s * s / 24 / nm / nm * (pow((sin(am)), 2) * (2 + pow(tm, 2) + 2 * pow(ym, 2)) + pow((cos(am)), 2) * (2 + 7 * pow(ym, 2) + 9 * pow(tm, 2) * pow(ym, 2) + 5 * pow(ym, 4))));
    a = am - 0.5 * a;
}

void GuassZ(double l0, double b, double l, double dN, double dE, double &CoorX, double &CoorY)
{
    double b0, dl, t, b1, y2, dx, dy, sx, a;
    double n, bl;
    b0 = 0;
    df(b0, l0, b, l0, sx, a);
    dl = l - l0;
    t = tan(b);
    n = ra / sqrt(1 - e2 * sin(b) * sin(b));
    bl = cos(b) * dl;
    y2 = e2 / (1 - e2) * cos(b) * cos(b);
    dx = 0.5 + bl * bl * (5 - t * t + 9 * y2 + 4 * y2 * y2) / 24 + bl * bl * bl * bl * (61 - 58 * t * t + t * t * t * t) / 720;
    double xx = sx + n * t * cos(b) * cos(b) * dl * dl * dx;
    dy = 1 + bl * bl * (1 - t * t + y2) / 6 + bl * bl * bl * bl * (5 - 18 * t * t + t * t * t * t + 14 * y2 - 58 * t * t * y2) / 120;
    double yy = n * cos(b) * dl * dy;
    CoorX = yy + dE;
    CoorY = xx + dN;
}

void Traj::BL2xy(double lon, double lat, double &x, double &y){
    double hf = 180 / 3.14159265 * 3600;
    double l = (lat * 3600) / hf;
    double b = (lon * 3600) / hf;
    double l0 = 121.5 * 3600 / hf;
    GuassZ(l0, l, b, -3400000.00, 100000,x,y);
}


double Traj::calDist(TrajPoint tp1, TrajPoint tp2){
    double x1,x2,y1,y2,dist;
    Trajectory::Traj::BL2xy(tp1.longitude,tp1.latitude,x1,y1);
    Trajectory::Traj::BL2xy(tp2.longitude,tp2.latitude,x2,y2);
    dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return dist;
}

double Traj::calVel(TrajPoint tp1, TrajPoint tp2){
    double delta = difftime(tp2.time,tp1.time);
    if(delta==0){
        return 0;
    }
    else{  
        double vel = Traj::calDist(tp1, tp2)/delta;
        return vel;
    }
}

bool compareTime(const TrajPoint &tp1, const TrajPoint &tp2){
    if(difftime(tp2.time,tp1.time)>0){
        return true;
    }
    else{
        return false;
    }
}


int Traj::eraseError(){
    int dst=0,step=0;
    TrajPoint tp;
    for (list<Trajectory::TrajPoint>::iterator it = this->trajPoints.begin(); it != this->trajPoints.end(); ++it){
        step++;
        if(it!=this->trajPoints.begin()){
            double vel = Traj::calVel(tp,*it);
            if(vel>100/3.6){
                it=this->trajPoints.erase(it);
                dst++;
            }
        }
        tp=*it;
    }
    return dst;
}

void Traj::sortTrajPoints(){
    this->trajPoints.sort(compareTime);
}

bool compareTime2(TrajPoint tp, time_t t){
    if(difftime(tp.time,t)>0){
        return true;
    }
    else{
        return false;
    }
}

double Traj::calTotalDist(time_t tstart, time_t tend){
    double dst=0;
    TrajPoint tp;
    for (list<Trajectory::TrajPoint>::iterator it = this->trajPoints.begin(); it != this->trajPoints.end(); ++it){
        if(it!=this->trajPoints.begin()){
            if(compareTime2(*it,tstart) && !(compareTime2(*it,tend))){
                double d=Traj::calDist(tp,*it);
                dst+=d;
            }
        }
        tp=*it;
    }
    return dst;
}



}

