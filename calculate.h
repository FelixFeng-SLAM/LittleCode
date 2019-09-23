#ifndef CALCULATE_H
#define CALCULATE_H

#include "global.h"

class calculate
{
public:
    calculate();

    static double calf();
    static double calDist2D(CoorXY c1, CoorXY c2);
    static double calAngle2D(CoorXY c1, CoorXY c2);
    static double calDistSphere(sphereCoor sCoor1, sphereCoor sCoor2);

    static sphereCoorXYZ getSphereCoorXYZ(CoorXY coor, double f);
    static sphereCoorXYZ getSphereCoorXYZ(sphereCoor sCoor);

    static sphereCoorXYZ normSphereCoorXYZ(sphereCoorXYZ coor);
    static sphereCoorXYZ crossSphereCoorXYZ(sphereCoorXYZ coor1, sphereCoorXYZ coor2);

    static sphereCoor getSphereCoor(sphereCoorXYZ coor);
//    static void getSphereDatas(vector<mstar2D> Sdatas, vector<mstarSphere> &SdatasSphere);
    static sphereCoorXYZ translateSphereCoorXYZ(sphereCoorXYZ coor, sphereCoor sCoor);




    static CoorXY calCenter(vector<mstar2D> Sdatas);
    static double calCenterDist(CoorXY center);

    static double getMaxDist(vector<mstar2D> Sdatas, int &maxid);
    static double getMinDist(vector<mstar2D> Sdatas, int &minid);
    static double getSecDist(vector<mstar2D> Sdatas, int maxid, int &secid);

    static double getAngleOf2id(vector<mstar2D> Sdatas, int maxid, int secid);


    static void calDistPD(vector<mstar2D> Sdatas, double &R, int PD[]);
    static void calAnglePD(vector<mstar2D> Sdatas, int PD[], int &maxpos);
    static void calAnglePD2(vector<mstar2D> Sdatas, int PD[], int &minpos);
    static void calAnglePD3(vector<mstar2D> Sdatas,  int minpos, int PD[]);


    static mfeature calFeature(vector<mstar2D> Sdatas);

    static double calFeatureDiff(mfeature mf1, mfeature mf2);



};

#endif // CALCULATE_H
