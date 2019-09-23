#include <iostream>

#include "mio.h"

using namespace std;

void test_getSearchStars(mref refs){
    sphereCoor sC;
    sC.lat = 30.0/180*PI;
    sC.lon = 120.0/180*PI;
    bound bd = refs.getSearchArea(sC);

    vector<mstarRef> Srefs;
    refs.getSearchStars(bd,Srefs);
    int i=0;
}

void test_getRefStars(mref refs){
    sphereCoor sC;
    sC.lat = 30.0/180*PI;
    sC.lon = 120.0/180*PI;
    bound bd = refs.getSearchArea(sC);

    vector<mstarRef> Srefs;
    refs.getSearchStars(bd,Srefs);

    vector<mstarRef> refStars;
    refs.getRefStars(Srefs,sC,refStars);

    int i=0;
}


int main()
{
    mref refs;
    mio::readRefs("../sphericalStars/data/ref.txt", refs);
    mdata datas;
    mio::readDatas("../sphericalStars/data/08.txt", datas);

//    test_getSearchStars(refs);
//    test_getRefStars(refs);

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    sphereCoor sCoor;
    vector<mstar2D> refs2D;
    vector<mstar2D> Sdatas;
    double diff = 1e8;
    mio::matchDatasRefs(refs,datas,sCoor,refs2D, Sdatas,diff);

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double> > (t2 - t1);

    cout << "time cost =" << time_used.count()<< " seconds"<< endl;

    for (int i=0; i<refs2D.size(); i++) {
        cout << refs2D[i].id << "," << refs2D[i].coorXY.x << "," << refs2D[i].coorXY.y << endl;
    }
    cout << endl;
    cout <<sCoor.lon<<","<<sCoor.lat<< endl;
    cout << endl;
    cout << diff << endl;
    cout << endl;

    for (int i=0; i<Sdatas.size(); i++) {
        cout << Sdatas[i].id << "," << Sdatas[i].coorXY.x << "," << Sdatas[i].coorXY.y << endl;
    }

    return 0;
}
