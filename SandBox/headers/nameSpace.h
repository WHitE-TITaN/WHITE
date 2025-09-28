#include <sched.h>      // for clone(), CLONE_* flags
#include <sys/mount.h>  // for mount()
#include <unistd.h>     // for execvp()
#include <signal.h>     // for SIGCHLD
#include <string.h>     // for strerror
#include <stdlib.h>     // for exit
#include <iostream>

class sandBox{
    //memory for sandBox.
    #define STACK_SIZE (1024 * 1024); //1024byte = 1 kb -> 1024 * 1024 = 1048576byte = 1024kb = 1MB.

    public:
    int createNameSpace(std::string* path);
    int runItBoxed(std::string* path);
    int runItBoxed(std::string* path, std::string* args);
};


//to create SandBox
int sandBox::createNameSpace(std::string* path){
    return 1;
}


//to execute program in sandBox 
int sandBox::runItBoxed(std::string* path){
    //getpid() will featch the process id that is being executed.
    std::cout<<"pid - > "<<getpid();
    std::cout.flush(); //send print buffer to the terminal immidatly as execel will replace the
    //current execution flow. 

    
    const char * programPath = path->c_str();

    //execl is used for executing a program
    //and return -1 if program is fail to run
    //perror() tells the error why the program failed.
    int returns = execl(programPath, programPath, NULL);
    if(returns == -1)perror("execl");

    return 1;
}


//sand box with arguments
int sandBox::runItBoxed(std::string *path, std::string* args){
    int returns = execl(path->c_str(), path->c_str(), "-c", args->c_str(), NULL);
    if(returns == -1) perror("execl");
    return 1;
}


