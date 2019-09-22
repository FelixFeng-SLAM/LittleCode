#include "filerecord.h"

namespace Trajectory {

FileRecord::FileRecord(){}
FileRecord::FileRecord(string _str){
    vector<string> splitstr;
    stringstream sstr(_str);
    string tempstr;
    while(getline(sstr, tempstr, ','))
    {
        splitstr.push_back(tempstr);
    }
    if(splitstr.size()==12){
        string sss = splitstr[2];
        if(sss=="TR"&& atoi(splitstr[3].c_str()) <99999 && atoi(splitstr[3].c_str())>=0){
            this->isGood = true;
            tm temptm;
            temptm.tm_year =atoi((splitstr[0].substr(0,4)).c_str()) - 1900;
            temptm.tm_mon = atoi((splitstr[0].substr(4,2)).c_str()) - 1;
            temptm.tm_mday = atoi((splitstr[0].substr(6,2)).c_str());
            temptm.tm_hour = atoi((splitstr[1].substr(0,2)).c_str());
            temptm.tm_min = atoi((splitstr[1].substr(2,2)).c_str());
            temptm.tm_sec = atoi((splitstr[1].substr(4,2)).c_str());
            temptm.tm_isdst = 0;
            this->time = mktime(&temptm);
            this->company = splitstr[2];
            this->id = atoi(splitstr[3].c_str());
            this->longitude = atof(splitstr[4].c_str());
            this->latitude = atof(splitstr[5].c_str());
            this->speed = atof(splitstr[6].c_str());
            this->azimuth = atof(splitstr[7].c_str());
            this->status = atoi(splitstr[8].c_str());
            this->flag = atoi(splitstr[9].c_str());
            this->reserved_field= splitstr[10];
            tm temptm2;
            temptm2.tm_year =atoi((splitstr[11].substr(0,4)).c_str()) - 1900;
            temptm2.tm_mon = atoi((splitstr[11].substr(5,2)).c_str()) - 1;
            temptm2.tm_mday = atoi((splitstr[11].substr(8,2)).c_str());
            temptm2.tm_hour = atoi((splitstr[11].substr(11,2)).c_str());
            temptm2.tm_min = atoi((splitstr[11].substr(14,2)).c_str());
            temptm2.tm_sec = atoi((splitstr[11].substr(17,2)).c_str());
            temptm2.tm_isdst = 0;
            this->unloading_time= mktime(&temptm2);
        }
    }
}




}
