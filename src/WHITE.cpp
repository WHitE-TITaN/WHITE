#include <iostream>
#include <string>
#include "os.h"

int main(){
  getOs *crOS = currentOS();
  std::cout << "running - > Current OS: " << crOS->os << std::endl;
  delete crOS; // Clean up allocated memory
  return 0;
}