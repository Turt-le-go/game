#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>


std::string getPath();

std::string getGameDir();

struct Display{
    int width;
    int height;
};

Display getDisplaySize();

class Settings{
    private:
        std::string file;
    public:
        bool isLoaded = false;
        Settings();
        bool readSettingsFromFile(const std::string& path);
        int getSetting(const std::string& name);
};