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
        for(int i = 0; i < x.size(); i++){
            if(x.at(i) == ';' || x.at(i) == '|' || x.at(i) == '&'){
                return true;
            }
        }
        return false;
    }
}

string parse(string x, string& c, vector<string> &v){
    if(x == ";" || x == "||" || x == "&&"){
        c = x;
        return "";
    }
    if(x.at(0) == ';'){
        c = x.at(0);
        v.push_back(x.substr(1,x.size()-1));
        return "";
    }
    else if(x.at(0) == '&' && x.at(1) == '&'){
        c = x.substr(0,1);
        v.push_back(x.substr(2, x.size()-1));
        return "";
    }
    else if(x.at(0) == '|' && x.at(1) == '|'){
        c = x.substr(0,1);
        v.push_back(x.substr(2, x.size()-1));
        return "";
    }
    else if(x.at(x.size()-1) == ';'){
        c = x.at(0);
        return x.substr(0, x.size()-1);
    }
    else if(x.at(x.size()-2) == '|' && x.at(x.size()-1) == '|'){
        c = x.substr(x.size()-2, x.size()-1);
        return x.substr(0, x.size() - 2) ;
    }
    else if(x.at(x.size()-2) == '&' && x.at(x.size()-1) == '&'){
        c = x.substr(x.size()-2, x.size()-1);
        return x.substr(0, x.size() - 2) ;
    }
    else{
        for(int i = 0; i < x.size(); i++){
            if(x.at(i) == ';' || x.at(i) == '&' || x.at(i) == '|'){
                if(x.at(i) == ';'){
                    c = x.at(i);
                    v.push_back(x.substr(i+1, x.size()-1));
                    return x.substr(0, i);
                }
                else if(i != x.size() - 1 && (x.at(i) == '&' || x.at(i) == '|')){
                    c = x.substr(i, i + 1);
                    v.push_back(x.substr(i+2, x.size() -1));
                    return x.substr(0, i);
                }
            }
        }
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
            if(pch != NULL){
                    string tmp = pch;
                    tmp = parse(tmp, connector, addIn);
                    if(tmp != ""){
                        cmd.push_back(tmp);
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
                 cout << cmd.at(0) << endl;
                 if(-1 ==  execvp(argc[0], argc)){
                    perror("execvp");
                    return false;
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
