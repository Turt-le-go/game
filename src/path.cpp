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
    
    path = path.substr(0,path.rfind("/"));
    return path;
};


Display getDisplaySize(){
    Display result = {0,0};

    system("xdpyinfo -display :0.0 | grep dimensions | egrep -o \"[0-9]+x[0-9]+ pixels\" | egrep -o \"[0-9]+x[0-9]+\" > tmp");

    std::ifstream tmp;
    tmp.open("tmp");
    if(tmp.is_open()){ 
        char c;
        bool flag = false;
        std::string args[2];
        int i = 0;
        while(tmp.get(c)){
            if ((int(c) >= 48)&&(int(c) <= 57)){
                args[i] += c;
                flag = true;
            }else if (flag){
                flag = false;
                i++;
            };
        };
    result = {std::stoi(args[0]) , std::stoi(args[1]) };
    };
    tmp.close();
    system("rm tmp");
    return result;
};