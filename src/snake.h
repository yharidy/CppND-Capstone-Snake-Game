#ifndef SNAKE_H
#define SNAKE_H

#include <memory>
#include <mutex>
#include <vector>
#include "SDL.h"
#include "params.h"
#undef main

class Cell; //forward declaration

class Snake
{
public:
  enum Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };
  Snake();
  Snake(float speed, float head_x, float head_y);
  Direction GetDirection() const;
  void SetDirection(Direction direction);
  float GetSpeed() const;
  void SetSpeed(float speed);
  int GetSize() const;
  bool GetState() const;
  void SetState(bool state);
  std::pair<float, float> GetHeadPosition() const;
  std::vector<SDL_Point> GetBody() const;

  void Update();
  void GrowBody();
  bool SnakeCell(int x, int y);

protected:
  void init();
  void UpdateHead();
  void UpdateBody(SDL_Point& current_cell, SDL_Point& prev_cell);
  void UpdateOccupiedCells();
  bool _growing{ false };
  int _grid_width;
  int _grid_height;

  Direction _direction = Direction::kUp;
  float _speed{};
  int _size{ 1 };
  bool _alive{ true };
  float _head_x;
  float _head_y;
  std::vector<SDL_Point> _body;
  std::vector<SDL_Point>* _player_occupied_cells_ptr{ nullptr };
  const std::vector<SDL_Point>* _obstacles_ptr{ nullptr };
};
#endif