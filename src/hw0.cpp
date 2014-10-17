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

bool conntest(string x){
    int i = x.size() - 1;
    int y = 0;
    if(x == "&&" || x == ";" || x == "||"){
        //cout << "a" << endl;
        return true;
    }
    else if(x.at(i) == ';' || x.at(i) == '&'
         || x.at(i) == '|')
    {
        return true;
    }
    else if(x.at(y) == ';' || x.at(y) == '|' || x.at(y) == '&'){
        return true;
    }
    else{
        return false;
    }
}

bool endswith(string x){
    int i = x.size() - 1;
    if(x.at(i) == ';' || x.at(i) == '&'
         || x.at(i) == '|')
    {
        return true;
    }
    else{
        return false;
    }
}

int findend(string x){
    for(int i = 0; i < x.size(); i++){
        if(x.at(i) == '&' || x.at(i) == ';'
            || x.at(i) == '|')
        {
            return i;
        }
    }
    return -1;
}

bool beginswith(string x){
    if(x.at(0) == ';' || x.at(0) == '&' || x.at(0) == '|'){
        return true;
    }
    else{
        return false;
    }
}


string chopFront(string x, vector<string> &v){
    if(x.at(0) == '&' && x.at(1) == '&'){
        v.push_back(x.substr(2,x.size()-1));
        return x.substr(0, 2);
    }
    else if(x.at(0) == '|' && x.at(1) == '|'){
        v.push_back(x.substr(2,x.size()-1));
        return x.substr(0, 2);
    }
    else{
        v.push_back(x.substr(1,x.size()-1));
        return x.substr(0, 1);
    }
}

bool run(char str[]){
    char* pch;
    bool sucs;
    string connector;
    vector<string> cmd;
    vector<string> addIn;
    pch = strtok(str, " ");

    while(pch != "exit"){
        if(pch == NULL || conntest(pch)){
            if(pch != NULL && (endswith(pch) || beginswith(pch))){
                string tmp = pch;
	            int loc = findend(pch);
	            if(loc > 0){
                    tmp = tmp.substr(0, loc);
                    cmd.push_back(tmp);
	            }
                if(loc == 0){
                    connector  = chopFront(pch, addIn);
                    cout << connector << endl;
                }
            }
            int pid = fork();
            if(pid == 0){
                char* argc[cmd.size() + 1];
                 for(int i = 0 ; i < cmd.size(); i++ ){
                	 argc[i] = new char[cmd.at(i).size()];
                	 strcpy(argc[i], cmd.at(i).c_str());
                 }
                 argc[cmd.size()] = NULL;
                 if(-1 ==  execvp(argc[0], argc)){
                    perror("execvp");
                 }
            }
            else{
                wait(NULL);
	            pch = strtok(NULL, " ");
	            cmd.clear();
                for(int i = 0; i < addIn.size(); i++){
                    cmd.push_back(addIn.at(i));
                }
                addIn.clear();
	            if(pch == "exit"){
	                exit(1);
                }
                if(pch == NULL && cmd.size() == 0){
	                return true;
                }
            }
       }
       else{
	       if(pch != " "){
               cmd.push_back(pch);
           }
           pch = strtok(NULL, " ");
	       if(pch == "exit"){
	           exit(1);
           }
       }
   }
   return false;
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
