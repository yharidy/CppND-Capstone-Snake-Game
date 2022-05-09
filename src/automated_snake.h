#ifndef AUTOMATED_SNAKE_H
#define AUTOMATED_SNAKE_H

#include <memory>
#include <mutex>
#include <vector>
#include "SDL.h"
#include "params.h"
#include "snake.h"

class AutomatedSnake : public Snake
{
public:
  AutomatedSnake() : Snake(){};
  AutomatedSnake(float speed, float head_x, float head_y)
      : Snake(speed, head_x, head_y){};
  void Simulate(const SDL_Point& target, const std::vector<SDL_Point>* obstacles_ptr, std::vector<SDL_Point>* player_occupied_cells_ptr);

private:
  void AddNeighbors(const SDL_Point& current_cell, std::vector<Cell>& neighbors);
  Cell NextCell(const SDL_Point& target, const std::vector<Cell>& neighbors);
  int ManhattanDistance(const SDL_Point& p1, const SDL_Point& p2);
};
#endif