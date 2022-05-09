#include "snake.h"
#include <cmath>
#include <iostream>
#include "cell.h"
#include "game.h"


void Snake::init()
{
  _grid_width = Params::kGridWidth;
  _grid_height = Params::kGridHeight;
}
Snake::Snake()
{
  init();
  _head_x = _grid_width / 2.0F;
  _head_y = _grid_height / 2.0F;
  _speed = 0.05F;
}

Snake::Snake(float speed, float head_x, float head_y) 
{
  init();
  _head_x = head_x;
  _head_y = head_y;
  _speed = speed;
}

Snake::Direction Snake::GetDirection() const
{
  return this->_direction;
}

void Snake::SetDirection(Snake::Direction direction) 
{
  this->_direction = direction;
}

float Snake::GetSpeed() const
{
  return this->_speed;
}

void Snake::SetSpeed(float speed)
{
  this->_speed = speed;
}

int Snake::GetSize() const
{
  return this->_size;
}

bool Snake::GetState() const
{
  return this->_alive;
}

void Snake::SetState(bool state)
{
  this->_alive = state;
}

std::pair<float, float> Snake::GetHeadPosition() const
{
  return std::pair<float, float>(this->_head_x, this->_head_y);
}

std::vector<SDL_Point> Snake::GetBody() const
{
  return this->_body;
}

void Snake::Update(const std::vector<SDL_Point>* obstacles_ptr, std::vector<SDL_Point>* player_occupied_cells_ptr)
{
  _player_occupied_cells_ptr = player_occupied_cells_ptr;
  _obstacles_ptr = obstacles_ptr;
  SDL_Point prev_cell{ static_cast<int>(_head_x), static_cast<int>(_head_y) }; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{ static_cast<int>(_head_x), static_cast<int>(_head_y) }; // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
  UpdateOccupiedCells();
}


void Snake::UpdateHead()
{
  switch (_direction) {
  case Direction::kUp:
    _head_y -= _speed;
    break;

  case Direction::kDown:
    _head_y += _speed;
    break;

  case Direction::kLeft:
   _head_x -= _speed;
    break;

  case Direction::kRight:
    _head_x += _speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  _head_x = static_cast<float>(fmod(_head_x + _grid_width, _grid_width));
  _head_y = static_cast<float>(fmod(_head_y + _grid_height, _grid_height));
  
  // Check if the snake has died.
  for (auto const& obstacle : *_obstacles_ptr) {
    if (static_cast<int>(_head_x) == obstacle.x && static_cast<int>(_head_y) == obstacle.y) {
      _alive = false;
    }
  }
  for (auto const& point : _body) {
    if (static_cast<int>(_head_x) == point.x && static_cast<int>(_head_y) == point.y) {
      _alive = false;
    }
  }
  /*
  for (auto& occupied_cell : *(this->_player_occupied_cells_ptr)) {
    if (static_cast<int>(_head_x) == occupied_cell.x && static_cast<int>(_head_y) == occupied_cell.y)
      _alive = false;
  }
  */
}

void Snake::UpdateBody(SDL_Point& current_head_cell, SDL_Point& prev_head_cell)
{
  // Add previous head location to vector
  _body.push_back(prev_head_cell);

  if (!_growing) {
    // Remove the tail from the vector.
    _body.erase(_body.begin());
  } else {
    _growing = false;
    ++_size;
  }

  // Check if the snake has died.
  for (auto const& item : _body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      _alive = false;
    }
  }
}

void Snake::UpdateOccupiedCells()
{
  std::lock_guard<std::mutex> lck(Game::_mtx_occupied_cells);
  _player_occupied_cells_ptr->push_back(SDL_Point{ static_cast<int>(_head_x), static_cast<int>(_head_y) });
  for (SDL_Point point : _body)
    _player_occupied_cells_ptr->push_back(point);
}

void Snake::GrowBody()
{
  _growing = true;
}

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y)
{
  if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y)) {
    return true;
  }
  for (auto const& item : _body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
