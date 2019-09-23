#include "calculate.h"

calculate::calculate()
{

}


double calculate::calf()
{
    return (IMAGE_SIZE/2)/tan(FOV/2);
}

double calculate::calDist2D(CoorXY c1, CoorXY c2)
{
    return sqrt((c1.x-c2.x)*(c1.x-c2.x)+(c1.y-c2.y)*(c1.y-c2.y));
}

double calculate::calAngle2D(CoorXY c1, CoorXY c2)
{
    double d1, d2, d;
    CoorXY o;
    o.x = 0; o.y = 0;
    d1 = calDist2D(c1,o);
    d2 = calDist2D(c2,o);
    d = c1.x*c2.x+c1.y*c2.y;
    return acos(d/(d1*d2));
}

CoorXY calculate::calCenter(vector<mstar2D> Sdatas)
{
    CoorXY coor;
    coor.x = 0;
    coor.y = 0;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        coor.x += Sdatas[i].coorXY.x;
        coor.y += Sdatas[i].coorXY.y;
    }
    coor.x /= sz;
    coor.y /= sz;
    return coor;
}

double calculate::calCenterDist(CoorXY center)
{
    CoorXY o;
    o.x = 0;
    o.y = 0;
    return calculate::calDist2D(center,o);
}

double calculate::getMaxDist(vector<mstar2D> Sdatas, int &maxid)
{
    double dst = 0;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        CoorXY coo;
        coo.x = 0; coo.y = 0;
        double temp = calDist2D(Sdatas[i].coorXY,coo);
        if(dst < temp){
            dst = temp;
            maxid = Sdatas[i].id;
        }
    }
    return dst;
}

double calculate::getMinDist(vector<mstar2D> Sdatas, int &minid)
{
    double dst = 1e8;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        CoorXY coo;
        coo.x = 0; coo.y = 0;
        double temp = calDist2D(Sdatas[i].coorXY,coo);
        if(dst > temp){
            dst = temp;
            minid = Sdatas[i].id;
        }
    }
    return dst;

}

double calculate::getSecDist(vector<mstar2D> Sdatas, int maxid, int &secid)
{
    double dst = 0;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        if(Sdatas[i].id != maxid){
            CoorXY coo;
            coo.x = 0; coo.y = 0;
            double temp = calDist2D(Sdatas[i].coorXY,coo);
            if(dst < temp){
                dst = temp;
                secid = Sdatas[i].id;
            }
        }
    }
    return dst;

}

double calculate::getAngleOf2id(vector<mstar2D> Sdatas, int maxid, int secid)
{
    mstar2D max, sec;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        if(Sdatas[i].id == maxid){
            max = Sdatas[i];
        }
        if(Sdatas[i].id == maxid){
            sec = Sdatas[i];
        }
    }
    return calAngle2D(max.coorXY,sec.coorXY);
}

void calculate::calDistPD(vector<mstar2D> Sdatas, double &R, int PD[])
{
    for(int i=0; i<10; i++){
        PD[i]=0;
    }
    R = (IMAGE_SIZE/2.0)/calf();
    int sz = Sdatas.size();
    mstar2D o;
    o.coorXY.x=0; o.coorXY.y=0;
    for(int i=0; i<sz; i++){
        double dis = calDist2D(Sdatas[i].coorXY,o.coorXY);
        int pos = (int)(dis/(0.1*R));
        PD[pos]++;
        int p=0;
    }
}

void calculate::calAnglePD(vector<mstar2D> Sdatas, int PD[], int &maxpos)
{
    for(int i=0; i<2; i++){
        PD[i]=0;
    }
    double dst = 0;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        CoorXY coo;
        coo.x = 0; coo.y = 0;
        double temp = calDist2D(Sdatas[i].coorXY,coo);
        if(dst < temp){
            dst = temp;
            maxpos = i;
        }
    }

    for(int i=0; i<sz; i++){
        if(i != maxpos){
            double flag = (Sdatas[maxpos].coorXY.x-Sdatas[i].coorXY.x)*
                    (0-Sdatas[i].coorXY.y)-(Sdatas[maxpos].coorXY.y-Sdatas[i].coorXY.y)*
                    (0-Sdatas[i].coorXY.x);
            if(flag > 0)
            {
                PD[0]++;
            }
            else {
                PD[1]++;
            }
        }
    }

    if(PD[1]>PD[0]){
        int temp = PD[1];
        PD[1]=PD[0];
        PD[0]=temp;
    }

}

void calculate::calAnglePD2(vector<mstar2D> Sdatas, int PD[], int &minpos)
{
    for(int i=0; i<2; i++){
        PD[i]=0;
    }
    double dst = 1e8;
    int sz = Sdatas.size();
    for(int i=0; i<sz; i++){
        CoorXY coo;
        coo.x = 0; coo.y = 0;
        double temp = calDist2D(Sdatas[i].coorXY,coo);
        if(dst > temp){
            dst = temp;
            minpos = i;
        }
    }

    for(int i=0; i<sz; i++){
        if(i != minpos){
            double flag = (Sdatas[minpos].coorXY.x-Sdatas[i].coorXY.x)*
                    (0-Sdatas[i].coorXY.y)-(Sdatas[minpos].coorXY.y-Sdatas[i].coorXY.y)*
                    (0-Sdatas[i].coorXY.x);
            if(flag > 0)
            {
                PD[0]++;
            }
            else {
                PD[1]++;
            }
        }
    }

    if(PD[1]>PD[0]){
        int temp = PD[1];
        PD[1]=PD[0];
        PD[0]=temp;
    }
}

void calculate::calAnglePD3(vector<mstar2D> Sdatas, int maxpos, int PD[])
{
    for(int i=0; i<2; i++){
        PD[i]=0;
    }
    double dst = 0;
    int sz = Sdatas.size();
    int secpos;
    for(int i=0; i<sz; i++){
        if(i != maxpos){
            CoorXY coo;
            coo.x = 0; coo.y = 0;
            double temp = calDist2D(Sdatas[i].coorXY,coo);
            if(dst < temp){
                dst = temp;
                secpos = i;
            }
        }
    }
    for(int i=0; i<sz; i++){
        if(i != secpos){
            double flag = (Sdatas[secpos].coorXY.x-Sdatas[i].coorXY.x)*
                    (0-Sdatas[i].coorXY.y)-(Sdatas[secpos].coorXY.y-Sdatas[i].coorXY.y)*
                    (0-Sdatas[i].coorXY.x);
            if(flag > 0)
            {
                PD[0]++;
            }
            else {
                PD[1]++;
            }
        }
    }

    if(PD[1]>PD[0]){
        int temp = PD[1];
        PD[1]=PD[0];
        PD[0]=temp;
    }



}

mfeature calculate::calFeature(vector<mstar2D> Sdatas)
{

    mfeature mf;
    CoorXY coo = calCenter(Sdatas);
    mf.centerDist = calCenterDist(coo);
    int maxid, secid, minid;
    mf.maxDist = getMaxDist(Sdatas, maxid);
    mf.secDist = getSecDist(Sdatas, maxid, secid);
    mf.angleMaxSec = getAngleOf2id(Sdatas, maxid, secid);
    mf.minDist = getMinDist(Sdatas, minid);
    mf.angleMaxMin = getAngleOf2id(Sdatas, maxid, minid);

    double R;
    calDistPD(Sdatas,R, mf.dist_PD);

    int maxpos, minpos;
    calAnglePD(Sdatas, mf.angle_PD,maxpos);
    calAnglePD2(Sdatas, mf.angle_PD2,minpos);
    calAnglePD3(Sdatas,maxpos, mf.angle_PD3);

    return mf;
}

double calculate::calFeatureDiff(mfeature mf1, mfeature mf2)
{
//    return (mf1.centerDist-mf2.centerDist)*(mf1.centerDist-mf2.centerDist)+
//            (mf1.maxDist-mf2.maxDist)*(mf1.maxDist-mf2.maxDist)+
//            (mf1.secDist-mf2.secDist)*(mf1.secDist-mf2.secDist)+
//            (mf1.angleMaxSec-mf2.angleMaxSec)*(mf1.angleMaxSec-mf2.angleMaxSec)+
//            (mf1.minDist-mf2.minDist)*(mf1.minDist-mf2.minDist)+
//            (mf1.angleMaxMin-mf2.angleMaxMin)*(mf1.angleMaxMin-mf2.angleMaxMin);
    double dist=0;
    for(int i=0; i<10; i++){
        dist += (mf1.dist_PD[i]-mf2.dist_PD[i])*(mf1.dist_PD[i]-mf2.dist_PD[i]);
    }
    for(int i=0; i<2; i++){
        dist += (mf1.angle_PD[i]-mf2.angle_PD[i])*(mf1.angle_PD[i]-mf2.angle_PD[i]);
    }
    for(int i=0; i<2; i++){
        dist += (mf1.angle_PD2[i]-mf2.angle_PD2[i])*(mf1.angle_PD2[i]-mf2.angle_PD2[i]);
    }
    for(int i=0; i<2; i++){
        dist += (mf1.angle_PD3[i]-mf2.angle_PD3[i])*(mf1.angle_PD3[i]-mf2.angle_PD3[i]);
    }
     dist+= (mf1.centerDist-mf2.centerDist)*(mf1.centerDist-mf2.centerDist)+
                (mf1.maxDist-mf2.maxDist)*(mf1.maxDist-mf2.maxDist)+
                (mf1.secDist-mf2.secDist)*(mf1.secDist-mf2.secDist)+
                (mf1.angleMaxSec-mf2.angleMaxSec)*(mf1.angleMaxSec-mf2.angleMaxSec)+
                (mf1.minDist-mf2.minDist)*(mf1.minDist-mf2.minDist)+
                (mf1.angleMaxMin-mf2.angleMaxMin)*(mf1.angleMaxMin-mf2.angleMaxMin);

    return dist;

}


double calculate::calDistSphere(sphereCoor sCoor1, sphereCoor sCoor2)
{
    double dst;
    dst = acos(cos(sCoor1.lat)*cos(sCoor2.lat)*cos(sCoor2.lon-sCoor1.lon)+
               sin(sCoor1.lat)*sin(sCoor2.lat));
    return dst;
}

sphereCoorXYZ calculate::getSphereCoorXYZ(CoorXY coor, double f)
{
    sphereCoorXYZ sCoor;
    double nor = coor.x*coor.x+coor.y*coor.y+f*f;
    sCoor.x = coor.x / nor;
    sCoor.y = coor.y / nor;
    sCoor.z = -1*f / nor;
    return sCoor;
}

sphereCoorXYZ calculate::getSphereCoorXYZ(sphereCoor sCoor)
{
    sphereCoorXYZ sc;
    sc.x = cos(sCoor.lon)*cos(sCoor.lat);
    sc.y = sin(sCoor.lon)*cos(sCoor.lat);
    sc.z = sin(sCoor.lat);
    return sc;
}

sphereCoorXYZ calculate::normSphereCoorXYZ(sphereCoorXYZ coor)
{
    sphereCoorXYZ dst;
    double d = sqrt(coor.x*coor.x+coor.y*coor.y+coor.z*coor.z);
    dst.x = coor.x / d;
    dst.y = coor.y / d;
    dst.z = coor.z / d;

    return dst;
}

sphereCoorXYZ calculate::crossSphereCoorXYZ(sphereCoorXYZ coor1, sphereCoorXYZ coor2)
{
    sphereCoorXYZ dst;
    dst.x = coor1.y*coor2.z-coor2.y*coor1.z;
    dst.y = coor2.x*coor1.z-coor1.x*coor2.z;
    dst.z = coor1.x*coor2.y-coor2.x*coor1.y;
    return dst;
}

sphereCoor calculate::getSphereCoor(sphereCoorXYZ sCoor)
{
    sphereCoor sC;
    sC.lat = asin(sCoor.z);
    sC.lon = asin(sCoor.y/cos(sC.lat));
    return sC;
}

sphereCoorXYZ calculate::translateSphereCoorXYZ(sphereCoorXYZ coor, sphereCoor sCoor)
{
    sphereCoorXYZ sz = getSphereCoorXYZ(sCoor);
    sphereCoorXYZ sx, sy, dst;
    sx.x = sz.z;
    sx.y = sz.z;
    sx.z = -sz.x-sz.y;

    sx = normSphereCoorXYZ(sx);
    sy = crossSphereCoorXYZ(sz,sx);

    dst.x = coor.x*sx.x+coor.y*sx.y+coor.z*sx.z;
    dst.y = coor.x*sy.x+coor.y*sy.y+coor.z*sy.z;
    dst.z = coor.x*sz.x+coor.y*sz.y+coor.z*sz.z;

    return dst;
}
