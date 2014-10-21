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

//parses the entered command putting each string seperated
//by spaces into a vector passed in by reference
//the function utilizes strtok to do this
void parse(char  x[], vector<string> &v){
    char* tmp;
    //separates the string by spaces
    tmp = strtok(x, " ");
    while(tmp != NULL){
        //stores in the vector
        v.push_back(tmp);
        //searches for the next " " character
        tmp = strtok(NULL, " ");
    }
}

//the function returns false if the command passed in
//is "exit" or any spaced variation of it.
//returnd false otherwise
bool isExit(char x[]){
    //sets up vars necessary for comparison
    string tmp = x;
    string hold = "exit";
    char spc = ' ';
    unsigned int k = 0;

    //if the size of the passed in char[] is less than that
    //necessary for it to be exit return false
    if(tmp.size() < hold.size()){
        return false;
    }

    //check each non-space character if within the first
    //four characters any of the characters dont align with "exit" return false
    //otherwise count how many more characters there are in the passed in string
    //if there are more than 4 return false otherwise return true
    //if a space appears when k is > 0 but < 4 return false, this prevents
    //bugs like "exi t" from working
    for(unsigned int i = 0; i < tmp.size(); i++){
        if(tmp.at(i) != spc){
            if(k < hold.size() && tmp.at(i) != hold.at(k)){
                return false;
            }
            k++;
        }
        else if(k > 0 && k < hold.size()){
            return false;
        }
    }
    if(k != hold.size()){
        return false;
    }
    else{
        return true;
    }
    //alternative way to parse that causes a bug when calling exit
    /*
    //sets up any of the acceptable commands
    //to exit
    string ext = "exit";
    string ext2 = " exit";
    string ext3 = "exit ";
    string ext4 = " exit ";
    //checks if the command is eqaul to any of the acceptable forms
    //returning true if equal to any and false otherwise
    if(x == ext || x == ext2 || x == ext3 || x == ext4){
        return true;
    }
    else{
        return false;
    }
    */
}

bool multiConn(string x){
    int amp = 0;
    int ln = 0;
    int semi = 0;
    int total = 0;
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) == '&' && i != x.size()-1){
            if(x.at(i+1) == '&'){
                amp++;
            }
        }
        if(x.at(i) == '|' && i != x.size()-1){
           if(x.at(i+1) == '|'){
               ln++;
           }
        }
        if(x.at(i) == ';'){
            semi++;
        }

    }
    if(amp > 0){
        total++;
    }
    if(semi > 0){
        total++;
    }
    if(ln > 0){
        total++;
    }
    if(total > 1){
        return true;
    }
    else{
        return false;
    }
}


//the function searches for the '#' and
//removes anything after that character
string commentRemoval(string x){
    int comm = x.find('#');
    //if no '#' is found return the entire string
    if(comm == -1){
        return x;
    }
    //if the '#' is the first character return nothing
    else if(comm == 0){
        return "";
    }
    //otherwise return every thing up to the '#'
    else{
        return x.substr(0, comm);
    }
}

//this function runs the command using fork and execvp
//and returns once all commands from the entered char[]
//have been executed
void run(char str[]){

    //needed to parse with strtok
    char* pch;

    //holds the bool of if a command executed properly
    bool sucs = true;

    //holds the return status of a fork/execvp
    int status = 0;

    //holds the connector used for the current list of comamnds
    string connector;

    //holds a string version of the passed in char[] for later
    //comparing
    string strz = str;

    //the vector that holds the commands to be converted and executed
    vector<string> cmd;

    //cbegins breaking the entered commands up by connector
    pch = strtok(str, ";");

    //if the command is empty return
    if(pch==NULL){
        return;
    }

    //if using strtok with ";" changed the strz
    //set the connector to be ";"
    else if(pch != strz){
        connector = ";";
    }
    //else if strz was unchanged use strtok with "&&"
    //if this changes the string is NULL return
    //of if strtok changed from the original value of strz set the connector
    if(pch == strz){
        pch = strtok(str, "&&");
        if(pch == NULL){
            return;
        }
        if(pch != strz){
            connector = "&&";
        }
    }
    //repeat the above process but with "||" instead of "&&"
    if(pch == strz){
        pch = strtok(str, "||");
        if(pch == NULL){
            return;
        }
        if(pch != strz){
            connector = "||";
        }
    }

    //if the pch is not NULL and is the exit command exit the programm
    if(pch != NULL && isExit(pch)){
        exit(0);
    }

    //this while loop is where fork and execvp execute commands
    while(pch != NULL){
        //fork the programm
        int pid = fork();
        //if pid is -1 the fork failed so exit
        if(pid == -1){
            perror("fork");
            exit(1);
        }
        //if the pid is 0 the current id the current process is the child
        else if(pid == 0){
            //call the parsing function on the command and the cmd vector
            //to break it up into command and params
            parse(pch, cmd);
            //set the size of the dynamic char** that will be passed into execvp
            int cmd_size = cmd.size() + 1;
            char** argc = new char*[cmd_size];
            //for each string in cmd copy it into argc, which will be passed
            //into execvp
            for(unsigned int i = 0 ; i < cmd.size(); i++ ){
                argc[i] = new char[cmd.at(i).size()];
                strcpy(argc[i], cmd.at(i).c_str());
            }
            //set the last value of argc to be NULL so that execvp will work properly
            argc[cmd.size()] = NULL;
            //call execvp on the first element of argc and the entirety of it
            //if it returns -1 it has failed fo print an error and delete
            //the dynamically allocated memory
            if(-1 ==  execvp(argc[0], argc)){
                perror("execvp");
                delete[] argc;
                exit(1);
            }
        }
        //otherwise it is the parrent process
        else{
            //wait for any process to exit, in this case I only created on,
            //and store its exit code in status
            if(-1 == waitpid(-1, &status, 0)){
	        perror("waitpid");
		exit(1);
	    }

            //if the value of status is larger than 0
            //it failed
            if(status > 0){
                sucs = false;
            }
            //otherwise if succeeded
            else{
                sucs = true;
            }

            //clear the vector holding the command to execute
	        cmd.clear();

            //run the next command if the connector logic and value of sucs allow it
            if((connector=="&&" && sucs) || (connector=="||" && !sucs) || (connector==";")){
                pch = strtok(NULL, connector.c_str());
            }
            //otherwise return
            else{
                return;
            }
            //if the next command is not NULL and is exit exit the program
            if(pch != NULL && isExit(pch)){
                exit(0);
            }
        }
    }
    //if there are no more commands to execute/parse return
    return;
}

//main takes in commands and passes them to run to execute
int main(){
    //continue until terminated by a conditional branch within run
    while(true){
        //get the users command including all spaces
        char* login = getlogin();
        if(login == NULL){
            perror("getlogin");
        }
        char host[128];
        int hostFlag;
        string input;
        if((hostFlag = gethostname(host, sizeof(host))) == -1){
            perror("gethostname");
        }
        if(login != NULL && hostFlag != -1){
            cout << login << "@" << host << "$ ";
        }
        else{
            cout << "$ ";
        }
        getline(cin, input);

        //remove anything that is a comment from the users input
        input = commentRemoval(input);
        if(!multiConn(input)){
            //determines the size of the input
            int input_size = input.size()+1;
            //dynamically allocates a char*[] of the size of the input + 1
            char* str = new char[input_size];
            //copies the input into the char* str[]
            strcpy(str, input.c_str());
            //calls run on the users entered commands
            run(str);
            //after running the dynamically allocated memory is deleted
            delete[] str;
        }
        else{
            cerr << "Multiple connector types forbidden" << endl;
        }
   }
   return 0;
}
