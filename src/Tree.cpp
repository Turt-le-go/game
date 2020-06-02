#include "include/Tree.h"

auto Tree::setImage(const std::string& path) -> bool{
    textureIsLoad = texture.loadFromFile(path);
    this->setTexture(texture);
    this->treeSize.width = texture.getSize().x;
    this->treeSize.height = texture.getSize().y;
    return textureIsLoad;
};
void Tree::setRectangle(int x, int y, int width, int height){
    this->rectangles.push_back({x,y,x+width,y+height});
};
auto Tree::getSize() -> size{
    return treeSize;
};
auto Tree::checkRainOnTree(const Drop& drop) -> bool{
    int dropX = drop.getPosition().x;
    int dropY = drop.getPosition().y;
    int treeX = this->getPosition().x;
    int treeY = this->getPosition().y;
    if ((dropX >= treeX)&&(dropX <= (treeX + this->treeSize.width))){
        if(dropY >= treeY){
            for (auto & rectangle : rectangles){
                if(( dropX >= treeX+rectangle[0])&&
                   ( dropX <= treeX+rectangle[2])&&
                   ( dropY >= treeY+rectangle[1])&&
                   ( dropY <= treeY+rectangle[3])) {
                    return true;
                };
            };
        };
    };
    return false;
};