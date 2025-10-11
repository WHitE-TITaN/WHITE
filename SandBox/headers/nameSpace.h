#include <sched.h>      // for clone(), CLONE_* flags
#include <unistd.h>     // for execvp()
#include <signal.h>     // for SIGCHLD

#include <string.h>     // for strerror

#include <stdlib.h>     // for exit

#include <iostream>

#include <sys/wait.h>   
#include <sys/mount.h>  // for mount()
#include <sys/stat.h>   //for mkdir() commands


//structure to hold the directory paths
struct mountDirectory{
    const char* minimalMount;
    const char* programToRun;
};

class sandBox{
    public:
        //to execute program in sandBox 
        static int runItBoxed(void* args){
            //getpid() will featch the process id that is being executed.
            std::cout<<"\npid - > "<<getpid();
            std::cout.flush(); //send print buffer to the terminal immidatly as execel will replace the
            //current execution flow.
            
            //casting arguments into class obj;
            mountDirectory* newMask = static_cast<mountDirectory*>(args);

            //Remounting the fileSystem For Process.
            std::cout<<"\nremount the root filesystem (/) as a private mount";
            if(mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1){
                //MS_REC | MS_PRIVATE means recursively remount / and all its submounts as private, so mount events won’t propagate to other namespaces.
                perror("mount MS_PRIVATE");
                return 1;
            }

            
            //changes the root directory of the process to "mountMask/root"
            if(chroot(newMask->minimalMount) == -1){
                //For chroot to work, the directory must exist and be a valid root environment
                //so it will require libraries, /proc, etc.
                perror("chroot");
                return 1;
            }

            //change directory to / inside the new root.
            if(chdir("/") == -1){
                perror("chdir");
                return 1;
            }
            
            //proc mount point before mounting
            mkdir("/proc", 0555); // Permissions: read/execute for all

             //Mounts the proc filesystem inside the new root at /proc
            if(mount("proc", "/proc", "proc", 0, NULL) != 0){
                perror("mount /proc");
                return -1;
            }

            //execl is used for executing a program
            //and return -1 if program is fail to run
            //perror() tells the error why the program failed.
            int returns = execl(newMask->programToRun, newMask->programToRun, NULL);
            perror("execl");

            return 1;
        }



        //to create SandBox
        int createNameSpace(void* args){

            //converting
            mountDirectory* newMask = static_cast<mountDirectory*>(args);

            //allocating stack.
            #define STACK_SIZE (1024 * 1024) //1024byte = 1 kb -> 1024 * 1024 = 1048576byte = 1024kb = 1MB.
            void* child_stack = malloc(STACK_SIZE);

            //CLONE_NEWPID create a new pi for child and sandbox it so it cant see any other process
            //but a real PID is also attached that is visible to Parent process
            //SIGCHILD = is a flag that let Parent recive signal form child Process.
            int flag = CLONE_NEWPID | SIGCHLD | CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWNS;

            //pid_t is a signed integer that represent process id in linux and macos
            //clone() returns a real pid for the child process but due to CLONE_NEWPID flag child process only sees it self.
            pid_t child = clone(runItBoxed, (char*) child_stack + STACK_SIZE, flag, newMask);
            std::cout<<"\nchild pid -> "<<child;

            if(child == -1){
                perror("clone");
                exit(EXIT_FAILURE);
            }

            //attach the logger to log all the activity the child do -> bpftrace
            //track the hole system.
            //track specific PID

            pid_t tracer = fork();
            if(tracer == -1){
                std::cerr<<"fail to start bptrace";
                perror("fork");
                throw std::runtime_error("Launch Fail");
            }
            else if(tracer == 0){
                std::cerr<<"\nsub process id - "<<tracer;

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

            waitpid(child, NULL, 0);
            return 0;            
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