#include "include/Tree.h"

bool Tree::setImage(std::string path){
    textureIsLoad = texture.loadFromFile(path);
    this->setTexture(texture);
    this->treeSize.width = texture.getSize().x;
    this->treeSize.height = texture.getSize().y;
    return textureIsLoad;
};
void Tree::setRectangle(int x, int y, int width, int height){
    this->rectangle.push_back({x,y,x+width,y+height});
};
size Tree::getSize(){
    return treeSize;
};
bool Tree::checkRainOnTree(Drop drop){
    int dropX = drop.getPosition().x;
    int dropY = drop.getPosition().y;
    int treeX = this->getPosition().x;
    int treeY = this->getPosition().y;
    if ((dropX >= treeX)&&(dropX <= (treeX + this->treeSize.width))){
        if(dropY >= treeY){
            for (int i = 0 ; i < rectangle.size(); i++){
                if(( dropX >= treeX+rectangle[i][0])&&( dropX <= treeX+rectangle[i][2])&&( dropY >= treeY+rectangle[i][1])&&( dropY <= treeY+rectangle[i][3]))
                    return true;
            };
        };
    };
    return false;
};