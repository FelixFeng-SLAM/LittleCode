#ifndef MIO_H
#define MIO_H

#include "mref.h"
#include "mdata.h"
#include <fstream>
#include <chrono>

class mio
{
public:
    mio();

    static void readRefs(string path, mref &refs);
    static void readDatas(string path, mdata &datas);

    static void matchDatasRefs(mref refs, mdata datas, sphereCoor &sCoor, vector<mstar2D> &refs2D, vector<mstar2D> &Sdatas, double &diff);


};

#endif // MIO_H
