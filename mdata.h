#ifndef MDATA_H
#define MDATA_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>

#include "global.h"
#include "calculate.h"

using namespace std;

class mdata
{
public:
    vector<mstar2D> datas;
public:
    mdata();
    mdata(string path);

    void selectDatas(vector<mstar2D> _datas, vector<mstar2D> &Sdatas);
    void normData(vector<mstar2D> &Sdatas);



};

#endif // MDATA_H
