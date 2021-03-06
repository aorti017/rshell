//comment again
#include <iostream>
#include <signal.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
using namespace std;

static void handler(int signum){
    if(signum == SIGINT){
        int pid = getpid();
        if(-1 == pid){
            perror("getpid");
            exit(0);
        }
        if(pid == 0){
            exit(0);
        }
    }
}

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
}

//traverses the entered list of commands
//and checks to see how many different kinds of
//connectors there are, if there are more than one
//it returns true else returns false
bool multiConn(string x){
    int amp = 0;
    int ln = 0;
    int semi = 0;
    int total = 0;
    //if any part of the string does not
    //align with "exit" returns false, otherwise counts
    //all characters after the passed in command
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
    //calculates the total amount of different connectors
    if(amp > 0){
        total++;
    }
    if(semi > 0){
        total++;
    }
    if(ln > 0){
        total++;
    }
    //if more than one return true
    if(total > 1){
        cerr << "Multiple connector types forbidden" << endl;
        return true;
    }
    //else false
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

vector<unsigned int> getPipes(string &str){
    vector<unsigned int> v;
    if(str.size() > 0 && str.at(0) == '|'){
        cerr << "Invalid pipe placement" << endl;
        v.push_back(0);
        return v;
    }
    if(str.at(str.size()-1) == '|'){
        cerr << "Invalid pipe placement" << endl;
        v.push_back(0);
        return v;
    }
    for(unsigned int i = 0; i < str.size(); i++){
        if(str.at(i) == '|' && i + 1 < str.size()){
            if(str.at(i+1) != '|' && str.at(i-1) != '|'){
                v.push_back(i);
                str.at(i) = '~';
            }
        }
        else if(str.at(i) == '|' && i + 1 >= str.size()){
            if(str.at(i-1) != '|'){
                v.push_back(i);
                str.at(i) = '~';
            }
        }
    }
    return v;
}

string is_iore(vector<string> cmd){
    for(unsigned int i = 0; i < cmd.size(); i++){
        if(cmd.at(i) == "<" || cmd.at(i) == ">>" || cmd.at(i) == ">" || cmd.at(i) == "<<<"){
            return cmd.at(i);
        }
    }
    return "NULL";
}

vector<string> remRed(vector<string> v){
    vector<string> retVec;
    for(unsigned i = 0; i < v.size(); i++){
        if(v.at(i) != "<" && v.at(i) != ">" && v.at(i) != ">>" && v.at(i) != "<<<"){
            if(v.at(i) == "|" && i != 0){
                return retVec;
            }
            else if(v.at(i) != "|"){
                if(!isdigit(v.at(i).at(0))){
                    retVec.push_back(v.at(i));
                }
                else if(isdigit(v.at(i).at(0)) && i+1 >= v.size()){
                    retVec.push_back(v.at(i));
                }
                else if(isdigit(v.at(i).at(0)) && i+1 < v.size()){
                    if(v.at(i+1) != ">" && v.at(i+1) != ">>"){
                        retVec.push_back(v.at(i));
                    }
                }
            }
        }
        else{
            return retVec;
        }
    }
    return retVec;
}


string getFile(vector<string> v){
    for(unsigned int i = 0; i < v.size(); i++){
        if((v.at(i) == "<" || v.at(i) == ">" || v.at(i) == ">>"  || v.at(i) == "<<<")
            && (i+1 < v.size())){
            return v.at(i+1);
        }
        else if((v.at(i) == "<" || v.at(i) == ">" ||
            v.at(i) == ">>" || v.at(i) == "<<<") && (i+1 >= v.size())){
            return "|";
        }
    }
    return "|";
}

vector<string> getRed(vector<string> v){
    vector<string> ret;
    for(unsigned int i = 0; i < v.size(); i ++){
        if(v.at(i) == "<"){
            ret.push_back(v.at(i));
        }
        else if(v.at(i) == ">"){
            ret.push_back(v.at(i));
        }
        else if(v.at(i) == ">>" || v.at(i) == "<<<"){
            ret.push_back(v.at(i));
        }
        else if(v.at(i) == "|"){
            ret.push_back(v.at(i));
        }
    }
    return ret;
}

vector<string> remCmd(vector<string> v){
    vector<string> ret;
    bool push = false;
    string temp = "";
    bool quo = false;
    for(unsigned int i = 0; i < v.size(); i++){
        if(v.at(i) == "<" || v.at(i) == ">" || v.at(i) == ">>" || v.at(i) == "|" || v.at(i) == "<<<"){
            push = true;
        }
        if(push || quo){
            if(quo){
                temp.append(" ");
                temp.append(v.at(i));
            }
            if(v.at(i).at(0) == '"'){
                quo = true;
                temp.append(v.at(i));
            }
            if(!isdigit(v.at(i).at(0)) && !quo){
                ret.push_back(v.at(i));
            }
            if(v.at(i).at(v.at(i).size()-1) == '"'){
                quo = false;
                ret.push_back(temp);
                temp = "";
            }
        }
    }
    return ret;
}

vector<string> remPrev(vector<string> v){
    bool first = true;
    bool add = false;
    vector<string> ret;
    for(unsigned int i = 0; i < v.size(); i++){
        if(v.at(i) == "<" || v.at(i) == ">" || v.at(i) == ">>" || v.at(i) == "<<<"){
            if(first){
                add = false;
                first = false;
            }
            else{
                add = true;
            }
        }
        if(add){
            ret.push_back(v.at(i));
        }
    }
    return ret;
}

int lastRan(vector<int> v, int x){
    int ret = 0;
    for(unsigned int i = 0; i < v.size(); i++){
        if(v.at(i) == x){
            ret = i;
        }
    }
    return ret;
}

bool pipe_aft(int total, string x){
    if(x.at(total) == '|' && x.size() <= 1){
        return false;
    }
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) == '|' && i != 0){
            return true;
        }
    }
    return false;
}
bool pipe_bef(int cmd, int total, string x){
    int nu = total - cmd;
    if(nu < 0){
        return false;
    }
    else if(nu == 0){
        return x.at(nu) == '|';
    }
    return false;
}

bool hasPipe(string x){
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) == '|' && i + 1 < x.size()){
            if(x.at(i+1) != '|'){
                if(i >= 1){
                    if(x.at(i-1) != '|'){
                        return true;
                    }
                }
                else{
                    return true;
                }
            }
        }
    }
    return false;
}

vector<string> get_next_cmd(vector<string> &v){
    vector<string> ret;
    vector<string> ref_ret;
    bool stop = false;
    for(unsigned int i = 0; i < v.size(); i++){
        if(v.at(i) != "|" && !stop){
            ret.push_back(v.at(i));
        }
        else if(!stop && i !=0){
            ret.push_back(v.at(i));
            ref_ret.push_back(v.at(i));
            stop = true;
        }
        else if(!stop && i==0){
            ret.push_back(v.at(i));
        }
        else if(stop){
            ref_ret.push_back(v.at(i));
        }
    }
    v = ref_ret;
    return ret;
}

int num_re(string x){
    /*int ret_ret = 0;
    int mult = 1;
    int ret = -1;
    unsigned int i = 0;*/

    bool stop = true;
    vector<string> broken;
    string temp = "";
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) != ' '){
            if(temp == "" && isdigit(x.at(i))){
                stop = false;
            }
            if(x.at(i) == '>'){
                stop = true;
            }
            temp.push_back(x.at(i));
        }
        else if(stop){
            if( temp.size() > 0 && temp.at(temp.size()-1) == '>'){
                break;
            }
           /* if(temp.at(temp.size()-1) == '>'){
            cout << temp << endl;
                broken.push_back(temp);
            }*/
            temp = "";
        }
    }
    /*for(unsigned int i = 0; i < broken.size() - 1; i++){
        string t = broken.at(i);
        t = t.substr(0, t.size()-2);
        cout << t << endl;
        temp.append(t);
    }*/
    if(temp.size() <= 1 ||  temp.at(0) == '>'){
        int ret = -1;
        return ret;
    }
    int ret = atoi(temp.c_str());
    return ret;
}

//this function runs the command using fork and execvp
//and returns once all commands from the entered char[]
//have been executed
bool run(char str[]){

    //needed to parse with strtok
    char* pch;

    //holds the bool of if a command executed properly
    bool sucs = true;

    //holds the return status of a fork/execvp
    int status = 0;

    //holds the connector used for the current list of comamnds
    string connector;
    string strz = str;
    string pipe_cpy = str;
    string num_pipe_cpy = str;
    vector<string> current_command_check;
    string temp;
    for(unsigned int i = 0; i < pipe_cpy.size(); i++){
        //cout << pipe_cpy.at(i) << endl;
        if(pipe_cpy.at(i) != ' ' && i + 1 < pipe_cpy.size()){
            temp.push_back(pipe_cpy.at(i));
        }
        else if(pipe_cpy.at(i) != ' ' && i+1 >= pipe_cpy.size()){
            temp.push_back(pipe_cpy.at(i));
            current_command_check.push_back(temp);
            //cout << temp << endl;
            temp = "";
        }
        else{
            current_command_check.push_back(temp);
            //cout << temp << endl;
            temp = "";
        }
    }
    //cout << "ENOD" << endl;
    //holds a string version of the passed in char[] for later
    //comparing
    vector<unsigned int> pipeLocals = getPipes(strz);
    if(pipeLocals.size() == 1 && pipeLocals.at(0) == 0){
        return true;
    }
    strcpy(str, strz.c_str());
    /*for(unsigned int i = 0; i < pipeLocals.size(); i++){
        cout << pipeLocals.at(i) << " "
        << strz[pipeLocals.at(i)];
        cout << endl;
    }
    cout << str << endl;*/
    strz = str;
    //the vector that holds the commands to be converted and executed
    vector<string> cmd;

    string p = str;
    int ogsz = p.size();

    //cbegins breaking the entered commands up by connector
    pch = strtok(str, ";");

    //if the command is empty return
    if(pch==NULL){
        return true;
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
            return true ;
        }
        if(pch != strz){
            connector = "&&";
        }
    }
    //repeat the above process but with "||" instead of "&&"
    if(pch == strz){
        pch = strtok(str, "||");
        if(pch == NULL){
            return true;
        }
        if(pch != strz){
            connector = "||";
        }
    }

    //if the pch is not NULL and is the exit command exit the programm
    if(pch != NULL && isExit(pch)){
        return false;
    }
    int fromHead = 0;
    if(pipeLocals.size() > 0){
        string pchSz = strz;
        unsigned int k = 0;
        for(unsigned int i = 0; i < pchSz.size(); i++){
            if(pipeLocals.at(k) == i){
                str[i] = '|';
                k++;
                if(k >= pipeLocals.size()){
                    break;
                }
            }
        }
    }
    bool sec_run_pipe = false;
    int fd[2];
    if(-1 == pipe(fd)){
        perror("pipe");
        exit(0);
    }
    //bool last_out = false;
    //int bu = 0;
    //cout << pch << endl;
    //this while loop is where fork and execvp execute commands
    while(pch != NULL){
        //cout << bu << endl;
        //bu++;
        string pmt = pch;
        if(connector != ";"){
            fromHead += pmt.size() +2;
        }
        else{
            fromHead += pmt.size()+1;
        }
            //call the parsing function on the command and the cmd vector
            if(sec_run_pipe){
                pipe_cpy = pch;
                //char buf[BUFSIZ];
            //read(fd[0], buf, BUFSIZ);
            //cout << buf << endl;
            }

            //to break it up into command and params
            parse(pch, cmd);

            //look here to see if redirection
            //then remove and store file
            bool iRed = false;
            //int fd[2];
            //bool changed[2];
            //vector<int> changed_io;
            int changed_io_in;
            int changed_io_out;
            //changed[0] = false;
            //changed[1] = false;
            //vector<int> defout_vec;
            int defout_in;
            int defout_out;
            int fd_in = 0;
            int fd_out = 0;

            bool i_ran = false;
            bool o_ran = false;

            int curr_cmd = 0;
            int last = 0;
            int total = 0;

            //vector<int> fd_vec;
            vector<string> to_parse = get_next_cmd(current_command_check);
            //cout << "<====" << endl;
            //for(unsigned int i = 0; i < to_parse.size(); i++){
            //    cout << to_parse.at(i) << endl;
            //}
            //cout << "=====>" << endl;
            //for(unsigned int i = 0; i < to_parse.size(); i++){
            //    cout << to_parse.at(i) << endl;
            //}
            //cout << "DONE" << endl;
            //for(unsigned int i =0 ; i < current_command_check.size(); i++){
            //    cout << current_command_check.at(i) << endl;
            //}
            vector<string> listRed = getRed(to_parse);
            vector<string> cmd_cpy = cmd;
            vector<string> iuck = cmd;
            bool pip = hasPipe(pipe_cpy);
            if(listRed.size() > 0){
                cmd = remRed(cmd);
                cmd_cpy = remCmd(cmd_cpy);
            }
            //cout << "****begin****" << endl;
            //for(unsigned int i = 0; i < cmd.size(); i++){
            //    cout << cmd.at(i) << endl;
            //}
            //cout << "****end****" << endl;
            //bool restore_error = false;
            //int save_err = 0;
            int x = num_re(pipe_cpy);
            bool custom_out = false;
            if(x != -1){
                custom_out = true;
            }
            bool out = false;
            bool trip = false;
            string trip_str = "";
            for(unsigned int i = 0; i < listRed.size(); i++){
                if(listRed.at(i) != "NULL"){
                    if(listRed.at(i) == "<"){
                        iRed = true;
                        changed_io_in = 0;
                        defout_in = dup(0);
                        if(defout_in == -1){
                            perror("dup");
                            return true;
                        }
                        string file = getFile(cmd_cpy);
                        cmd_cpy = remPrev(cmd_cpy);
                        if(file != "|"){
                            fd_in = open(file.c_str(), O_RDONLY);
                            if(fd_in == -1){
                                perror("open");
                                return true;
                            }
                            i_ran = true;
                        }
                        else{
                            cout << "No input given" << endl;
                            return true;
                        }
                    }
                    else if(listRed.at(i)  == ">" || listRed.at(i) == ">>"){
                        if(x != 0){
                            out = true;
                        }
                        iRed = true;
                        changed_io_out = 1;
                        defout_out = dup(1);
                        if(defout_out == -1){
                            perror("dup");
                            return true;
                        }
                        string file = getFile(cmd_cpy);
                        cmd_cpy = remPrev(cmd_cpy);
                        int mode = 0666;
                        if(file != "|"){
                            if(listRed.at(i) == ">"){
                                fd_out = open(file.c_str(), O_WRONLY |
                                    O_TRUNC | O_CREAT, mode);
                                if(fd_out == -1){
                                    perror("open");
                                    return true;
                                }
                            }
                            else{
                                fd_out = open(file.c_str(), O_WRONLY |
                                    O_APPEND | O_CREAT, mode);
                                if(fd_out == -1){
                                    perror("open");
                                    return true;
                                }
                            }
                            o_ran = true;
                        }

                        else{
                            cout << "No output file given" << endl;
                            return true;
                        }
                    }
                    else if(listRed.at(i) == "<<<"){
                        trip = true;
                        string file = getFile(cmd_cpy);
                        if(file.size() > 2){
                            file = file.substr(1, file.size()-2);
                        }
                        else{
                            file = "";
                        }
                        trip_str = file;
                    }
                    else if(listRed.at(i) == "|" && i != 0){
                        break;
                    }
                }
            }

            /* here search for the first element of argc,
             * once found change it to the path
             */
            char* master_path = getenv("PATH");
            if(master_path == NULL){
                perror("getnev");
                exit(0);
            }
            string master_paths = master_path;
            string temp = "";
            vector<string> paths;
            //paths.push_back(".");
            for(unsigned int i = 0; i < master_paths.size(); i++){
                if(master_paths.at(i) != ':'){
                    temp.push_back(master_paths.at(i));
                }
                else{
                    paths.push_back(temp);
                    temp = "";
                }
            }
            string ui_cmd = cmd.at(0);
            //struct stat s_buf;
            vector<string> path_cmd;
            for(unsigned int i = 0; i < paths.size(); i++){
                string xemp = paths.at(i);
                xemp.append("/");
                xemp.append(ui_cmd);
                path_cmd.push_back(xemp);
                xemp = "";
                //if(stat(xemp.c_str(), &s_buf) == 0){
                //    ui_cmd = xemp;
                //    break;
                //}
            }
            path_cmd.push_back(cmd.at(0));
            int cmd_size = cmd.size();
            char** argc = new char*[cmd_size + 1];
            //for each string in cmd copy it into argc, which will be passed
            //into execvp
            for(unsigned int i = 0 ; i < cmd.size(); i++ ){
                if(i == 0){
                    argc[0] = new char[ui_cmd.size()+1];
                    strcpy(argc[0], "");
                }
                else{
                    argc[i] = new char[cmd.at(i).size() + 1];
                    strcpy(argc[i], cmd.at(i).c_str());
                }
            }
            //set the last value of argc to be NULL so that execvp will work properly
            argc[cmd.size()] = NULL;

            bool after = false;
            bool before = false;
            if(pip){
            bool stop = false;
            curr_cmd = 0;
            for(unsigned int i = last; i < iuck.size(); i++){
                for(unsigned int k = 0; k < iuck.at(i).size(); k++){
                    if(iuck.at(i) == "|"){
                        stop = true;
                        break;
                    }
                    curr_cmd++;
                }
                if(stop){
                    break;
                }
                curr_cmd++;
            }
            total += curr_cmd;
            after = pipe_aft(total, pipe_cpy);
            before = pipe_bef(curr_cmd, total, pipe_cpy);
            last = curr_cmd;
            curr_cmd = 0;
        }
         int fd_2[2];
         if(before && !i_ran){
                if(-1 == close(fd[1])){
                    perror("close");
                    exit(0);
                }
         }
         if(after){
            if(-1 == pipe(fd_2)){
                perror("pipe");
                exit(0);
            }
        }
    if(cmd.at(0) != "cd"){
        //fork the programm
        int pid = fork();
        //if pid is -1 the fork failed so exit
        if(pid == -1){
            perror("fork");
            exit(1);
        }
        //if the pid is 0 the current id the current process is the child
        else if(pid == 0){
            if(i_ran){
                if(-1 == close(0)){
                    perror("close");
                    exit(0);
                }
                if(-1 == dup(fd_in)){
                    perror("dup");
                    return true;
                }
            }
            if(o_ran){
                if(x != 0 && x != 2){
                    if(-1 == close(1)){
                        perror("close");
                        exit(0);
                    }
                    if(-1 == dup(fd_out)){
                        perror("dup");
                        return true;
                    }
                }
                if(custom_out && x != 0){
                    if(-1 == close(x)){
                        perror("close");
                        exit(-1);
                    }
                    if(-1 == dup(fd_out)){
                        perror("dup");
                        return true;
                    }
                }
            }
            //check if there is a pipe coming up, but none behind
            //if so write to pipe
            if(before && !i_ran){
                if(-1 == dup2(fd[0], 0)){
                    perror("dup2");
                    exit(0);
                }
            }
            if(before && after && !out){
                if(-1 == dup2(fd_2[1], 1)){
                    perror("dup2");
                    exit(0);
                }
                if(-1 == close(fd_2[0])){
                    perror("close");
                    exit(0);
                }
            }
            //if there is a pipe coming up, and one behind
            //read from pipe and output to pipe
            else if(!before && after && !out){
                if(-1 == dup2(fd[1], 1)){
                    perror("dup2");
                    exit(0);
                }
                if(-1 == close(fd[0])){
                    perror("close");
                    exit(0);
                }
            }
            if(trip){
                char* buf = new char[BUFSIZ];
                strcpy(buf, trip_str.c_str());
                int fd_3[2];
                if(-1 == pipe(fd_3)){
                    perror("pipe");
                    exit(0);
                }
                if(-1 == write(fd_3[1], buf, trip_str.size()+1)){
                    perror("write");
                    exit(0);
                }
                if(-1 == close(fd_3[1])){
                    perror("close");
                    exit(0);
                }
                if(-1 == dup2(fd_3[0], 0)){
                    perror("dup2");
                    exit(0);
                }
                delete[] buf;
            }
            if(cmd.at(0).at(0) != '.'){
            for(unsigned int i = 0; i < path_cmd.size(); i++){
		delete[]  argc[0];
		argc[0] = new char[path_cmd.at(i).size() + 1];
		strcpy(argc[0], path_cmd.at(i).c_str());
                execv(path_cmd.at(i).c_str(), argc);
            }
            perror("execv");
	    exit(1);
            }
            else{
		delete[] argc[0];
		argc[0] = new char[cmd.at(0).size()+1];
		strcpy(argc[0], cmd.at(0).c_str());
                if(-1 == execv(cmd.at(0).c_str(), argc)){
                    perror("execv");
                    exit(1);
                }
            }
        }
        else{
            //wait for any process to exit, in this case I only created on,
            //and store its exit code in status
            if(-1 == waitpid(-1, &status, 0)){
    	        perror("waitpid");
                delete[] argc;
	        	exit(1);
	        }
        }
    }
            if(cmd.at(0) == "cd"){
                char* path = getenv("HOME");
                if(path == NULL){
                    perror("getenv");
                    exit(0);
                }
                if(cmd.size() >=  2){
                    if(-1 == chdir(cmd.at(1).c_str())){
                        perror("chdir");
                        status = 1;
                    }
                }
                else{
                    if(-1 == chdir(path)){
                        perror("chdir");
                        status = 1;
                    }
                }
            }
            if(before && after){
                fd[1] = fd_2[1];
                fd[0] = fd_2[0];
            }
            if(iRed){
                if(i_ran){
                    if(-1 == close(fd_in)){
                        perror("close");
                        exit(0);
                    }
                    if(-1 == dup2(defout_in, changed_io_in)){
                        perror("dup2");
                        return true;
                    }
                    if(-1 == close(defout_in)){
                        perror("close");
                        exit(0);
                    }
                }
                if(o_ran){
                    if(-1 == close(fd_out)){
                        perror("close");
                        exit(0);
                    }
                    if(-1 == dup2(defout_out, changed_io_out)){
                        perror("dup2");
                        return true;
                    }
                    if(-1 == close(defout_out)){
                        perror("close");
                        exit(0);
                    }
                }
            }
            for(unsigned int i = 0; i <= cmd.size(); i++){
                delete[] argc[i];
            }
            delete[] argc;
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
            if(pip){
                unsigned int count = 0;
                bool stop = false;
                string next_cmd;
                for(unsigned int i = 0; i < iuck.size(); i++){
                    for(unsigned int k = 0; k < iuck.at(i).size(); k++){
                        next_cmd.push_back(iuck.at(i).at(k));
                        if(iuck.at(i).at(k) == '|' && !stop
                            && i!=0){
                            stop = true;
                            break;
                        }
                        if(!stop){
                            count++;
                        }
                    }
                    if(i+1 < iuck.size()){
                        next_cmd.push_back(' ');
                    }
                    if(!stop){
                        count++;
                    }
                }
                if(count >= next_cmd.size()){
                    return true;
                }
                next_cmd = next_cmd.substr(count, next_cmd.size());
                strcpy(pch, next_cmd.c_str());
                if(count == 0){
                    return true;
                }
                sec_run_pipe = true;
                continue;
            }
            else if((connector=="&&" && sucs) || (connector=="||" && !sucs) || (connector==";")){
                if(ogsz >= fromHead){
                    if(pipeLocals.size() > 0){
                        unsigned int k = 0;
                        for(unsigned int i = 0; i < strz.size(); i++){
                            if(pipeLocals.at(k) == i){
                                str[i] = '~';
                                k++;
                                if(k >= pipeLocals.size()){
                                    break;
                                }
                            }
                        }
                    }
                    pch = strtok(str+fromHead, connector.c_str());
                    if(pipeLocals.size() > 0){
                        unsigned int k = 0;
                        for(unsigned int i = 0; i < strz.size(); i++){
                            if(pipeLocals.at(k) == i){
                                str[i] = '|';
                                k++;
                                if(k >= pipeLocals.size()){
                                    break;
                                }
                            }
                        }
                    }
                }
                else{
                    return true;
                }
            }
            //otherwise return
            else{
                return true;
            }
            //if the next command is not NULL and is exit exit the program
            if(pch != NULL && isExit(pch)){
                return false;
            }
    }
    //if there are no more commands to execute/parse return
    return true;
}

string addIOSpaces(string x){
    string ret;
    char spc = ' ';
    bool quo = false;
    unsigned int po = 0;
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) != ' '){
            if(x.at(i) == '"' && !quo){
                ret.push_back(spc);
                quo = true;
                po = i;
            }
            if(quo){
                ret.push_back(x.at(i));
                if(x.at(i) == '"' && i != po){
                    quo = false;
                    po = i;
                    ret.push_back(spc);
                }
                continue;
            }
        if(x.at(i) == '<' || x.at(i) == '>' || x.at(i) == '|'){
            if(i+1 < x.size()){
                ret.push_back(x.at(i));
                if(x.at(i+1) != '>' && x.at(i+1) != ' ' && x.at(i+1) != '|' && x.at(i+1) != '<'){
                    ret.push_back(spc);
                }
                if(x.at(i) == '<'){
                    if(i >= 2){
                        if(x.at(i-2) == '<' && x.at(i-1) == '<'){
                            ret.push_back(spc);
                        }
                    }
                }
            }
            else{
                ret.push_back(x.at(i));
            }
        }
        else{
            if(i+1 < x.size()){
                ret.push_back(x.at(i));
                if(x.at(i+1) == '<' || x.at(i+1) == '>' || x.at(i+1) == '|'){
                    ret.push_back(spc);
                }
            }
            else{
                ret.push_back(x.at(i));
            }
        }
        }
        else{
            ret.push_back(x.at(i));
        }
    }
    return ret;
}

bool io_and_conn(string x){
    unsigned int pipes = 0;
    unsigned int conn = 0;
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) == ';' || x.at(i) == '&'){
            conn++;
        }
        else if(x.at(i) == '|' && i + 1 < x.size()){
            if(x.at(i+1) == '|'){
                conn++;
            }
            else if(i >= 1){
                if(x.at(i-1) == '|'){
                    conn++;
                }
                else{
                    pipes++;
                }
            }
        }
    }
    if(pipes > 0 && conn > 0){
        cout << "Piping with connectors is forbidden" << endl;
        return false;
    }
    else{
        return true;
    }
}

bool onlySpaces(string x){
    for(unsigned int i = 0; i < x.size(); i++){
        if(x.at(i) != ' '){
            return true;
        }
    }
    return false;
}

//main takes in commands and passes them to run to execute
int main(){
    if(signal(SIGINT, handler) == SIG_ERR){
        perror("signal");
	exit(1);
    }
    bool cont = true;
    //continue until terminated by a conditional branch within run
    while(cont){

        //retrieves the login name
        //and checks to make sure there was no error
        char* login = getlogin();
        if(login == NULL){
            perror("getlogin");
        }

        //sets up the host name variable, maximum of 128 chars
        char host[128];

        //holds return value of gethostname
        int hostFlag;

        //sets up input var
        string input;

        //retrieves the host name and checks for errors
        if((hostFlag = gethostname(host, sizeof(host))) == -1){
            perror("gethostname");
        }

        char buf[BUFSIZ];
        if(getcwd(buf, BUFSIZ) == NULL){
            perror("getcwd");
        }
        //if both login and gethostname rerurned without error
        //cout the login@host
        if(login != NULL && hostFlag != -1 && getcwd != NULL){
            cout << login << "@" << host  << ":~" << buf << "$ ";
        }
        //otherwise cout vanilla prompt
        else if(getcwd != NULL){
            cout << buf << "$ ";
        }
        else if(getcwd == NULL && login != NULL && hostFlag != -1){
            cout << login << "@" << host << "$ ";
        }
        else{
            cout << "$ ";
        }
        //retrieve user input including spaces
        getline(cin, input);

        //remove anything that is a comment from the users input
        input = commentRemoval(input);
        input = addIOSpaces(input);
        //check for multiple connectors
        if(!multiConn(input) && io_and_conn(input) && input.size() > 0 && onlySpaces(input)){
            //determines the size of the input
            int input_size = input.size()+1;
            //dynamically allocates a char*[] of the size of the input + 1
            char* str = new char[input_size];
            //copies the input into the char* str[]
            strcpy(str, input.c_str());
            //calls run on the users entered commands
            cont = run(str);
            //after running the dynamically allocated memory is deleted
            delete[] str;
        }
        //if there are more than one connector do not run commands
        //and cerr error
   }
   return 0;
}
