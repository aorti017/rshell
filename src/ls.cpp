#include <iostream>
#include <iomanip>
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

class Entry{
    string path;
    string name;
    string info;
    bool dir;
    bool hidden;
    bool ex;
    public:
    bool isDir(){
        return dir;
    }
    bool isEx(){
        return ex;
    }
    Entry(string p, string n, string i){
        path = p;
        name = n;
        info = i;
        dir = false;
        hidden = false;
        ex = false;
    }
    Entry(string p, string n){
        path = p;
        name = n;
        info = "";
        dir = false;
        hidden = false;
        ex = false;
    }
    Entry(string p, string n, string i, bool d, bool h, bool e){
        path = p;
        name = n;
        info = i;
        dir = d;
        hidden = h;
        ex = e;
    }
    Entry(string p, string n, bool d, bool h, bool e){
        path = p;
        name = n;
        info = "";
        dir = d;
        hidden = h;
        ex = e;
    }
    string get_name(){
        return this->name;
    }
    string get_path(){
        return this->path;
    }
    string get_info(){
        return this->info;
    }
    bool operator < (const Entry& e) const{
        string x = this->name;
        string y = e.name;
        string tmpx = this->name;
        string tmpy = this->name;
        if(x == "."){
            return true;
        }
        else if(y == "."){
            return false;
        }
        if(x == ".." && y != "."){
            return true;
        }
        else if(x != "." && y == ".."){
            return false;
        }
        if(x.at(0) == '.'){
            x = x.substr(1,x.size());
        }
        if(y.at(0) == '.'){
            y = y.substr(1,y.size());
        }
        string x_lower = x;
        string y_lower = y;
        transform(x_lower.begin(), x_lower.end(), x_lower.begin(), ::tolower);
        transform(y_lower.begin(), y_lower.end(), y_lower.begin(), ::tolower);
        if(x_lower == y_lower){
            unsigned int k = 0;
            for(unsigned int i = 0; i < x.size(); i++){
                char im = tolower(x.at(i));
                char ex = tolower(y.at(k));
                if(x.at(i) == '.' && i + 1 < x.size()){
                    i++;
                    im = tolower(x.at(i));
                }
                if(y.at(k) == '.' && k + 1 < y.size()){
                    k++;
                    ex = tolower(y.at(k));
                }
                if(toupper(im) != x.at(i)
                    && toupper(ex) == y.at(k)){
                    return true;
                }
                else if(toupper(ex) != y.at(k)
                    && toupper(im) == x.at(i)){
                    return false;
                }
                k++;
                if(k == y.size()){
                    if(tmpx.at(0) == '.'){
                        return true;
                    }
                    else if(tmpy.at(0) == '.'){
                        return false;
                    }
                }
            }
        }
        //unsigned int x_size = x.size();
        //unsigned int y_size = y.size();
        unsigned int k = 0;
        for(unsigned int i = 0; i < x.size(); i++){
            char imtmp = x.at(i);
            char extmp = y.at(k);
            char im = tolower(imtmp);
            char ex = tolower(extmp);
            if(x.at(i) == '.' && i + 1 < x.size()){
                i++;
                imtmp = x.at(i);
                im = tolower(x.at(i));
                //x_size--;
            }
            //else if(i + 1 >=  x.size() && x.at(i) == '.'){
                //return true;
            //}
            if(y.at(k) == '.' && k + 1 < y.size()){
                k++;
                extmp = y.at(k);
                ex = tolower(y.at(k));
                //y_size--;
            }
            //else if(k + 1 >=  y.size() && y.at(k) == '.'){
                //return true;
            //}
            if(!isalnum(imtmp) && isalnum(extmp)){
                return true;
            }
            else if(!isalnum(extmp) && isalnum(imtmp)){
                return false;
            }
            else if(!isalnum(imtmp) && !isalnum(extmp)){
                return imtmp < extmp;
            }
            if(!isdigit(imtmp) && isdigit(extmp)){
                return false;
            }
            else if(!isdigit(extmp) && isdigit(imtmp)){
                return true;
            }
            if(im < ex){
                return true;

            }
            else if(im > ex){
                return false;
            }
            /*else if(im == ex){
                if(toupper(im) != x.at(i)
                        && toupper(ex) == y.at(k)){
                    if(x_size >  y_size){
                        return false;
                    }
                    else{
                        return true;
                    }
                }
                else if(toupper(im) == x.at(i) && toupper(ex) != y.at(k)){
                    if(x_size >=  y_size){
                        return false;
                    }
                    else{
                        return true;
                    }
                }
            }*/
            k++;
            if(k >= y.size() && i+1 < x.size()){
                return false;
            }
        }
        if(tmpx.at(0) == '.' && tmpy.at(0) != '.'){
            return true;
        }
        else if(tmpx.at(0) != '.' && tmpy.at(0) == '.'){
            return false;
        }
        else if(tmpx.at(0) == '.' && tmpy.at(0) == '.'){
            if(x.size() < y.size()){
                return true;
            }
            else{
                return false;
            }
        }
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
    total_block_size += buf.st_blocks;
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
int getTotalChars(vector<Entry> v){
    int n = 0;
    for(unsigned int i = 0; i < v.size(); i++){
        n += 2;
        n += v.at(i).get_name().size();
    }
    return n;
}
void print_ls(bool flags[], deque<Entry> paths, string mainPath){
    if(paths.empty()){
        return;
    }
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
    vector<Entry> fileObj;
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
        string i;
        if(flags[1] && flags [0]){
            i = getInfo(tmpPath, buf);
        }
        if(flags[1] && !flags[0] && ftmp.at(0) != '.'){
            i = getInfo(tmpPath, buf);
        }
        if(flags[0]){
            bool dir;
            if(S_ISDIR(buf.st_mode)){
                dir = true;
            }
            else{
                dir = false;
            }
            bool ex;
            if((S_IEXEC & buf.st_mode)){
                ex = true;
            }
            else{
                ex = false;
            }
            bool hide;
            if(ftmp.at(0) == '.'){
                hide = true;
            }
            else{
                hide = false;
            }
            Entry e(tmpPath, direntp->d_name, i, dir, hide, ex);
            fileObj.push_back(e);
            cnt++;
        }
        else if(!flags[0] && ftmp.at(0) != '.'){
            bool dir;
            if(S_ISDIR(buf.st_mode)){
                dir = true;
            }
            else{
                dir = false;
            }
            bool ex;
            if((S_IEXEC & buf.st_mode)){
                ex = true;
            }
            else{
                ex = false;
            }

            Entry e(tmpPath, direntp->d_name, i, dir, false, ex);
            fileObj.push_back(e);
            cnt++;
        }
        if(S_ISDIR(buf.st_mode) && ftmp != "." && ftmp != ".."
            && flags[2]){
            if(flags[0]){
                bool hide;
                if(ftmp.at(0) == '.'){
                    hide = true;
                }
                else{
                    hide = false;
                }
                Entry r(tmpPath, direntp->d_name, true, hide, false);
                paths.push_back(r);
            }
            else if(!flags[0] && ftmp.at(0) != '.'){
                Entry r(tmpPath, direntp->d_name, true, false, false);
                paths.push_back(r);
            }
        }
    }
    if(flags[1]){
        cout << "total " << total_block_size / 2 << endl;
    }
    sort(fileObj.begin(), fileObj.end());
    //double  total_line = getTotalChars(fileObj);
    //unsigned int int_total_line = (total_line / 60) + 0.5;
    unsigned int count = 0;
    unsigned int c_count = 0;
    //if(int_total_line <= 1 || flags[1]){
        for(unsigned int i = 0; i < fileObj.size(); i++){
            if(flags[1]){
                cout << fileObj.at(i).get_info();
                if(fileObj.at(i).isDir()){
                    cout << "\033[34m" << fileObj.at(i).get_name()
                        << "\033[0m" << endl;
                }
                else if(fileObj.at(i).isEx()){
                    cout << "\033[32m" << fileObj.at(i).get_name()
                        << "\033[0m" << endl;
                }
                else{
                    cout << fileObj.at(i).get_name() << endl;
                }
                c_count++;
            }
            if(!flags[1]){
                if(fileObj.at(i).isDir()){
                    unsigned int tmp = fileObj.at(i).get_name().size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << "\033[34m" << fileObj.at(i).get_name()
                        <<"\033[0m";
                    cout << "  ";
                }
                else if(fileObj.at(i).isEx()){
                    unsigned int tmp = fileObj.at(i).get_name().size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << "\033[32m" << fileObj.at(i).get_name()
                        << "\033[0m";
                    cout << "  ";
                }
                else{
                    unsigned int tmp = fileObj.at(i).get_name().size();
                    tmp += 2;
                    count += tmp;
                    if(count >= 80){
                        count = tmp;
                        cout << endl;
                    }
                    cout << fileObj.at(i).get_name();
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
    sort(paths.begin(), paths.end());
    fileObj.clear();
    closedir(dirp);
    if(paths.empty()){
        return;
    }
    total_block_size = 0;
    print_ls(flags, paths, paths.front().get_path());
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
    deque<Entry> paths;
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
        Entry r(".", ".", true, true, false);
        paths.push_back(r);
        string pth = ".";
        print_ls(flags, paths, pth);
        paths.pop_front();
    }
    else if(valid && files.size() > 0){
        //sort files by name
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
                    Entry r(files.at(i), files.at(i));
                    paths.push_back(r);
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
                    Entry r(files.at(i), files.at(i));
                    paths.push_back(r);
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
                    Entry r("./" + files.at(i), "./"
                        + files.at(i));
                    paths.push_back(r);
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
