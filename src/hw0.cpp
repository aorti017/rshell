#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
using namespace std;

void parse(char  x[], vector<string> &v){
    //cout << "parse executes" << endl;
    char* tmp;
    tmp = strtok(x, " ");
    while(tmp != NULL){
        v.push_back(tmp);
        //cout << "*" << tmp << "*" << endl;
        tmp = strtok(NULL, " ");
    }
}

bool isExit(char x[]){
    string tmp = x;
    string ext = "exit";
    int e = tmp.find('e');
    int t = tmp.find('t', e);
    if(e == -1 || t == -1){
        return false;
    }
    int k = 0;
    for(int i = e; i < t + 1; i++){
        if(tmp.at(i) != ext.at(k)){
            return false;
        }
        k++;
    }
    return true;
}

bool run(char str[]){
    char* pch;
    bool sucs;
    bool found = true;
    string ext = "exit";
    string connector;
    string strz = str;
    vector<string> cmd;
    pch = strtok(str, ";");

    if(pch==NULL){
        return true;
    }

    else if(pch != strz){
        connector = ";";
    }

    else if(pch == strz){
        pch = strtok(str, "&&");
        if(pch == NULL){
            return true;
        }
        if(pch != strz){
            connector = "&&";
        }
    }

    else if(pch == strz){
        pch = strtok(str, "||");
        if(pch == NULL){
            return true;
        }
        if(pch != strz){
            connector = "||";
        }
    }
    if(pch != NULL && isExit(pch)){
        exit(0);
    }
    while(pch != NULL){
        //cout << "execvp executes" << endl;
            int pid = fork();
            if(pid == 0){
                parse(pch, cmd);
                char* argc[cmd.size() + 1];
                 for(int i = 0 ; i < cmd.size(); i++ ){
                	 argc[i] = new char[cmd.at(i).size()];
                	 strcpy(argc[i], cmd.at(i).c_str());
                 }
                 argc[cmd.size()] = NULL;
                 if(-1 ==  execvp(argc[0], argc)){
                    perror("execvp");
                    return false;
                 }
            }
            else{
                wait(NULL);
	            cmd.clear();
                pch = strtok(NULL, connector.c_str());
                if(pch != NULL && isExit(pch)){
                    exit(0);
                }
            }
   }
   return true;
}



int main(){
   bool goon = true;
   while(goon){
       string input;
       cout << "$ ";
       getline(cin, input);
       char str[input.size()+1];
       strcpy(str, input.c_str());
       goon = run(str);
   }
   return 0;
}
