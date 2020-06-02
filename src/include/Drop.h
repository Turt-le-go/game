#pragma once
#include <SFML/Graphics.hpp> 

class Drop : public sf::RectangleShape {
    private:
        float speed;
        sf::Time life;
    public:
        Drop();
        void respawn(bool rain,int wind, int windowWidth, int windowHeight);
        sf::Time getLife();
        float getSpeed() const;
};