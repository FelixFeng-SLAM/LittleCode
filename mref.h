#ifndef MREF_H
#define MREF_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>

#include "global.h"
#include "calculate.h"

using namespace std;

struct mstarRef{
    int id;
    sphereCoor sCoor;
    double rank;
};

struct bound {
    double maxLon;
    double minLon;
    double maxLat;
    double minLat;
};

class mref
{
public:
    vector<mstarRef> refs;

public:
    mref();
    mref(string path);

    void expandRefs();

    bound getSearchArea(sphereCoor sCoor);
    void getSearchStars(bound bd, vector<mstarRef> &Srefs);
    void getRefStars(vector<mstarRef> Srefs, sphereCoor sCoor, vector<mstarRef> &refStars);
    void rankRefStars(vector<mstarRef> &refStars);

    void getRefStars2D(vector<mstarRef> refStarsRank, sphereCoor sCoor, vector<mstar2D> &refStars2D);



    ///---****
    void getRefStars2DFromBL(sphereCoor sCoor, vector<mstar2D> &refStars2D);




};

#endif // MREF_H
