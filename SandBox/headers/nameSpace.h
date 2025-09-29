#include <sched.h>      // for clone(), CLONE_* flags
#include <sys/mount.h>  // for mount()
#include <unistd.h>     // for execvp()
#include <signal.h>     // for SIGCHLD
#include <string.h>     // for strerror
#include <stdlib.h>     // for exit
#include <iostream>
#include <sys/wait.h>

class sandBox{
    public:
        //to execute program in sandBox 
        static int runItBoxed(void* args){
            //getpid() will featch the process id that is being executed.
            std::cout<<"pid - > "<<getpid();
            std::cout.flush(); //send print buffer to the terminal immidatly as execel will replace the
            //current execution flow.

            //convert arg to valid string path as execl required a char* 
            const char * path = static_cast<const char*>(args);

            //execl is used for executing a program
            //and return -1 if program is fail to run
            //perror() tells the error why the program failed.
            int returns = execl(path, path, NULL);
            perror("execl");

            return 1;
        }

        //to create SandBox
        int createNameSpace(const char* path){
            //allocating stack.
            #define STACK_SIZE (1024 * 1024) //1024byte = 1 kb -> 1024 * 1024 = 1048576byte = 1024kb = 1MB.
            void* child_stack = malloc(STACK_SIZE);

            //CLONE_NEWPID create a new pi for child and sandbox it so it cant see any other process
            //but a real PID is also attached that is visible to Parent process
            //SIGCHILD = is a flag that let Parent recive signal form child Process.
            int flag = CLONE_NEWPID | SIGCHLD;

            //pid_t is a signed integer that represent process id in linux and macos
            //clone() returns a real pid for the child process but due to CLONE_NEWPID flag child process only sees it self.
            pid_t child = clone(runItBoxed, (char*) child_stack + STACK_SIZE, flag, (void*)path);
            std::cout<<"child pid -> "<<child;

            if(child == -1){
                perror("clone");
                exit(EXIT_FAILURE);
            }

            //attach the logger to log all the activity the child do -> bpftrace
            //track the hole system.
            // int tacker = execl("/bin/bash", "/bin/bash", "-c", 
            //     "sudo bpftrace -e \"tracepoint:syscalls:sys_enter_openat { "
            //     "printf(\\\"%s opened %s\\n\\\", comm, str(args->filename)); }\"",
            //     NULL);

            //track specific PID
            std::string trackCMD =  
                "sudo bpftrace -e \"tracepoint:syscalls:sys_enter_openat /pid == " + std::to_string(child) + "/ { "
                "printf(\\\"%s opened %s\\n\\\", comm, str(args->filename)); }\"";
            //execute the track command on new bash
            int tacker = execl("/bin/bash", "/bin/bash", "-c",
                trackCMD.c_str(),
                NULL);

            perror("execl");

            //to wait until the executioon of child is finished
            waitpid(child, NULL, 0);
            return 1;
        }

    private:
        void trackSystem(){
            //create a subprocess that log every process detail.
            int tacker = execl("/bin/bash", "/bin/bash", "-c", 
                "sudo bpftrace -e \"tracepoint:syscalls:sys_enter_openat { "
                "printf(\\\"%s opened %s\\n\\\", comm, str(args->filename)); }\"",
                NULL);

            perror("execl");
        }
};