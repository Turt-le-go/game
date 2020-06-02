#include "include/settings.h"

auto getPath() -> std::string{
    std::string path;
    path.resize(512);
    int ret = readlink("/proc/self/exe", &path[0], path.size()); 
    path.resize(ret);
    return path;
};

auto getGameDir() -> std::string{
    std::string path = getPath();
    path = path.substr(0,path.rfind('/'));
    return path + "/";
};

auto getDisplaySize() -> Display{
    Display result = {0,0};

    system(R"(xdpyinfo | grep dimensions | egrep -o "[0-9]+x[0-9]+ pixels" | egrep -o "[0-9]+x[0-9]+" > tmp)");

    std::ifstream tmp;
    tmp.open("tmp");
    if(tmp.is_open()){ 
        char c = 0;
        bool flag = false;
        std::string args[2];
        int i = 0;
        while(tmp.get(c)){
            if ((int(c) >= int('0'))&&(int(c) <= int('9'))){
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

auto Settings::readSettingsFromFile(const std::string& path) -> bool{
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
            int n = line.find('#');
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

auto Settings::getSetting(const std::string& name) -> int{
    int start=this->file.find(name);
    if(start == -1){ 
        return -1;
        //error
    };
    int end = this->file.find('\n',start);
    return std::stoi(this->file.substr(start+name.size()+1,end));
};
