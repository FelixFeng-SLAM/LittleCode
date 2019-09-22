#ifndef IOINTERFACE_H
#define IOINTERFACE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>

#include "filerecord.h"
#include "traj.h"

using namespace std;

struct ioPath{
    vector<string> inPath;
    string outPath;
    time_t t_start;
    time_t t_end;
};

namespace Trajectory {

class IOinterface
{
public:
    IOinterface();
    static void readFile(string path, vector<Trajectory::FileRecord> &records);
    static int splitFilewithID(string path,  bool IDflag[], int &recordnum);
    static void deleteFilewithID(bool IDflag[]);
    static void getFileNames(string path, vector<string>& filenames);
    static time_t inputTime(int year,int month,int day,int hour,int minute,int second);
    static void outTrajLenth(string dataPath, string outPath, time_t tstart, time_t tend);

    static void outTrajLenthUsingThread(string dataPath, string outPath, time_t tstart, time_t tend);


};


}

#endif // IOINTERFACE_H
