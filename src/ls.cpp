#include <iostream>
#include <iomanip>
#include <unordered_map>
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

int total_block_size = 0;

int getBlk(unordered_map<string, string> m, vector<string> v){
    int blocks = 0;
    struct stat buf;
    string path;
    for(unsigned int i = 0; i < v.size(); i++){
        path = m[v.at(i)];
        if(-1 == stat(path.c_str(), &buf)){
            perror("stat");
            exit(0);
        }
        blocks += buf.st_blocks;
    }
    return blocks;
}

bool isDir(string path){
    struct stat buf;
    if(-1 == stat(path.c_str(), &buf)){
        perror("stat");
    }
    return S_ISDIR(buf.st_mode);
}

bool isEx(string path){
    struct stat buf;
    if(-1 == stat(path.c_str(), &buf)){
        perror("stat");
    }
    return buf.st_mode & S_IXUSR;
}

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

string getInfo(string path){
    struct stat buf;
    if(-1 == stat(path.c_str(), &buf)){
        perror("stat");
        exit(0);
    }
    string ret;
    //total_block_size += buf.st_blocks;
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
    if(S_IRUSR & buf.st_mode){
        ret.append("r");
    }
    else{
        ret.append("-");
    }
    if(S_IWUSR & buf.st_mode){
        ret.append("w");
    }
    else{
        ret.append("-");
    }
    if(S_IXUSR & buf.st_mode){
        ret.append("x");
    }
    else{
        ret.append("-");
    }
    if(S_IRGRP & buf.st_mode){
        ret.append("r");
    }
    else{
        ret.append("-");
    }
    if(S_IWGRP & buf.st_mode){
        ret.append("w");
    }
    else{
        ret.append("-");
    }
    if(S_IXGRP & buf.st_mode){
        ret.append("x");
    }
    else{
        ret.append("-");
    }
    if(S_IROTH & buf.st_mode){
        ret.append("r");
    }
    else{
        ret.append("-");
    }
    if(S_IWOTH & buf.st_mode){
        ret.append("w");
    }
    else{
        ret.append("-");
    }
    if(S_IXOTH & buf.st_mode){
        ret.append("x");
    }
    else{
        ret.append("-");
    }
    ret.append(" ");
    int l = buf.st_nlink;
    ret.append(to_string(static_cast<long long>(l)));
    ret.append(" ");
    struct passwd *pw = getpwuid(buf.st_uid);
    ret.append(pw->pw_name);
    ret.append(" ");
    struct group *gr = getgrgid(buf.st_gid);
    ret.append(gr->gr_name);
    ret.append(" ");
    int t = buf.st_size;
    ret.append(to_string(static_cast<long long>(t)));
    ret.append(" ");
    time_t secs = buf.st_mtime;
    struct tm * ptm;
    ptm = localtime(&secs);
    ret.append(mnth.at(ptm->tm_mon));
    ret.append(" ");
    ret.append(to_string(static_cast<long long>(ptm->tm_mday)));
    ret.append(" ");
    ret.append(to_string(static_cast<long long>(ptm->tm_hour)));
    ret.append(":");
    if(ptm->tm_min < 10){
        ret.append("0");
    }
    ret.append(to_string(static_cast<long long>(ptm->tm_min)));
    ret.append(" ");
    return ret;
}
void print_ls(bool flags[], deque<string> paths, string mainPath){
    if(paths.empty()){
        return;
    }
    wcout.imbue(locale(""));
    int cnt = 0;
    struct stat buf;
    string tmp = mainPath;
    string ftmp;
    DIR *dirp = opendir(tmp.c_str());
    if(dirp == NULL){
        perror("opendir");
        exit(0);
    }
    dirent *direntp;
    vector<string> fileObj;
    unordered_map<string, string> filehash;
    if(flags[2]){
        cout << tmp  << ":" << endl;
    }
    while((direntp = readdir(dirp))){
        if(direntp == NULL){
            perror("readdir");
            exit(0);
        }
        ftmp = direntp->d_name;
        string tmpPath = mainPath;
        tmpPath.append("/");
        tmpPath.append(ftmp);
        if(stat(tmpPath.c_str(), &buf) == -1){
            perror("stat");
            exit(0);
        }
        if(flags[0]){
            fileObj.push_back(direntp->d_name);
            filehash[direntp->d_name] = tmpPath;
            cnt++;
        }
        else if(!flags[0] && ftmp.at(0) != '.'){
            fileObj.push_back(direntp->d_name);
            filehash[direntp->d_name] = tmpPath;
            cnt++;
        }
        if(S_ISDIR(buf.st_mode) && ftmp != "." && ftmp != ".."
            && flags[2]){
            if(flags[0]){
                paths.push_back(tmpPath);
            }
            else if(!flags[0] && ftmp.at(0) != '.'){
                paths.push_back(tmpPath);
            }
        }
    }
    total_block_size += getBlk(filehash, fileObj);
    if(flags[1]){
        cout << "total " << total_block_size / 2 << endl;
    }
    total_block_size = 0;
    sort(fileObj.begin(), fileObj.end(), locale("en_US.UTF-8"));
    unsigned int count = 0;
    unsigned int c_count = 0;
        for(unsigned int i = 0; i < fileObj.size(); i++){
            if(flags[1]){
                //total_block_size += getBlk(filehash, fileObj);
                cout << getInfo(filehash[fileObj.at(i)]);
                if(isDir(filehash[fileObj.at(i)])){
                    cout << "\033[34m" << fileObj.at(i)
                        << "\033[0m" << endl;
                }
                else if(isEx(filehash[fileObj.at(i)])){
                    cout << "\033[32m" << fileObj.at(i)
                        << "\033[0m" << endl;
                }
                else{
                    cout << fileObj.at(i) << endl;
                }
                c_count++;
            }
            if(!flags[1]){
                //total_block_size += getBlk(filehash, fileObj);
                if(isDir(filehash[fileObj.at(i)])){
                    unsigned int tmp = fileObj.at(i).size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << "\033[34m" << fileObj.at(i)
                        <<"\033[0m";
                    cout << "  ";
                }
                else if(isEx(filehash[fileObj.at(i)])){
                    unsigned int tmp = fileObj.at(i).size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << "\033[32m" << fileObj.at(i)
                        << "\033[0m";
                    cout << "  ";
                }
                else{
                    unsigned int tmp = fileObj.at(i).size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << fileObj.at(i);
                    cout << "  ";
                }
                c_count++;
            }
        }
        paths.pop_front();
        if((paths.empty() || flags[2]) && !flags[1] && count > 0){
            cout << /* "a" <<*/ endl;
        }
        if(!paths.empty()){
            cout << /* "b" <<*/ endl;
        }
        count = 0;
    sort(paths.begin(), paths.end(), locale("en_US.UTF-8"));
    fileObj.clear();
    closedir(dirp);
    if(paths.empty()){
        return;
    }
    //total_block_size = 0;
    deque<string> recurPaths;
    recurPaths.push_back(paths.front());
    print_ls(flags, recurPaths, paths.front());
    paths.pop_front();
    if(paths.empty()){
        return;
    }
    else{
        cout << endl;
    }
    print_ls(flags, paths, paths.front());
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
    sort(files.begin(), files.end(), locale("en_US.UTF-8"));
    struct stat buf;
    if(valid && files.size() <=0){
        paths.push_back(".");
        string pth = ".";
        print_ls(flags, paths, pth);
        paths.pop_front();
    }
    else if(valid && files.size() > 0){
        for(unsigned int i = 0; i < files.size(); i++){
            string pth;
            if(files.at(i).at(0)  == '.'){
                string tmp = files.at(i);
                if(tmp.at(tmp.size()-1) == '/'){
                    files.at(i) = tmp.substr(0, tmp.size()-1);
                }
                if(-1 == stat(files.at(i).c_str(), &buf)){
                    perror("stat");
                    exit(0);
                }
                else if(S_ISREG(buf.st_mode)){
                    cout << files.at(i) << endl;
                }
                else{
                    paths.push_back(files.at(i));
                    pth = files.at(i);
                    if(files.size() > 1 && !flags[2]){
                        cout << pth << ":" << endl;
                    }
                    print_ls(flags, paths, pth);
                    paths.pop_front();
                }
            }
            else if(files.at(i).at(0) == '/'){
                string tmp = files.at(i);
                if(tmp.at(tmp.size()-1) == '/'){
                    files.at(i) = tmp.substr(0, tmp.size()-1);
                }

                if(-1 == stat(files.at(i).c_str(), &buf)){
                    perror("stat");
                    exit(0);
                }
                else if(S_ISREG(buf.st_mode)){
                    cout << files.at(i) << endl;
                }
                else{
                    paths.push_back(files.at(i));
                    pth = files.at(i);

                    if(files.size() > 1 && !flags[2]){
                        cout << pth << ":" << endl;
                    }
                    print_ls(flags, paths, pth);
                    paths.pop_front();
                }
            }
            else{
                string x = "./";
                x.append(files.at(i));
                string tmp = files.at(i);
                if(tmp.at(tmp.size()-1) == '/'){
                    files.at(i) = tmp.substr(0, tmp.size()-1);
                }

                if(-1 == stat(x.c_str(), &buf)){
                    perror("stat");
                    exit(0);
                }
                else if(S_ISREG(buf.st_mode)){
                    cout << files.at(i) << endl;
                }
                else{
                    paths.push_back("./"+files.at(i));
                    pth = "./" + files.at(i);
                    if(files.size() > 1 && !flags[2]){
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
