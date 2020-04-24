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
