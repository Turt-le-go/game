#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "include/path.h"
#include "include/Drop.h"


bool checkRainOnTree(Drop drop);
//Default main parameters
    Display display = getDisplaySize();
    bool fullscreen = false;
    int windowWidth = fullscreen? display.width : 500;
    int windowHeight = fullscreen? display.height : 500;
    int grassLevel = 0.0463 * 1080; //0.000043*1080^2
    int rainSize = 100;
    bool rain = true;
//Wind
    int wind = 0;

int main(int argc, char** argv){
    //Settings from the file 
        std::ifstream input;
        input.open(getGameDir() + "/data/settings.txt");
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
            windowWidth = fullscreen? display.width : 500;
            windowHeight = fullscreen? display.height : 500;
            grassLevel = std::stoi(args[1]);
            rainSize = std::stoi(args[2]);    
        };
        input.close();

    //Fullscreen from terminal (on/off)
        if (argc > 1){
            if (atoi(argv[1]) == 1 ){
                fullscreen = true;
                windowWidth = display.width;
                windowHeight = display.height;
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
        sf::Texture textureGrass;
        bool grassInGame = textureGrass.loadFromFile(getGameDir() + "/image/grass.png");
        int grassCount = (grassInGame) ? windowWidth/textureGrass.getSize().x + bool(windowWidth%textureGrass.getSize().x) : 0;
        sf::Sprite grass[grassCount];
        if (!(grassInGame && (grassLevel > 0))){
            grassLevel = 0;
            grassInGame = false;
        };
        for(int i = 0; i < grassCount; i++){
            grass[i].setTexture(textureGrass);
            grass[i].setPosition(i*textureGrass.getSize().x,windowHeight-grassLevel);
        };  

    // Earth 
        sf::Texture textureEarth;
        bool earthInGame = textureEarth.loadFromFile(getGameDir() + "/image/earth.png");
        int earthCount[]{0,0}; 
        if (grassInGame && (grassLevel > textureGrass.getSize().y)){
            if (earthInGame){
                earthCount[0] = (grassLevel - textureGrass.getSize().y)/textureEarth.getSize().y + bool((grassLevel - textureGrass.getSize().y)%textureEarth.getSize().y);
                earthCount[1] = windowWidth/textureEarth.getSize().x + bool(windowWidth%textureEarth.getSize().x);
            }else{
                grassLevel = textureGrass.getSize().y;
            };
        };
        sf::Sprite earth[earthCount[0]][earthCount[1]];
        for(int i = 0; i < earthCount[0]; i++){
            for(int j = 0; j < earthCount[1]; j++){
                earth[i][j].setTexture(textureEarth);
                earth[i][j].setPosition(j*textureEarth.getSize().x, windowHeight-grassLevel + textureGrass.getSize().y + i * textureEarth.getSize().y);
            };
        };

    //Herb
        std::srand(time(0));
        int textureHerbCount = 5;
        sf::Texture textureHerb;
        bool herbInGame = textureHerb.loadFromFile(getGameDir()+"/image/herb.png");
        int herbCount = (herbInGame)? std::rand()%int((windowWidth/(textureHerb.getSize().x/textureHerbCount))/4):0;
        sf::Sprite herb[herbCount];
        for(int i = 0; i< herbCount; i++){
            herb[i].setTexture(textureHerb);
            herb[i].setTextureRect(sf::IntRect((std::rand()%textureHerbCount)*(textureHerb.getSize().x/textureHerbCount),0,textureHerb.getSize().x/textureHerbCount,textureHerb.getSize().y));
            herb[i].setPosition(std::rand()%windowWidth, windowHeight - grassLevel - textureHerb.getSize().y);
        };

    // Tree  
        sf::Texture textureTree;
        sf::Sprite tree;
        int treePosition[2];
        int treeSize[2];
        bool treeInGame;

        if (!fullscreen){
            treeSize[0] = 75;
            treeSize[1] = 150; 
            treeInGame = textureTree.loadFromFile(getGameDir() + "/image/tree0.png");
        }else{
            treeSize[0] = 340;
            treeSize[1] = 390; 
            treeInGame = textureTree.loadFromFile(getGameDir() + "/image/tree1.png");
        };
        treePosition[0] = windowWidth - 2*treeSize[0];
        treePosition[1] = windowHeight - treeSize[1] - grassLevel;
        tree.setTexture(textureTree);
        tree.setPosition(treePosition[0],treePosition[1]);
        int treeEnd = treePosition[0]+treeSize[0];

        

    //Rain 
        int countDrop = 0;
        Drop drop[rainSize];
        int dropSize = fullscreen ? 5:3;
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
                if (treeInGame){
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
            if (earthInGame)
                for(int i = 0; i < earthCount[0]; i++)
                    for(int j = 0; j < earthCount[1]; j++)
                        window.draw(earth[i][j]);

            if (grassInGame) 
                for(int i = 0; i < grassCount; i++) 
                    window.draw(grass[i]);

            if(treeInGame) 
                window.draw(tree);

            if(herbInGame)
                for(int i = 0; i< herbCount; i++)
                    window.draw(herb[i]);

            for(int i = 0; i < rainSize;i++) 
                window.draw(drop[i]);
            window.display();
    };
    //Save information
        std::ofstream output;
        output.open(getGameDir() + "/data/status.txt");
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
