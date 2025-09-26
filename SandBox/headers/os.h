#include <iostream>

class getOs{
  public: 
  std::string os;
  friend getOs* currentOS();

};

getOs* currentOS(){
  getOs* crOS = new getOs();

  #if defined(_WIN64)
    crOS->os = "WIN64";
  #elif defined(_WIN32)
    crOS->os = "WIN32";
  #elif defined(__linux__)
    crOS->os = "LNX";
  #endif

  return crOS;
}