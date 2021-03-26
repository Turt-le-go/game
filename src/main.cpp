//include
    #include "include/Drop.h"
    #include "include/Tree.h"
    #include "include/settings.h"
    #include <SFML/Graphics.hpp>
    #include <fstream>
    #include <iostream>
    #include <string>

auto main(int argc, char** argv) -> int{
    //Display
        Display display = getDisplaySize();
        bool fullscreen = true;
        int windowWidth = fullscreen? display.width : 500;
        int windowHeight = fullscreen? display.height : 500;

    //Settings from the file 
        Settings settings;
        settings.readSettingsFromFile(getGameDir()+"data/settings.txt");
        
    //Fullscreen from terminal (on/off)
        if (argc > 1){
            if (std::stoi(argv[1]) == 1 ){
                fullscreen = true;
                windowWidth = display.width;
                windowHeight = display.height;
            }else if (std::stoi(argv[1]) == 0 ){
                fullscreen = false;
                windowWidth = 500;
                windowHeight = 500;
            }
        };

    //Create window
        sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), "Game :)");
        if (!fullscreen){
            window.setPosition(sf::Vector2i(300,200));
        };
        window.setFramerateLimit(60);

    //Grass

        int earthLevel = settings.isLoaded ? settings.getSetting("Earth level"): 50;
        sf::Texture textureGrass;
        bool grassInGame = textureGrass.loadFromFile(getGameDir() + "image/grass.png");
        int grassCount = (grassInGame) ? windowWidth/textureGrass.getSize().x + bool(windowWidth%textureGrass.getSize().x) : 0;
        sf::Sprite grass[grassCount];
        if (!(grassInGame && (earthLevel > 0))){
            earthLevel = 0;
            grassInGame = false;
        };
        for(int i = 0; i < grassCount; i++){
            grass[i].setTexture(textureGrass);
            grass[i].setPosition(i*textureGrass.getSize().x,windowHeight-earthLevel);
        };  

    //Earth 
        sf::Texture textureEarth;
        bool earthInGame = textureEarth.loadFromFile(getGameDir() + "image/earth.png");
        int earthCount[]{0,0}; 
        if (grassInGame && (earthLevel > textureGrass.getSize().y)){
            if (earthInGame){
                earthCount[0] = (earthLevel - textureGrass.getSize().y)/textureEarth.getSize().y + bool((earthLevel - textureGrass.getSize().y)%textureEarth.getSize().y);
                earthCount[1] = windowWidth/textureEarth.getSize().x + bool(windowWidth%textureEarth.getSize().x);
            }else{
                earthLevel = textureGrass.getSize().y;
            };
        };
        sf::Sprite earth[earthCount[0]][earthCount[1]];
        for(int i = 0; i < earthCount[0]; i++){
            for(int j = 0; j < earthCount[1]; j++){
                earth[i][j].setTexture(textureEarth);
                earth[i][j].setPosition(j*textureEarth.getSize().x, windowHeight-earthLevel + textureGrass.getSize().y + i * textureEarth.getSize().y);
            };
        };

    //Herb
        std::srand(time(0));
        int textureHerbCount = 5;
        sf::Texture textureHerb;
        bool herbInGame = textureHerb.loadFromFile(getGameDir()+"image/herb.png");
        int herbCount = (herbInGame)? std::rand()%int((windowWidth/(textureHerb.getSize().x/textureHerbCount))/4):0;
        sf::Sprite herb[herbCount];
        for(int i = 0; i< herbCount; i++){
            herb[i].setTexture(textureHerb);
            herb[i].setTextureRect(sf::IntRect((std::rand()%textureHerbCount)*(textureHerb.getSize().x/textureHerbCount),0,textureHerb.getSize().x/textureHerbCount,textureHerb.getSize().y));
            herb[i].setPosition(std::rand()%windowWidth, windowHeight - earthLevel - textureHerb.getSize().y);
        };

    //Tree  
        Tree tree;
        bool treeInGame;
        if (!fullscreen){
            treeInGame = tree.setImage(getGameDir() + "image/tree0.png");
            tree.setRectangle(0,0,75,90);
            tree.setRectangle(30,90,15,60);
        }else{
            treeInGame = tree.setImage(getGameDir() + "image/tree1.png");
            tree.setRectangle(0,20,140,150);
            tree.setRectangle(160,0,180,150);
            tree.setRectangle(40,170,70,50);
            tree.setRectangle(190,150,100,50);
            tree.setRectangle(130,200,60,190);
        };
        tree.setPosition(std::rand()%(windowWidth-tree.getSize().width),windowHeight-earthLevel-tree.getSize().height);
        
    //Rain 
        bool rain = settings.isLoaded ? settings.getSetting("Rain") : true;
        int rainSize = settings.isLoaded ? settings.getSetting("Rain power") : 100;
        int countDrop = 0;
        Drop drop[rainSize];
        int dropSize = fullscreen ? 5:3;
        for(int i = 0; i < rainSize ;i++){
            drop[i].setSize(sf::Vector2f(dropSize,dropSize));
            drop[i].setPosition( std::rand()%windowWidth ,windowHeight);
        };

    //Wind
        int wind = 0;

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
                if ((treeInGame)&&(tree.checkRainOnTree(drop[i]))){
                        //Move slowly
                        drop[i].move(0+wind/10,0.1*time*drop[i].getSpeed());
                }else if (drop[i].getPosition().y <= windowHeight-earthLevel-dropSize){
                    drop[i].move(0+wind,time*drop[i].getSpeed()); 
                };
            };

        //Close game
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
}
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
                    rain = !rain;
            };
        
        //Draw objects    
            window.clear();
            if (earthInGame) {
                for(int i = 0; i < earthCount[0]; i++) {
                    for(int j = 0; j < earthCount[1]; j++) {
                        window.draw(earth[i][j]);
            }}};
            if (grassInGame) { 
                for(int i = 0; i < grassCount; i++) { 
                    window.draw(grass[i]);
            }};
            if(treeInGame) { 
                window.draw(tree);
            };
            if(herbInGame) {
                for(int i = 0; i< herbCount; i++) {
                    window.draw(herb[i]);
            }};
            for(int i = 0; i < rainSize;i++) { 
                window.draw(drop[i]);
            };
            window.display();
    };
    //Save information
        std::ofstream output;
        output.open(getGameDir() + "data/status.txt");
        output<< "Всього випало крапель дощу: "<<countDrop <<'\n';
        output.close();

    return 0;
}
