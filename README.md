# CPPND: Capstone Snake Game Example

This is a variation on the Snake Game provided in the starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

## Variations 
* Automated enemy snake: an automated enemy snake uses A* search to locate the target, avoid obstacles, avoid the player snake and reach the food
* Levels and obstacles: Based on the score of the player, there are now three levels in the game. While the player score is below zero, there are no obstacles and the enemy snake has half the speed of the player snake. Starting from a score of 5, the enemy snake has twice its initial speed. Staring from a score of 10, obstacles will be randomly placed 
on the grid and renewed every time the food has been eaten. Both player and enemy must avoid the obstacles.

## Code Structure
The AutomatedSnake class, which inherits from the Snake class, a Cell class and diverse functions were added to the game. The game loop runs as follows: the Game::Update function first calls the Snake::Update function 
to handle player's input. Afterwards, it checks if the player is dead or if the player has reached the food. Afterwards, it does the same for the automated snake (simulate, check if dead and check if it reached the food). At the end, the game controls the difficulty of the game 
based on the player's score as described above.

## Expected behavior
There should be an automated snake that navigates towards the food, avoids the player snake and any obstacles. The game level changes as described above, based on the player's score. 
If the player crosses the automated snake (body or head), it should die. The automated snake starts the game with half the player's speed to make the game easier.

## Rubric Points
* Compiling and Testing:
  As in the Basic Build Instruction section above, the project code is able to compile and run without any errors.
* Loops, Functions, I/O
    The AutomatedSnake class as well as the Game::Update, Game::PlaceObstacles and Game:OccupiedCell Functionshas diverse if statements and for/while loops 
	
* Object Oriented Programming
    Cell, and AutomatedSnake are new classes added to the starter code to group data/functions into individual classes. The AutomatedSnake classe inherits from the Snake class and adds functionality such as 
	the A* search to autonomously navigate the grid, avoid obstacles and reach the target. The Snake class has been modified to include Setters and Getters. The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. All class data members are explicitly specified as public, protected, or private.
    
* Memory Management
  All functions in the AutomatedSnake class use pass-by-reference or pass-by-pointer to avoid unnecessary copying.

* Concurrency
  Does not apply, since the automated snake must react to the player's actions and therefore they must run sequentially. 


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
