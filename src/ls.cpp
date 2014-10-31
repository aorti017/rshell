#include <iostream>
#include <string.h>
#include <queue>
#include <time.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

class Entry{
    string path;
    string name;
    string info;
};


bool multiCheck(string tmp, bool flags[]){
    string chk = "alR";
    for(unsigned int i = 1; i < tmp.size();  i++){
        if(chk.find(tmp.at(i)) < 0 ||
            chk.find(tmp.at(i)) >= tmp.size()){
            return false;
        }
        else if(tmp.at(i) == 'a'){
            flags[0] = true;
        }
        else if(tmp.at(i) == 'l'){
            flags[1] = true;
        }
        else if(tmp.at(i) == 'R' ){
            flags[2] = true;
        }
    }
    return true;
}

void getInfo(string path, struct stat buf){
    vector<string> mnth;
    mnth.push_back("Jan");
    mnth.push_back("Feb");
    mnth.push_back("Mar");
    mnth.push_back("Apr");
    mnth.push_back("May");
    mnth.push_back("Jun");
    mnth.push_back("Jul");
    mnth.push_back("Aug");
    mnth.push_back("Sep");
    mnth.push_back("Oct");
    mnth.push_back("Nov");
    mnth.push_back("Dec");
    if(S_ISLNK(buf.st_mode)){
        cout << "l";
    }
    else if(S_ISDIR(buf.st_mode)){
        cout << "d";
    }
    else if(S_ISREG(buf.st_mode)){
        cout << "-";
    }
    cout << " ";
    time_t secs = buf.st_mtime;
    struct tm * ptm;
    ptm = localtime(&secs);
    cout << mnth.at(ptm->tm_mon);
    cout << " ";
    cout << ptm->tm_mday;
    cout << " ";
    cout << ptm->tm_hour;
    cout << ":";
    cout << ptm->tm_min;
    cout << " ";
}

void print_ls(bool flags[], queue<string> paths, string path){
    if(paths.empty()){
        return;
    }
    int cnt = 0;
    struct stat buf;
    string tmp = paths.front();
    string ftmp;
    DIR *dirp = opendir(tmp.c_str());
    dirent *direntp;

    if(flags[2]){
        cout << paths.front() << ":" << endl;
    }
    while((direntp = readdir(dirp))){
        ftmp = direntp->d_name;
        string tmpPath = paths.front();
        tmpPath.append("/");
        tmpPath.append(ftmp);
        stat(tmpPath.c_str(), &buf);
        if(flags[1] && flags [0]){
            //get and cout info
            getInfo(tmpPath, buf);
        }
        if(flags[1] && !flags[0] && ftmp.at(0) != '.'){
            ////get and cout info
            getInfo(tmpPath, buf);
        }
        if(flags[0]){
            cout << direntp->d_name << endl;
            cnt++;
        }
        else if(!flags[0] && ftmp.at(0) != '.'){
            cout << direntp->d_name << endl;
            cnt++;
        }
        if(S_ISDIR(buf.st_mode) && ftmp != "." && ftmp != ".."
            && flags[2]){
            if(tmp != ".git"){
                if(flags[0]){
                    paths.push(tmpPath);
                }
                else if(!flags[0] && ftmp.at(0) != '.'){
                    paths.push(tmpPath);
                }
            }
        }
    }
    cout << endl;
    closedir(dirp);
    paths.pop();
    cout << endl;
    print_ls(flags, paths, path);
    return;
}

int main(int argc, char* argv[]){
    int flagCount = argc - 2;
    bool flags[3];
    flags[0] = false;
    flags[1] = false;
    flags[2] = false;

    vector<string> files;
    queue<string> paths;
    for(unsigned int i = 2; i < flagCount + 2; i++){
        string tmp = argv[i];
        if(tmp == "-a"){
            flags[0] = true;
        }
        else if(tmp == "-l"){
            flags[1] = true;
        }
        else if(tmp == "-R"){
            flags[2] = true;
        }
        else if(argv[i][0] == '-'){
            multiCheck(tmp, flags);
        }
        else{
            files.push_back(argv[i]);
        }
    }
   // if(files.size() <= 0){
        paths.push(".");
        string pth = ".";
   // }
    print_ls(flags, paths, pth);

/*    cout << flags[0] << " " << flags[1] << " "  << flags[2] << endl;
    for(unsigned int i = 0; i < files.size(); i++){
        cout << files.at(i) << endl;
    }*/
    return 0;
}
