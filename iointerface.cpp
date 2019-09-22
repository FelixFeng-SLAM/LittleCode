#include "iointerface.h"
namespace Trajectory {

IOinterface::IOinterface()
{
}

void IOinterface::readFile(string path, vector<Trajectory::FileRecord> &records){
    ifstream infile;
    infile.open(path.c_str());
    string s;
    int step=0;
    while(getline(infile,s))
    {
        Trajectory::FileRecord tempfr = Trajectory::FileRecord(s);
        records.push_back(tempfr);
        step++;
    }
    infile.close();
}

int mkpath(std::string s,mode_t mode=0755)
{
    size_t pre=0,pos;
    std::string dir;
    int mdret;
    if(s[s.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        s+='/';
    }
    while((pos=s.find_first_of('/',pre))!=std::string::npos){
        dir=s.substr(0,pos++);
        pre=pos;
        if(dir.size()==0) continue; // if leading / first time is 0 length
        if((mdret=::mkdir(dir.c_str(),mode)) && errno!=EEXIST){
            return mdret;
        }
    }
    return mdret;
}

int IOinterface::splitFilewithID(string path, bool IDflag[],  int &recordnum){
    char exePath[128];
    getcwd(exePath, sizeof(exePath)-1);
    string tempPath =exePath;
    tempPath = tempPath +"/TempFile";
    mkpath(tempPath,0755);
    ifstream infile;
    infile.open(path.c_str());
    string s;
    int step=0;
    while(getline(infile,s))
    {
        Trajectory::FileRecord tempfr = Trajectory::FileRecord(s);
        if(tempfr.isGood){
           recordnum++;
           IDflag[tempfr.id] = true;

           stringstream ss;
           ss << setfill('0') << setw(5) << tempfr.id;
           string outpath = tempPath+"/"+ss.str()+".txt";
           ofstream outfile;
           outfile.open(outpath, ios::out |ios::app);
           if(outfile.is_open()){
               outfile << s <<endl;
           }
           outfile.close();
        }
        step++;
    }
    infile.close();
    return step;
}

void IOinterface::deleteFilewithID(bool IDflag[]){
    char exePath[128];
    getcwd(exePath, sizeof(exePath)-1);
    string tempPath =exePath;
    tempPath = tempPath +"/TempFile";
    for(int i=0;i<99999;i++){
        if(IDflag[i]){
            stringstream ss;
            ss << setfill('0') << setw(5) << i;
            string outpath = tempPath+"/"+ss.str()+".txt";
            remove(outpath.c_str());
        }
    }
    rmdir(tempPath.c_str());
}

void IOinterface::getFileNames(string path, vector<string>& filename)
{
    struct dirent *ptr;
    DIR *dir;
    dir=opendir(path.c_str());
    while((ptr=readdir(dir))!=NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        filename.push_back(ptr->d_name);
    }
    for(int i=0;i<filename.size()-1;i++)
    {
        for(int j=0;j<filename.size()-i-1;j++)
        {
            if(filename[j]>filename[j+1])
            {
                string temp = filename[j];
                filename[j] = filename[j+1];
                filename[j+1] = temp;
            }
        }
    }
    closedir(dir);
}

time_t IOinterface::inputTime(int year,int month,int day,int hour,int minute,int second){
    time_t t;
    tm temptm;
    temptm.tm_year =year- 1900;
    temptm.tm_mon = month - 1;
    temptm.tm_mday = day;
    temptm.tm_hour = hour;
    temptm.tm_min = minute;
    temptm.tm_sec = second;
    temptm.tm_isdst = 0;
    t = mktime(&temptm);
    return t;
}

void IOinterface::outTrajLenth(string dataPath, string outPath, time_t tstart, time_t tend){
    bool IDflag[99999];
    int recordnum=0;
//    int totalnum=Trajectory::IOinterface::splitFilewithID(dataPath,IDflag,recordnum);

    //    cout << "record number =  " << totalnum<< endl;
    //    cout << "good number =  " << recordnum<< endl;

    char exePath[128];
    getcwd(exePath, sizeof(exePath)-1);
    string tempPath =exePath;
    tempPath = tempPath +"/TempFile";

    vector<string> filenames;
    Trajectory::IOinterface::getFileNames(tempPath,filenames);

    ofstream outfile;
    outfile.open(outPath, ios::out |ios::app);

    for(int i=0;i<filenames.size();i++){
        string mpath = tempPath +"/"+filenames[i];
        vector<Trajectory::FileRecord> frs;
        Trajectory::IOinterface::readFile(mpath,frs);
        Trajectory::Traj traj = Trajectory::Traj(frs);
        traj.sortTrajPoints();
        int errornum=traj.eraseError();
        double trajdist = traj.calTotalDist(tstart,tend);
        if(outfile.is_open()){
            stringstream ss1,ss2;
            ss1<<traj.id ;
            ss2<<trajdist;
            string str =ss1.str() +","+ ss2.str();
            outfile <<str <<endl;
        }
    }

    outfile.close();


//    Trajectory::IOinterface::deleteFilewithID(IDflag);


}

pthread_mutex_t mutex;
void *mythread(void *data)
{
    ioPath *iopath=(ioPath*)data;
    vector<Trajectory::FileRecord> frs;
    for(int k=0;k<iopath->inPath.size();k++){
        Trajectory::IOinterface::readFile(iopath->inPath[k],frs);
        Trajectory::Traj traj = Trajectory::Traj(frs);
        traj.sortTrajPoints();
        int errornum=traj.eraseError();
        double trajdist = traj.calTotalDist(iopath->t_start,iopath->t_end);

        pthread_mutex_lock(&mutex);
        ofstream outfile;
        outfile.open(iopath->outPath, ios::out |ios::app);
        if(outfile.is_open()){
            stringstream ss1,ss2;
            ss1<<traj.id ;
            ss2<<trajdist;
            string str =ss1.str() +","+ ss2.str();
            outfile <<str <<endl;
        }
        outfile.close();
        pthread_mutex_unlock(&mutex);
    }

}


void IOinterface::outTrajLenthUsingThread(string dataPath, string outPath, time_t tstart, time_t tend){
    bool IDflag[99999];
    int recordnum=0;
//    int totalnum=Trajectory::IOinterface::splitFilewithID(dataPath,IDflag,recordnum);

    //    cout << "record number =  " << totalnum<< endl;
    //    cout << "good number =  " << recordnum<< endl;

    char exePath[128];
    getcwd(exePath, sizeof(exePath)-1);
    string tempPath =exePath;
    tempPath = tempPath +"/TempFile";
    vector<string> filenames;
    Trajectory::IOinterface::getFileNames(tempPath,filenames);

    int num_thread = 5;
    pthread_t *pt = (pthread_t *)malloc(sizeof(pthread_t) * num_thread);
    int * id = (int *)malloc(sizeof(int) * num_thread);
    pthread_mutex_init(&mutex, NULL);

    ioPath myioPath[num_thread];
    int interval = filenames.size()/num_thread;
    for(int j1=0;j1<num_thread-1;j1++){
        for(int j2 = j1*interval;j2<(j1+1)*interval;j2++){
            myioPath[j1].inPath.push_back(tempPath +"/"+filenames[j2]);
        }
        myioPath[j1].outPath=outPath;
        myioPath[j1].t_start=tstart;
        myioPath[j1].t_end=tend;
    }
    for(int j2 = (num_thread-1)*interval;j2<filenames.size();j2++){
        myioPath[num_thread-1].inPath.push_back(tempPath +"/"+filenames[j2]);
    }
    myioPath[num_thread-1].outPath=outPath;
    myioPath[num_thread-1].t_start=tstart;
    myioPath[num_thread-1].t_end=tend;

    for(int j=0;j<num_thread;j++){
        id[j] = j;
        int ret=pthread_create(&pt[j],NULL,mythread,(void *)&myioPath[j]);
    }

    for (int i = 0; i < num_thread; i++){
        pthread_join(pt[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(pt);
    free(id);

//    Trajectory::IOinterface::deleteFilewithID(IDflag);


}

}
