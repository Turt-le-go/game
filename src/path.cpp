#include "include/path.h"

std::string getPath(){
    std::string path;
    path.resize(512);
    int ret = readlink("/proc/self/exe", &path[0], path.size()); // &path[0] - не const char*
    path.resize(ret);
    return path;
};

std::string  getGameDir(){
    std::string path = getPath();
    
    path = path.substr(0,path.rfind("/bin/"));
    return path;
};