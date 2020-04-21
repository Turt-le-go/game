# Game
 
Added:
---
* Rain on/off
* Wind 
* Fullscreen mode
* New tree

Fullscreen mode:
---

 You can change video mode in file settings.txt or from terminal:
 
 ```./game 1``` - to launch the game in fullscreen,
 
 ```./game 0``` - to start the game in window mode.

Wind:
---

 Press left or right key to control the wind.

Rain:
---

 Press R to start / stop rain.

Run:
---

 Firstly, install SFML:

  ```sudo pacman -S sfml``` - for Archlinux,

  ```sudo apt-get install libsfml-dev``` - for Debian.

 Then, you can run game:

  ```./game```

  Also, if you want to recompile game, run:

  ```g++ -c main.cpp; g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system; rm main.o;```
