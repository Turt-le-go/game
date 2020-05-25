#include "include/settings.h"

std::string getPath(){
    std::string path;
    path.resize(512);
    int ret = readlink("/proc/self/exe", &path[0], path.size()); 
    path.resize(ret);
    return path;
};

std::string  getGameDir(){
    std::string path = getPath();
    path = path.substr(0,path.rfind("/"));
    return path + "/";
};

Display getDisplaySize(){
    Display result = {0,0};

    system("xdpyinfo | grep dimensions | egrep -o \"[0-9]+x[0-9]+ pixels\" | egrep -o \"[0-9]+x[0-9]+\" > tmp");

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

Settings::Settings(){};

bool Settings::readSettingsFromFile(std::string path){
    std::fstream file;
    file.open(path);
    if(file.is_open()){
        this->isLoaded = true;
        this->file.clear();
        char l[256];
        std::string line;
        while(!file.eof()){
            line.clear();
            file.getline(l, 256);
            line = l;
            int n = line.find("#");
            if (n != std::string::npos){
                line = line.substr(0,n);
            };
            this->file += line +'\n';
        };
        file.close();
        return true;
    };
    file.close();
    return false;
};

int Settings::getSetting(std::string name){
    int start=this->file.find(name);
    if(start == -1){ 
        return -1;
        //error
    };
    int end = this->file.find('\n',start);
    return std::stoi(this->file.substr(start+name.size()+1,end));
};
