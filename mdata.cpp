#include "mdata.h"

mdata::mdata()
{

}

mdata::mdata(string path)
{
    ifstream infile;
    infile.open(path.c_str());
    string s;
    int step=0;
    while(getline(infile,s))
    {
        vector<string> splitstr;
        stringstream sstr(s);
        string tempstr;
        while(getline(sstr, tempstr, ','))
        {
            splitstr.push_back(tempstr);
        }
        if(splitstr.size()==3){
            mstar2D ms;
            ms.id = atoi((splitstr[0]).c_str());
            ms.coorXY.x = atof((splitstr[1]).c_str())-IMAGE_SIZE/2;
            ms.coorXY.y = IMAGE_SIZE/2-atof((splitstr[2]).c_str());

            this->datas.push_back(ms);
        }
        step++;
    }
    infile.close();


}


void mdata::selectDatas(vector<mstar2D> _datas, vector<mstar2D> &Sdatas)
{
    mstar2D ms;
    ms.coorXY.x = 0; ms.coorXY.y = 0; ms.id = 0;
    for(int i=0; i<_datas.size(); i++){
        if(calculate::calDist2D(_datas[i].coorXY,ms.coorXY) < IMAGE_SIZE/2){
            Sdatas.push_back(_datas[i]);
        }
    }
}

void mdata::normData(vector<mstar2D> &Sdatas)
{
    double f = calculate::calf();
    for(int i=0; i<Sdatas.size(); i++){
        Sdatas[i].coorXY.x /= f;
        Sdatas[i].coorXY.y /= -1*f;
    }
}

