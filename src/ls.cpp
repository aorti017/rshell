#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <algorithm>
#include <string.h>
#include <deque>
#include <time.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

class Entry{
    string path;
    string name;
    string info;
    public:
    Entry(string p, string n, string i){
        path = p;
        name = n;
        info = i;
    }
    string get_name(){
        return this->name;
    }
    string get_info(){
        return this->info;
    }
    bool operator < (const Entry& e) const{
        string x = this->name;
        string y = e.name;
        string tmpx = this->name;
        string tmpy = this->name;
        //cout << x << " " << y << endl;
        if(x == "."){
            return true;
        }
        else if(y == "."){
            return false;
        }
        if(x.at(0) == '.'){
            x = x.substr(1,x.size());
        }
        if(y.at(0) == '.'){
            y = y.substr(1,y.size());
        }
        unsigned int k = 0;
        for(unsigned int i = 0; i < x.size(); i++){
            char im = tolower(x.at(i));
            char ex = tolower(y.at(k));
            if(im < ex){
                //cout << "T1" << endl;
                return true;

            }
            else if(im > ex){
                if(im > ex && ex == toupper(ex)){
                    //cout << "T2" << endl;
                    return true;
                }
                return false;
            }
            else if(im == ex){
                /*cout << x << " " << y << endl;
                cout << "!" << im << " " << x.at(i) << endl;
                if(im != x.at(i) && x.size() == y.size()){
                    cout << "F1" << endl;
                    return false;
                }
                else if(im != x.at(i)){
                    cout << "T1" << endl;
                    return true;
                }
                //cout << "!" << im << " " << y.at(i) << endl;
                if(ex != y.at(i)){
                    //cout << "F2" << endl;
                    return true;
                }*/
                if(x.size() < y.size()){
                    return true;
                }
                else if(x.size() > y.size()){
                    return false;
                }
                else if(x.size() == y.size()){
                    if(toupper(im) != x.at(i)
                        && toupper(ex) == y.at(i)){
                        return true;
                    }
                    else if(toupper(im) == x.at(i)
                        && toupper(ex) != y.at(i)){
                        return false;
                    }
                    if(tmpx.at(0) == '.'){
                        return false;
                    }
                    else if(tmpy.at(0) == '.'){
                        return true;
                    }
                }
                k++;
                if(k == y.size()){
                    //cout << "T3" << endl;
                    return true;
                }
            }
        }
        //cout << "T4" << endl;
        return true;
    }
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

string getInfo(string path, struct stat buf){
    string ret;
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
        ret.append("l");
    }
    else if(S_ISDIR(buf.st_mode)){
        ret.append("d");
    }
    else if(S_ISREG(buf.st_mode)){
        ret.append("-");
    }
    ret.append(" ");
    int l = buf.st_nlink;
    ret.append(to_string(l));
    ret.append(" ");
    struct passwd *pw = getpwuid(buf.st_uid);
    ret.append(pw->pw_name);
    ret.append(" ");
    struct group *gr = getgrgid(buf.st_gid);
    ret.append(gr->gr_name);
    ret.append(" ");
    int t = buf.st_size;
    ret.append(to_string(t));
    ret.append(" ");
    time_t secs = buf.st_mtime;
    struct tm * ptm;
    ptm = localtime(&secs);
    ret.append(mnth.at(ptm->tm_mon));
    ret.append(" ");
    ret.append(to_string(ptm->tm_mday));
    ret.append(" ");
    ret.append(to_string(ptm->tm_hour));
    ret.append(":");
    ret.append(to_string(ptm->tm_min));
    ret.append(" ");
    return ret;
}

void print_ls(bool flags[], deque<string> paths, string path){
    if(paths.empty()){
        return;
    }
    int cnt = 0;
    struct stat buf;
    string tmp = paths.front();
    string ftmp;
    DIR *dirp = opendir(tmp.c_str());
    dirent *direntp;
    vector<Entry> fileObj;
    if(flags[2]){
        cout << paths.front() << ":" << endl;
    }
    while((direntp = readdir(dirp))){
        ftmp = direntp->d_name;
        string tmpPath = paths.front();
        tmpPath.append("/");
        tmpPath.append(ftmp);
        stat(tmpPath.c_str(), &buf);
        string i;
        if(flags[1] && flags [0]){
            //return string instead of cout
            i = getInfo(tmpPath, buf);
        }
        if(flags[1] && !flags[0] && ftmp.at(0) != '.'){
            //return string instead pf cout
            i = getInfo(tmpPath, buf);
        }
        if(flags[0]){
            Entry e(tmpPath, direntp->d_name, i);
            fileObj.push_back(e);
            cnt++;
        }
        else if(!flags[0] && ftmp.at(0) != '.'){
            Entry e(tmpPath, direntp->d_name, i);
            fileObj.push_back(e);
            cnt++;
        }
        //construct class obj and push into data structure
        if(S_ISDIR(buf.st_mode) && ftmp != "." && ftmp != ".."
            && flags[2]){
            if(tmp != ".git"){
                if(flags[0]){
                    paths.push_back(tmpPath);
                }
                else if(!flags[0] && ftmp.at(0) != '.'){
                    paths.push_back(tmpPath);
                }
            }
        }
    }
    sort(fileObj.begin(), fileObj.end());
    for(unsigned int i = 0; i < fileObj.size(); i++){
        if(flags[1]){
            cout << fileObj.at(i).get_info();
        }
        cout << fileObj.at(i).get_name() << endl;
    }
    sort(begin(paths), end(paths));
    fileObj.clear();
    closedir(dirp);
    paths.pop_front();
    cout << endl;
    print_ls(flags, paths, path);
    return;
}

int main(int argc, char* argv[]){
    unsigned int flagCount = argc - 2;
    //int flagCount = argc;
    bool flags[3];
    flags[0] = false;
    flags[1] = false;
    flags[2] = false;

    bool valid = true;

    vector<string> files;
    deque<string> paths;
    for(unsigned int i = 1; i < flagCount + 2; i++){
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
            if(!multiCheck(tmp, flags)){
                valid = false;
            }
        }
        else{
            files.push_back(argv[i]);
        }
    }
    sort(files.begin(), files.end());
    struct stat buf;
    if(valid && files.size() <=0){
        paths.push_back(".");
        string pth = ".";
        print_ls(flags, paths, pth);
        paths.pop_front();
    }
    else if(valid && files.size() > 0){
        //sort files by name
        for(unsigned int i = 0; i < files.size(); i++){
            string pth;
            if(files.at(i).at(0)  == '.'){
                if(-1 == stat(files.at(i).c_str(), &buf)){
                    perror("stat");
                }
                else{
                    paths.push_back(files.at(i));
                    pth = files.at(i);
                    if(files.size() > 1){
                        cout << pth << ":" << endl;
                    }
                    print_ls(flags, paths, pth);
                    paths.pop_front();
                }
            }
            else if(files.at(i).at(0) == '/'){
                if(-1 == stat(files.at(i).c_str(), &buf)){
                    perror("stat");
                }
                else{
                    paths.push_back(files.at(i));
                    pth = files.at(i);
                    if(files.size() > 1){
                        cout << pth << ":" << endl;
                    }
                    print_ls(flags, paths, pth);
                    paths.pop_front();
                }
            }
            else{
                string x = "./";
                x.append(files.at(i));
                if(-1 == stat(x.c_str(), &buf)){
                    perror("stat");
                }
                else if(S_ISREG(buf.st_mode)){
                    cout << files.at(i) << endl << endl;
                }
                else{
                    paths.push_back("./" + files.at(i));
                    //pth = "./" + files.at(i);
                    if(files.size() > 1){
                        //cout << pth << ":" << endl;
                        cout << files.at(i) << ":" << endl;
                    }
                    print_ls(flags, paths, pth);
                    paths.pop_front();
                }
            }
        }
    }
    else{
        cerr << "No such flag" << endl;
    }
    return 0;
}
