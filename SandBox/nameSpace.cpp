#include <sched.h>      // for clone(), CLONE_* flags
#include <sys/mount.h>  // for mount()
#include <unistd.h>     // for execvp()
#include <signal.h>     // for SIGCHLD
#include <string.h>     // for strerror
#include <stdlib.h>     // for exit
#include <iostream>

//memory for sandBox.
#define STACK_SIZE (1024 * 1024); //1024byte = 1 kb -> 1024 * 1024 = 1048576byte = 1024kb = 1MB.


//to create SandBox
int createNameSpace(std::string* path){
    return 1;
}


//to execute program in sandBox 
int runItBoxed(std::string* path){
//execl is used for executing a program
    std::cout<<"pid - > "<<getpid();

    execl((char *)path, NULL);
    perror("execl");
   
    return 1;
}

int main(){
    std::string path = "/home/kali/Documents/GitHub/WHITE/TestSubject1";
    std::string *pathPnt = &path;
    runItBoxed(pathPnt);
    return 0;
}