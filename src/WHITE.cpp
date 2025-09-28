#include <iostream>
#include <string>
#include "os.h"
#include "nameSpace.h"

int main(){
  getOs *crOS = currentOS();
  std::cout << "running - > Current OS: " << crOS->os << std::endl;
  delete crOS; // Clean up allocated memory

  sandBox *process;
  std::string path = "/home/kali/Documents/GitHub/WHITE/TestSubject1";
  process->runItBoxed(&path);
  return 0;
}

