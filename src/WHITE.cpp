#include <iostream>
#include <string>
#include "os.h"
#include <limits.h>
#include "nameSpace.h"

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

  sandBox process;
  std::string path = "/home/kali/Documents/GitHub/WHITE/TestSubject1";
  process.createNameSpace(path.c_str());
  return 0;
}

