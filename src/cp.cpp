
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
        cerr << "not enough arguments" << endl;
        return 0;
    }
    else if(argc ==4)
    {
        all = true;
    }
    /*struct stat buf;
    string path = argv[1];
    string tmp = "./";
    path = tmp + path;
    if(stat(path.c_str(), &buf) == -1){
        perror("stat, asdasd");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "input file error " << endl;
        return 0;
    }

    path = argv[2];
    tmp = "./";
    path = tmp + path;

    if(stat(path.c_str(), &buf) == -1)
    {
        perror("stat, asdadasdasd");
        exit(0);
    }
    if(S_ISDIR(buf.st_mode)){
        cerr << "output file error " << endl;
        return 0;
    }
*/

    if(ifstream(argv[2]))
    {
        cerr << "ERror" << endl;
        return 0;
    }
    else
    {
        ofstream tmp(argv[2]);
        if(!tmp.good())
        {
            cerr << "unable to open" << endl;
            return 0;
        }

    }
string path;
    string tmp;
    struct stat buf;
    if(argv[1][0] != '.' && argv[1][0] != '/' ){
        path = argv[1];
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
        if(stat(argv[1], &buf)){
            perror("stat");
            exit(0);
        }
        if(S_ISDIR(buf.st_mode)){
            cerr << "Input files if dir" << endl;
            exit(0);
        }
    }
    if(argv[2][0] != '.' && argv[2][0] != '/'){
        path = argv[2];
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
        if(stat(argv[2], &buf) == -1){
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
    ifstream inpoo(argv[1]);
    ofstream outpoo(argv[2]);

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
    cout << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << eTime << endl;

    inpoo.close();
    outpoo.close();
    }
    //next
    if(all)
    {
    Timer t;
    double eTime;
    t.start();
    int in = open(argv[1], O_RDONLY);
    int out = open(argv[2], O_WRONLY);
    char buf;
    int n;
    if(out == -1)
    {
        ofstream create(argv[2]);
        create.close();
        out = open(argv[2], O_WRONLY);
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
    cout << eTime << endl;
    t.elapsedUserTime(eTime);
    cout << eTime << endl;
    t.elapsedSystemTime(eTime);
    cout << eTime << endl;
    }
    //next
    if(all)
    {
    Timer t;
    double eTime;
    t.start();
    int in = open(argv[1], O_RDONLY);
    if(in == -1){
        perror("open");
        return 0;
    }
    int out = open(argv[2], O_WRONLY);

    if(out == -1)
    {
        ofstream create(argv[2]);
        create.close();
        out = open(argv[2], O_WRONLY);
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
    if(!all){
        Timer t;
    double eTime;
    t.start();
    int in = open(argv[1], O_RDONLY);
    if(in == -1){
        perror("open");
        return 0;
    }
    int out = open(argv[2], O_WRONLY);

    if(out == -1)
    {
        ofstream create(argv[2]);
        create.close();
        out = open(argv[2], O_WRONLY);
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
