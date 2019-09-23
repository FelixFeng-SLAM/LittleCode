#include "mio.h"
#include <math.h>

mio::mio()
{

}

void mio::readRefs(string path, mref &refs)
{
    refs = mref(path);
}

void mio::readDatas(string path, mdata &datas)
{
    datas = mdata(path);
}

void mio::matchDatasRefs(mref refs, mdata datas, sphereCoor &sCoor, vector<mstar2D> &refs2D,vector<mstar2D> &Sdatas, double &diff)
{
    datas.selectDatas(datas.datas,Sdatas);
    datas.normData(Sdatas);
    mfeature mfdatas = calculate::calFeature(Sdatas);

    for(int i=0; i<360; i++){
        for(int j=0; j<360; j++){

//            if(i==271 && j== 18) continue;

            sphereCoor sc;
            sc.lon = i/180.0*PI;
            sc.lat = j/180.0*PI;
            vector<mstar2D> refs2Dtemp, refs2Dtemp2;
            refs.getRefStars2DFromBL(sc,refs2Dtemp);

            if(refs2Dtemp.size() < Sdatas.size()) {
                continue;
            }
            else {
                for(int k=0; k<Sdatas.size(); k++){
//                for(int k=0; k<refs2Dtemp.size(); k++){
                    refs2Dtemp2.push_back(refs2Dtemp[k]);
                }
            }

            mfeature mftemp = calculate::calFeature(refs2Dtemp2);

            double difftemp = calculate::calFeatureDiff(mfdatas,mftemp);
            if(diff > difftemp){
                diff = difftemp;
                sCoor = sc;
                vector <mstar2D>().swap(refs2D);
                for(int k=0; k<refs2Dtemp2.size(); k++){
                    refs2D.push_back(refs2Dtemp2[k]);
                }
            }

        }
    }


}

