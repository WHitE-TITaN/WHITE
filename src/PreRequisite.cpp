#include <cstdlib>
#include <iostream>
#include <thread>

//function to install bpftrace to list out all 
//the syscalls that have been main.
void installbpfTrace(){
    int rc = system("sudo apt install -y bpftrace");
    //if command fails to install terminate the program
    if (WEXITSTATUS(rc) != 0) {
        perror("system");
        exit(1);
    }
    std::cout << "command exit status: " << rc << std::endl;

}

//function to install boot directory builder
// for container.
void installBootStrap(){
    int installBoot = system("sudo apt-get install debootstrap");
    if (WEXITSTATUS(installBoot) != 0) {
        perror("system");
        exit(1);
    }

    std::cout << "command exit status: " << installBoot << std::endl;
}

//main starting point
int main() {
    std::cout << "PreRequisite Check/Install\n";
    //installing module.
    
    //sepratily installing modules.
    installbpfTrace();
    installBootStrap();


    // rc contains shell exit status; WEXITSTATUS(rc) gives the command exit code on success
    std::cout << "successful\n";
    return 0;
}
