#include <cstdlib>
#include <string>

#include <iostream>

class launchDepenency{
  public:

    void installGNOME(std::string pathToMount){
      std::cerr << "\n--- Updating apt inside fakeroot ---";

      std::string upCommand = "sudo chroot "+ pathToMount +" apt-get install xterm";
        int installLauncher = system(upCommand.c_str());
        if(WEXITSTATUS(installLauncher) != 0){
            perror("system");
            exit(1);
        }

      
      std::cerr<<"\n--- Installing gnome-terminal inside fakeroot ---";

      std::string instCommand = "sudo chroot "+ pathToMount + " apt-get install -y gnome-terminal";
      installLauncher = system(instCommand.c_str());
      if(WEXITSTATUS(installLauncher) != 0){
        perror("system");
        exit(1);
      }

      std::cerr << "--- GNOME Terminal installed successfully in fakeroot ---";
    }
};