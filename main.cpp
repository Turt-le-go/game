#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

class Drop : public sf::RectangleShape {
    private:
        float speed;
        sf::Time life;
    public:

        Drop(){
            this->setFillColor(sf::Color::Cyan);
            this->life = sf::milliseconds(std::rand()%10000);
            speed = 0.08*(std::rand()%3+3);
        };
        void respawn(bool rain,int wind, int windowWidth, int windowHeight){
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
        sf::Time getLife(){
            return this->life;
        };
        float getSpeed(){
            return this->speed;
        };

};
bool checkRainOnTree(Drop drop);
//Default main parameters
    bool fullscreen = false;
    int windowWidth = fullscreen? 1920 : 500;
    int windowHeight = fullscreen? 1080 : 500;
    int grassLevel = 20;
    int rainSize = 100;
    bool rain = true;
//Wind
    int wind = 0;

int main(int argc, char** argv){
    //Settings from the file 
        std::ifstream input;
        input.open("settings.txt");
        if(input.is_open()){ 
            char c;
            bool flag = false;
            std::string args[3];
            int i = 0;
            while(input.get(c)){
                if ((int(c) >= 48)&&(int(c) <= 57)){
                    args[i] += c;
                    flag = true;
                }else if (flag){
                    flag = false;
                    i++;
                };
            };
            fullscreen = std::stoi(args[0]);
            windowWidth = fullscreen? 1920 : 500;
            windowHeight = fullscreen? 1080 : 500;
            grassLevel = std::stoi(args[1]);
            rainSize = std::stoi(args[2]);    
        };
        input.close();

    //Fullscreen from terminal (on/off)
        if (argc > 1){
            if (atoi(argv[1]) == 1 ){
                fullscreen = true;
                windowWidth = 1920;
                windowHeight = 1080;
            }else if (atoi(argv[1]) == 0 ){
                fullscreen = false;
                windowWidth = 500;
                windowHeight = 500;
            }
        };

    //Create window
        sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), "Game :)",(fullscreen?  sf::Style::Fullscreen : sf::Style::Close ) );
        if (!fullscreen){
            window.setPosition(sf::Vector2i(300,200));
        };
        window.setFramerateLimit(60);

    // Grass
        sf::RectangleShape grass(sf::Vector2f(windowWidth,grassLevel));
        grass.setFillColor(sf::Color(0,255,0));
        grass.setPosition(0,windowHeight-grassLevel );

    // Tree  
        sf::Texture texture;
        sf::Sprite sprite;
        int treePosition[2];
        int treeSize[2];

        if (!fullscreen){
            treeSize[0] = 75;
            treeSize[1] = 150; 
            texture.loadFromFile("image/tree0.png");
        }else{
            treeSize[0] = 340;
            treeSize[1] = 390; 
            texture.loadFromFile("image/tree1.png");
        };
        treePosition[0] = windowWidth - 2*treeSize[0];
        treePosition[1] = windowHeight - treeSize[1] - grassLevel;
        sprite.setTexture(texture);
        sprite.setPosition(treePosition[0],treePosition[1]);
        int treeEnd = treePosition[0]+treeSize[0];

        

    //Rain 
        int countDrop = 0;
        Drop drop[rainSize];
        int dropSize = fullscreen ? 6:3;
        for(int i = 0; i < rainSize ;i++){
            drop[i].setSize(sf::Vector2f(dropSize,dropSize));
            drop[i].setPosition( std::rand()%windowWidth ,windowHeight);
        };

    //Game time
        sf::Clock mainClock;
        sf::Clock clock;
        sf::Clock onOffClock;
    while (window.isOpen()){ //Main loop

        float time = clock.restart().asMicroseconds();
        time = fullscreen ? time/500 : time/1000 ;
    
        //Movement of drops
            for(int i = 0; i < rainSize; i++){
                //Respawn drop
                if (mainClock.getElapsedTime() > drop[i].getLife()){
                    drop[i].respawn(rain, wind, windowWidth, windowHeight);
                    countDrop++;
                }; 
                //If tree zone
                if ((drop[i].getPosition().x >= treePosition[0])&&
                    (drop[i].getPosition().x <= (treeEnd))){
                        //If tree
                        if(checkRainOnTree(drop[i])) {
                            //Move slowly
                            drop[i].move(0+wind/10,0.1*time*drop[i].getSpeed());
                        }else if (drop[i].getPosition().y <= windowHeight-grassLevel-dropSize){
                            drop[i].move(0+wind,time*drop[i].getSpeed());
                        };
                }else if (drop[i].getPosition().y <= windowHeight-grassLevel-dropSize){
                    drop[i].move(0+wind,time*drop[i].getSpeed()); 
                };
            };

        //Close game
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            };
            //Close game secret combination(Esc+End)
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))&&
                (sf::Keyboard::isKeyPressed(sf::Keyboard::End))){
                    window.close();
            };
        //Wind is blowing?
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right))&&
                (onOffClock.getElapsedTime().asSeconds() > 0.25)){
                    onOffClock.restart();
                    wind++;
            };
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left))&&
                (onOffClock.getElapsedTime().asSeconds() > 0.25)){
                    onOffClock.restart();
                    wind--;
            };
        //Rain?
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::R))&&
                (onOffClock.getElapsedTime().asSeconds() > 0.25)){
                    onOffClock.restart();
                    rain = (rain ? false :true);
            };
        
        //Draw objects    
            window.clear();
            window.draw(grass);
            window.draw(sprite);
            for(int i = 0; i < rainSize;i++) window.draw(drop[i]);
            window.display();
    };
    //Save information
        std::ofstream output;
        output.open("status.txt");
        output<< "Всього випало крапель дощу: "<<countDrop <<'\n';
        output.close();

    return 0;
}

bool checkRainOnTree(Drop drop){
    if(!fullscreen){
        int x = drop.getPosition().x;
        int y = drop.getPosition().y;
        return ((( x >= 350)&&
                ( x <= 425)&&
                ( y <= 440-grassLevel)&&
                ( y >= 350-grassLevel))||
                (( x >= 380)&&
                ( x <= 394)&&
                ( y >= 440-grassLevel)&&
                ( y <= 497-grassLevel)));
    }else if( drop.getPosition().y > 690-grassLevel){
        int x = drop.getPosition().x;
        int y = drop.getPosition().y;
        if(( x >= 1240)&&( x <= 1377)&&( y >= 710-grassLevel)&&( y <= 860-grassLevel))
            return true;
        if(( x >= 1400)&&( x <= 1577)&&( y >= 690-grassLevel)&&( y <= 840-grassLevel))
            return true;
        if(( x >= 1430)&&( x <= 1527)&&( y >= 840-grassLevel)&&( y <= 890-grassLevel))
            return true;
        if(( x >= 1280)&&( x <= 1347)&&( y >= 860-grassLevel)&&( y <= 910-grassLevel))
            return true;
        if(( x >= 1370)&&( x <= 1427)&&( y >= 890-grassLevel)&&( y <= 1080-grassLevel))
            return true;
    };
    return false;
};
