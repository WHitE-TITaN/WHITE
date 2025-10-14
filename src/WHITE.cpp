#include <iostream>
#include <string>
#include "os.h"
#include <limits.h>

//main isoloation program
#include "nameSpace.h"

//for installation of dependency in fake boot.
#include "launchDependency.h"

//for system calls
#include <cstdlib>
//to check pth
#include <filesystem>

//to sorten the std::filesystem -> fs
namespace fs = std::filesystem;




std::string getPath(){
  //defined max path length
   char buffer[PATH_MAX];
   //read what is the location of the binnary or where it is being executed.
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1) {
        perror("readlink");
        exit(1);
    }
    //read buffer and return path
    buffer[len] = '\0';
    std::string fullPath(buffer);
    return fullPath.substr(0, fullPath.find_last_of('/'));
}




//checking files and previous root builds
bool setupDirectoryAndDebootstrap(const fs::path& dir){
  //check if the directory exsist
  if(fs::is_directory(dir)){
    std::cout<<"\nDirectory exsist : "<<dir;
    return true;
  }

  std::cout<<"\nDirectory not found \nBuilding Boot";
  try{
    //create_directories() creates directory in correct order.
    fs::create_directories(dir);
    std::cout<<"\nDirectory building completed : "<<dir;
  }
  catch(fs::filesystem_error& e){
    //print error if fail to build dir.
    std::cout<<"\nError : please reffer\n"<<e.what();
    //rasing exception for main higher function to handle.
    throw std::runtime_error("Fail to build Dir check setupDirectoryAndDebootstrap() : WHITE.cpp");
  }

  //building command for bootstrap executaion.
  std::cout<<"\nRunning BootStrap :";
  std::string bootStrapStart = "sudo debootstrap stable " + dir.string() + " http://deb.debian.org/debian/";

  //running sysrtem command
  int returnCode = system(bootStrapStart.c_str());
  //processing results.
  if(returnCode == 0){
    std::cout<<"\nBootStrap Build Successfull";
    return true;
  }

  std::cerr<<"\nBootStrap Build Fail";
  //rasing exception for higher function ro handle.
  throw std::runtime_error("BootStrap Fails check setupDirectoryAndDebootstrap() : WHITE.cpp");
}




//installing prequisite before execution of sandBox.
void installPreRequisite(){
  //installing prerequisites
  //fork to copy pid from parentproces
  // > 0 goes to child
  // = 0 its the child
  // < 0 failed
  pid_t child = fork();
  if(child == -1){
    perror("fork");
    exit(1);
  }
  //if child process instal the dependecy.
  if(child == 0){
    std::string preRequisite = getPath()+"/PreRequisite";
    std::cout<<"\n"+preRequisite+"\n";
    int installing = execl(preRequisite.c_str(), preRequisite.c_str(), NULL);
    //if crash or fails log issue.
    perror("execl");
  }

  else{
    //saves the state the program currently is.
    int status;
    //wait for the parent to be exercuted.
    if (waitpid(child, &status, 0) == -1) {
      perror("waitpid failed");
      exit(1);
    }
    //if the parent have exited before executing the child
    if(WIFEXITED(status)){
      //check for the exeit code if killed or some other issue.
      int exitCode = WEXITSTATUS(status);
      if (exitCode != 0) {
        std::cerr << "Prerequisite binary exited with error: " << exitCode << "\n";
        exit(1);
      }
    
    //uif the program crash with some error.
    } else {
      std::cerr << "Prerequisite binary crashed or was killed\n";
      exit(1);
    }
  }
}





//main code flow ->
int main(){
  //getting the current os version
  getOs *crOS = currentOS();
  std::cout << "running - > Current OS: " << crOS->os << std::endl;
  delete crOS; // Clean up allocated memory

  //install dependency
  installPreRequisite();
  std::cout.flush();

  //Boot Check and creation minimal boot.
  std::string directory = getPath()+"/mountMask";
  fs::path bootDirectory = fs::path(directory);

  std::cerr<<"using boot from ->"<<bootDirectory;
  

  //setup main fake Directory
  try{
    setupDirectoryAndDebootstrap(bootDirectory);
    
    //installing GNOME
    launchDepenency install;
    install.installGNOME(bootDirectory);

  }

  //if fail to build exit program.
  catch(std::exception& e){
    std::cerr<<"\nError ! reffer : "<<e.what();
    exit(1);
  }



  try{
    // Create the target directory inside the fakeroot for the X11 socket
    fs::create_directories(bootDirectory / "tmp" / ".X11-unix");

    // mount the host's X11 socket into the sandbox
    std::string mount_x11_cmd = "sudo mount --bind /tmp/.X11-unix " + (bootDirectory / "tmp" / ".X11-unix").string();

    if (system(mount_x11_cmd.c_str()) != 0) {
      std::cerr << "\nFailed to mount X11 socket!" << std::endl;
      exit(1);
    }
  }
  catch(std::exception& e){
    std::cerr<<"\nError ! reffer : "<<e.what();
    exit(1);
  }


  

  sandBox process;
  std::string path = "/bin/bash";

  //storing paths
  mountDirectory newDirectories;
  newDirectories.minimalMount = directory.c_str();
  newDirectories.programToRun = path.c_str();


  //creating isolated process
  process.createNameSpace(&newDirectories);
  return 0;
}

