#pragma once

#include <SFML/Graphics.hpp>
#include "Drop.h"
#include <vector>
#include <string>
#include <array>

struct size {
    int width;
    int height;
};

class Tree : public sf::Sprite {
    private:
        bool textureIsLoad = false;
        sf::Texture texture;
        size treeSize{0,0};
        std::vector<std::array<int, 4>> rectangles;
    public:
        bool setImage(const std::string& path);
        void setRectangle(int x, int y, int width, int height);
        size getSize();
        bool checkRainOnTree(const Drop& drop);
};