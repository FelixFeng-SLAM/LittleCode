#ifndef FILERECORD_H
#define FILERECORD_H

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include<unistd.h>
#include<iomanip>


using namespace std;

namespace Trajectory {

class FileRecord
{
public:
    bool            isGood = false;
    time_t         time;
    string          company;
    int                id;
    double        longitude;
    double        latitude;
    double        speed;
    double        azimuth;
    int                status;
    int                flag;
    string           reserved_field;
    time_t          unloading_time;
public:
    FileRecord();
    FileRecord(string _str);


};


}

#endif // FILERECORD_H
