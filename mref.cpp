#include "mref.h"


mref::mref()
{

}

mref::mref(string path)
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
        if(splitstr.size()==4){
            mstarRef ms;
            ms.id = atoi((splitstr[0]).c_str());
            ms.sCoor.lon = atof((splitstr[1]).c_str())/180*PI;
            ms.sCoor.lat = atof((splitstr[2]).c_str())/180*PI;
            ms.rank = atof((splitstr[3]).c_str());
            this->refs.push_back(ms);
        }
        step++;
    }
    infile.close();
    expandRefs();
}

void mref::expandRefs()
{
    int sz =this->refs.size();
    for(int i=0 ;i< sz/10; i++){
        mstarRef temp;
        temp = this->refs[i];
        temp.sCoor.lon += PI;
        this->refs.push_back(temp);
    }
}

bound mref::getSearchArea(sphereCoor sCoor)
{
    bound bd;
    bd.maxLon = sCoor.lon + 1.7*FOV/2;
    bd.minLon = sCoor.lon - 1.7*FOV/2;
    bd.maxLat = sCoor.lat + 1.8*FOV/2;
    bd.minLat = sCoor.lat - 1.8*FOV/2;
    return bd;
}

void mref::getSearchStars(bound bd, vector<mstarRef> &Srefs)
{
    int start = 0;
    int end = 0;
    for(int i=0 ;i<this->refs.size()-1; i++){
        if(this->refs[i].sCoor.lon < bd.minLon && this->refs[i+1].sCoor.lon > bd.minLon){
            start = i;
        }
        if(this->refs[i].sCoor.lon < bd.maxLon && this->refs[i+1].sCoor.lon > bd.maxLon){
            end = i+1;
        }
    }
    for(int j=start ;j<end; j++){
        if(this->refs[j].sCoor.lat < bd.maxLat && this->refs[j].sCoor.lat > bd.minLat){
            Srefs.push_back(this->refs[j]);
        }
    }

}

void mref::getRefStars(vector<mstarRef> Srefs, sphereCoor sCoor, vector<mstarRef> &refStars)
{
    for(int i=0; i<Srefs.size(); i++){
        double dst = calculate::calDistSphere(sCoor, Srefs[i].sCoor);
        if(dst < FOV/2){
            refStars.push_back(Srefs[i]);
        }
    }
}

void mref::rankRefStars(vector<mstarRef> &refStars)
{
    int sz = refStars.size();
    for(int i=0; i<sz-1; i++)
    {
        for(int j=0; j<sz-1-i; j++)
        {
            if(refStars[j].rank > refStars[j+1].rank)
            {
                mstarRef temp;
                temp = refStars[j];
                refStars[j] = refStars[j+1];
                refStars[j+1] = temp;
            }
        }
    }
}

void mref::getRefStars2D(vector<mstarRef> refStars, sphereCoor sCoor, vector<mstar2D> &refStars2D)
{
    for(int i=0; i<refStars.size(); i++){
        sphereCoorXYZ sC;
        sC = calculate::getSphereCoorXYZ(refStars[i].sCoor);
        sC = calculate::translateSphereCoorXYZ(sC,sCoor);

        mstar2D mS;
        mS.id = refStars[i].id;
        mS.coorXY.x = sC.x / sC.z;
        mS.coorXY.y = sC.y / sC.z;

        refStars2D.push_back(mS);
    }
}


void mref::getRefStars2DFromBL(sphereCoor sCoor, vector<mstar2D> &refStars2D)
{
    bound bd = getSearchArea(sCoor);

    vector<mstarRef> Srefs;
    getSearchStars(bd,Srefs);

    vector<mstarRef> refStars;
    getRefStars(Srefs,sCoor,refStars);

    rankRefStars(refStars);

    getRefStars2D(refStars, sCoor, refStars2D);

}

