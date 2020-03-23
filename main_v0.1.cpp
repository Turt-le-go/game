#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

int random(int min, int max);
sf::Time set_drop_life();

int main(){

    //Default main parameters
        int window_widht =  500;
        int window_height =  500;
        int grass_level = 20;
        int rain_size = 100;

    //Settings from the file 
        std::ifstream input;
        input.open("settings.txt");
        if(input.is_open()){ 
            char c;
            bool flag = false;
            std::string args[2];
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
            grass_level = std::stoi(args[0]);
            rain_size = std::stoi(args[1]);    
        };
        input.close();



    //Create window
        sf::RenderWindow window(sf::VideoMode(window_widht,window_height), "Game :)",sf::Style::Close);
        window.setPosition(sf::Vector2i(300,200));
        window.setFramerateLimit(60);

    // Grass
        sf::RectangleShape grass(sf::Vector2f(window_widht,grass_level));
        grass.setFillColor(sf::Color(0,255,0));
        grass.setPosition(0,window_height-grass_level );

    // Tree   
        sf::RectangleShape wood(sf::Vector2f(15,100));
        wood.setFillColor(sf::Color(81,54,26));
        wood.setPosition(4*window_widht/5  ,window_height-grass_level-100);
        sf::ConvexShape foliage(4);
        foliage.setPoint(0,sf::Vector2f(4*window_widht/5 - 30, window_height-grass_level-150));
        foliage.setPoint(1,sf::Vector2f(4*window_widht/5 + 45, window_height-grass_level-150));
        foliage.setPoint(2,sf::Vector2f(4*window_widht/5 + 45, window_height-grass_level-60));
        foliage.setPoint(3,sf::Vector2f(4*window_widht/5 - 30, window_height-grass_level-60));
        foliage.setFillColor(sf::Color::Green);
    
    //Rain
        int count_drop = 0;
        sf::RectangleShape drop[rain_size];
        int drop_size = 3;
        for(int i = 0; i < rain_size ;i++){
            drop[i].setSize(sf::Vector2f(drop_size,drop_size));
            drop[i].setFillColor(sf::Color::Cyan);
            drop[i].setPosition(0,window_height);
        };
        sf::Time drop_life[rain_size];
        for(int i = 0; i<rain_size; i++){
            drop_life[i] = sf::milliseconds(random(0,10000)); //set_drop_life();
        };

    //Game time
        sf::Clock main_clock;
        sf::Clock clock1;

    while (window.isOpen()){ //Main loop

        float time = clock1.restart().asMicroseconds();
        time = time/1000 ;
        //Movement of drops
        for(int i = 0; i< rain_size; i++){
            //Respawn drop
            if (main_clock.getElapsedTime() > drop_life[i] ){
                drop[i].setPosition(random(0,window_widht),0); 
                drop_life[i] += set_drop_life();
                count_drop++;
            }; 
            //If tree
            if (((drop[i].getPosition().x >= 4*window_widht/5 - 30)&&
                (drop[i].getPosition().x <= 4*window_widht/5 + 45)&&
                (drop[i].getPosition().y <= window_height-grass_level-60)&&
                (drop[i].getPosition().y >= window_height-grass_level-150))||
                ((drop[i].getPosition().x >= 4*window_widht/5)&&
                (drop[i].getPosition().x <= 4*window_widht/5 + 15 - drop_size/2)&&
                (drop[i].getPosition().y >= window_height-grass_level-60)&&
                (drop[i].getPosition().y <= window_height-grass_level-drop_size))){
                    //Move slowly
                    if (random(0,2)){
                        drop[i].move(0,0.02*time*random(1, 4)); 
                    };
            }else if (drop[i].getPosition().y <= window_height-grass_level-drop_size){
                drop[i].move(0,0.1*time*random(1, 4)); 
            };
        };



        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.close();
        };
        

        window.clear();
        window.draw(foliage);
        window.draw(wood);
        window.draw(grass);
        for(int i = 0; i < rain_size;i++) window.draw(drop[i]);
        window.display();
    };
    //Save information
        std::ofstream output;
        output.open("status.txt");
        output<< "Всього випало крапель дощу: "<<count_drop <<'\n';
        output.close();

    return 0;
}

int random(int min,int max ){
    return std::rand()%(max-min)+min;
}

sf::Time set_drop_life(){
    return sf::milliseconds(random(6000,15000));
};
