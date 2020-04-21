#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

int random(int min, int max);
void respawnDrop(sf::RectangleShape &drop);
bool checkRainOnTree(sf::RectangleShape drop);
//Default main parameters
    bool fullscreen = false;
    int windowWidth = fullscreen? 1920 : 500;
    int windowHeight = fullscreen? 1080 : 500;
    int grassLevel = 20;
    int rainSize = 100;
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

    // Tree 500x500  

        int treePosition[2];
        int treeSize[2];
        sf::RectangleShape wood(sf::Vector2f(15,100));
        sf::ConvexShape foliage(4);

        if (!fullscreen){
            treeSize[0] = 75;
            treeSize[1] = 150; 
            treePosition[0] = 350; //windowWidth - 2*treeSize[0]; 
            treePosition[1] = 350-grassLevel; //windowHeight - treeSize[1] - grassLevel;
            wood.setFillColor(sf::Color(81,54,26));
            wood.setPosition(380, 400-grassLevel);
            foliage.setPoint(0,sf::Vector2f(treePosition[0], treePosition[1]));
            foliage.setPoint(1,sf::Vector2f(treePosition[0]+treeSize[0], treePosition[1]));
            foliage.setPoint(2,sf::Vector2f(treePosition[0]+treeSize[0], 440-grassLevel));
            foliage.setPoint(3,sf::Vector2f(treePosition[0], 440-grassLevel));
            foliage.setFillColor(sf::Color::Green);
        }

    //Tree fullscreen
        sf::Texture texture;
        sf::Sprite sprite;

        if (fullscreen){
            treeSize[0] = 340;
            treeSize[1] = 390; 
            treePosition[0] = windowWidth - 2*treeSize[0]; //1240;  
            treePosition[1] = windowHeight - treeSize[1] - grassLevel; //690-grassLevel;
            texture.loadFromFile("image/tree.png");
            sprite.setTexture(texture);
            sprite.setPosition(treePosition[0],treePosition[1]);
        };
        int treeEnd = treePosition[0]+treeSize[0];

        

    //Rain
        bool rain = true; 
        int countDrop = 0;
        sf::RectangleShape drop[rainSize];
        int dropSize = fullscreen ? 6:3;
        for(int i = 0; i < rainSize ;i++){
            drop[i].setSize(sf::Vector2f(dropSize,dropSize));
            drop[i].setFillColor(sf::Color::Cyan);
            drop[i].setPosition(random(0,windowWidth),windowHeight);
        };
        sf::Time dropLife[rainSize];
        for(int i = 0; i<rainSize; i++){
            dropLife[i] = sf::milliseconds(random(0,10000)); 
        };

    //Game time
        sf::Clock mainClock;
        sf::Clock clock;
        sf::Clock windClock;

    while (window.isOpen()){ //Main loop

        float time = clock.restart().asMicroseconds();
        time = fullscreen ? time/500 : time/1000 ;
        //Movement of drops
        for(int i = 0; i< rainSize; i++){
            //Respawn drop
            if (mainClock.getElapsedTime() > dropLife[i] ){
                respawnDrop(drop[i]);  
                dropLife[i] += sf::milliseconds(random(6000,15000));
                countDrop++;
            }; 
            //If tree zone
            if ((drop[i].getPosition().x >= treePosition[0])&&
                (drop[i].getPosition().x <= (treeEnd))){
                    //If tree
                    if(checkRainOnTree(drop[i])) {
                        //Move slowly
                        drop[i].move(0+wind/10,0.01*time*random(1, 4));
                    }else if (drop[i].getPosition().y <= windowHeight-grassLevel-dropSize){
                        drop[i].move(0+wind,0.1*time*random(1, 4));
                    };
            }else if (drop[i].getPosition().y <= windowHeight-grassLevel-dropSize){
                drop[i].move(0+wind,0.1*time*random(1, 4)); 
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
            (windClock.getElapsedTime().asSeconds() > 1)){
                windClock.restart();
                wind++;
        };
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left))&&
            (windClock.getElapsedTime().asSeconds() > 1)){
                windClock.restart();
                wind--;
        };
        
        
        window.clear();
        window.draw(grass);
        if(fullscreen) {
            window.draw(sprite);
        }else{
            window.draw(foliage);
            window.draw(wood);
        };
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

int random(int min,int max ){
    return std::rand()%(max-min)+min;
}
void respawnDrop(sf::RectangleShape &drop){
    if (wind == 0){
        drop.setPosition(std::rand()%windowWidth,0);
    }else if(wind > 0){
        drop.setPosition(std::rand()%(windowWidth+wind*150)-wind*150,0);
    }else{
        drop.setPosition(std::rand()%(windowWidth-wind*150),0);        
    }
};

bool checkRainOnTree(sf::RectangleShape drop){
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
