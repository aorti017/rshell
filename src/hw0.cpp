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
    char* tmp;
    tmp = strtok(x, " ");
    while(tmp != NULL){
        v.push_back(tmp);
        tmp = strtok(NULL, " ");
    }
}

bool isExit(char x[]){
    string ext = "exit";
    string ext2 = " exit";
    string ext3 = "exit ";
    string ext4 = " exit ";
    if(x == ext || x == ext2 || x == ext3 || x == ext4){
        return true;
    }
    else{
        return false;
    }
}

string commentRemoval(string x){
    int comm = x.find('#');
    if(comm == -1){
        return x;
    }
    else if(comm == 0){
        return "";
    }
    else{
        return x.substr(0, comm);
    }
}

bool run(char str[]){
    char* pch;
    bool sucs = true;
    int status = 0;
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

    if(pch == strz){
        pch = strtok(str, "&&");
        if(pch == NULL){
            return true;
        }
        if(pch != strz){
            connector = "&&";
        }
    }

    if(pch == strz){
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
            int pid = fork();
            if(pid == -1){
                perror("fork");
                exit(1);
            }
            else if(pid == 0){
                parse(pch, cmd);
                int cmd_size = cmd.size() + 1;
                 char** argc = new char*[cmd_size];
                 for(unsigned int i = 0 ; i < cmd.size(); i++ ){
                	 argc[i] = new char[cmd.at(i).size()];
                	 strcpy(argc[i], cmd.at(i).c_str());
                 }
                 argc[cmd.size()] = NULL;
                 if(-1 ==  execvp(argc[0], argc)){
                    perror("execvp");
                    delete[] argc;
                    exit(1);
                 }
            }
            else{
                waitpid(-1, &status, 0);
                if(status > 0){
                    sucs = false;
                }
                else{
                    sucs = true;
                }
	            cmd.clear();
                if((connector=="&&" && sucs) || (connector=="||" && !sucs) || (connector==";")){
                    pch = strtok(NULL, connector.c_str());
                }
                else{
                    return true;
                }
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
       input = commentRemoval(input);
       int input_size = input.size()+1;
       char* str = new char[input_size];
       strcpy(str, input.c_str());
       goon = run(str);
       delete[] str;
   }
   return 0;
}
