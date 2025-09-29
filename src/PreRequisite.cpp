#include <cstdlib>
#include <iostream>

int main() {
    std::cout << "PreRequisite Check/Install\n";
    //installing module.
    int rc = system("sudo apt update && sudo apt install -y bpftrace");
    if (rc == -1) {
        perror("system");
        return 1;
    }

    // rc contains shell exit status; WEXITSTATUS(rc) gives the command exit code on success
    std::cout << "command exit status: " << rc << std::endl;
    std::cout << "successful\n";
    return 0;
}
