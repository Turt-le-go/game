#include "include/Drop.h"

Drop::Drop(){
    this->setFillColor(sf::Color::Cyan);
    this->life = sf::milliseconds(std::rand()%10000);
    speed = 0.08*(std::rand()%3+3);
};
void Drop::respawn(bool rain,int wind, int windowWidth, int windowHeight){
    if(rain){
        if (wind == 0){
            this->setPosition(std::rand()%windowWidth,0);
        }else if(wind > 0){
            this->setPosition(std::rand()%(windowWidth+wind*150)-wind*150,0);
        }else{
            this->setPosition(std::rand()%(windowWidth-wind*150),0);
        };     
    }else{
        this->setPosition(std::rand()%windowWidth,windowHeight);
    };
    this->life += sf::milliseconds(std::rand()%9000+6000);
};
sf::Time Drop::getLife(){return this->life;}

float Drop::getSpeed(){return this->speed;}
