//temp comment
#include <iostream>
#include "Timer.h"
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[])
{
    bool all= false;
    if(argc <3)
    {
        cout << "cp: missing file operand" << endl;
        cout << "To copy using all methods, use the -a flag. Otherwise, no flags uses the fastest method" << endl;
        return 0;
    }
    else if(argc ==4)
    {
        all = true;
    }
string path;
string tmp;
struct stat buf;
int file1;
int file2;
if(all && argv[1][0] == '-'){
    if(argv[1][1] != 'a'){
        cerr << "Invalid flag" << endl;
        return 0;
    }
    file1 = 2;
    file2 = 3;
}
else if(all && argv[2][0] == '-'){
    if(argv[2][1] != 'a'){
        cerr << "Invalid lag" << endl;
        return 0;
    }
    file1 = 1;
    file2 = 3;
}
else if(all && argv[3][0] == '-'){
    if(argv[3][1] != 'a'){
        cerr << "Invalid flag" << endl;
        return 0;
    }
    file1 = 1;
    file2 = 2;
}
else{
    file1 = 1;
    file2 = 2;
}
if(argv[file1][0] != '.' && argv[file1][0] != '/' ){
    path = argv[file1];
    string tmp = "./";
    path = tmp + path;
    if(stat(path.c_str(), &buf) == -1){
        perror("stat");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "Input file is dir" << endl;
        exit(0);
    }
}
else{
    if(stat(argv[file1], &buf)){
        perror("stat");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "Input files if dir" << endl;
        exit(0);
    }
}
if(ifstream(argv[file2])){
    if(-1 == stat(argv[2], &buf))
     {
         perror("stat");
         exit(0);
     }

if(S_ISDIR(buf.st_mode))
{
    cerr << "Output parameter is dir" << endl;
    exit(0);
}
else{
    cerr << "Output exists" << endl;
    exit(0);
}
}
else{
    ofstream tmp(argv[file2]);
    if(!tmp.good()){
        cerr << "Unable to open file" << endl;
        return 0;
    }
    tmp.close();
}
if(argv[file2][0] != '.' && argv[file2][0] != '/'){
    path = argv[file2];
    tmp = "./";
    path = tmp + path;
    if(stat(path.c_str(), &buf) == -1){
        perror("stat");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "Input file is dir" << endl;
        exit(0);
    }
}
else{
    if(stat(argv[file2], &buf) == -1){
        perror("stat");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "Input file is dir" << endl;
        exit(0);
    }
}
    if(all)
    {
    Timer t;
    double eTime;
    t.start();
    ifstream inpoo(argv[file1]);
    ofstream outpoo(argv[file2]);

    if(!inpoo.good()|| !outpoo.good())
    {
        cerr << "Error: No file" << endl;
        return 0;
    }


    while(inpoo.good())
    {
        char c = inpoo.get();
        if(inpoo.good())
        {
            outpoo.put(c);
        }
    }
    cout << "Method 1" << endl;
    t.elapsedWallclockTime(eTime);
    cout << "Wall clock time: " << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << "User time: " << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << "System time: " << eTime << endl;

    inpoo.close();
    outpoo.close();
    }
    //next
    if(all)
    {
    Timer t;
    double eTime;
    t.start();
    int in = open(argv[file1], O_RDONLY);
    int out = open(argv[file2], O_WRONLY);
    char buf;
    int n;
    if(out == -1)
    {
        ofstream create(argv[file2]);
        create.close();
        out = open(argv[file2], O_WRONLY);
    }

    while((n = read(in, &buf, 1)))
    {
        int w = write(out, &buf, 1);
        if(w == -1)
        {
            perror("write error");
            return 0;
        }
    }
    if (n == -1)
    {
        perror("woops");
        return 0;
    }
    close(in);
    close(out);
    cout << "Method 2" << endl;
    t.elapsedWallclockTime(eTime);
    cout << "Wall clock time: " << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << "User time: " << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << "System time: " << eTime << endl;
    }
    //next
    if(all)
    {
    Timer t;
    double eTime;
    t.start();
    int in = open(argv[file1], O_RDONLY);
    if(in == -1){
        perror("open");
        return 0;
    }
    int out = open(argv[file2], O_WRONLY);

    if(out == -1)
    {
        ofstream create(argv[file2]);
        create.close();
        out = open(argv[file2], O_WRONLY);
    }
    char* buf = new char[BUFSIZ];
    int n = read(in, buf, BUFSIZ);
    if (n == -1)
    {
        perror("erorr");
        return 0;
    }
    int w = write(out, buf, BUFSIZ);
    if(w == -1)
    {
        perror("error");
        return 0;
    }
    delete [] buf;
    cout << "Method 3" << endl;
    t.elapsedWallclockTime(eTime);
    cout << "Wall clock time: " << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << "User time: " << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << "System time: " << eTime << endl;

    close(in);
    close(out);
    }
    if(!all){
        Timer t;
    double eTime;
    t.start();
    int in = open(argv[file1], O_RDONLY);
    if(in == -1){
        perror("open");
        return 0;
    }
    int out = open(argv[file2], O_WRONLY);

    if(out == -1)
    {
        ofstream create(argv[file2]);
        create.close();
        out = open(argv[file2], O_WRONLY);
    }
    char* buf = new char[BUFSIZ];
    int n = read(in, buf, BUFSIZ);
    if (n == -1)
    {
        perror("erorr");
        return 0;
    }
    int w = write(out, buf, BUFSIZ);
    if(w == -1)
    {
        perror("error");
        return 0;
    }
    delete [] buf;
    cout << "Method 3" << endl;
    t.elapsedWallclockTime(eTime);
    cout << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << eTime << endl;

    close(in);
    close(out);
    }
      return 0;
}
