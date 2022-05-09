#include <cmath>
#include <iostream>
#include "cell.h"
#include "game.h"
#include "automated_snake.h"

void AutomatedSnake::Simulate(
    const SDL_Point& target,
    const std::vector<SDL_Point>* obstacles_ptr,
    std::vector<SDL_Point>* player_occupied_cells_ptr)
{
  _player_occupied_cells_ptr = player_occupied_cells_ptr;
  _obstacles_ptr = obstacles_ptr;
  SDL_Point previous_cell{ static_cast<int>(_head_x), static_cast<int>(_head_y) }; // We first capture the head's cell before updating.
  std::vector<Cell> neighbors;
  AddNeighbors(previous_cell, neighbors);
  Cell target_cell = NextCell(target, neighbors);
  _direction = target_cell.direction;
  UpdateHead();
  SDL_Point current_cell{ static_cast<int>(_head_x), static_cast<int>(_head_y) };
  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != previous_cell.x || current_cell.y != previous_cell.y)
    UpdateBody(current_cell, previous_cell);
  UpdateOccupiedCells();
}

void AutomatedSnake::AddNeighbors(const SDL_Point& current_cell, std::vector<Cell>& neighbors)
{
  const int x = current_cell.x;
  const int y = current_cell.y;

  int left_x = static_cast<int>(fmod(x - 1 + _grid_width, _grid_width));
  Cell left_cell{ left_x, y, Snake::Direction::kLeft };
  int right_x = static_cast<int>(fmod(x + 1 + _grid_width, _grid_width));
  Cell right_cell{ right_x, y, Snake::Direction::kRight };
  int up_y = static_cast<int>(fmod(y - 1 + _grid_height, _grid_height));
  Cell up_cell{ x, up_y, Snake::Direction::kUp };
  int down_y = static_cast<int>(fmod(y + 1 + _grid_height, _grid_height));
  Cell down_cell{ x, down_y, Snake::Direction::kDown };

  // left
  if (!SnakeCell(left_cell.x, left_cell.y) && !Game::OccupiedCell(left_cell.x, left_cell.y, *_obstacles_ptr, *_player_occupied_cells_ptr)
      && (_size == 1 || _direction != Snake::Direction::kRight)) {
    neighbors.push_back(std::move(left_cell));
  }
  // right
  if (!SnakeCell(right_cell.x, right_cell.y) && !Game::OccupiedCell(right_cell.x, right_cell.y, *_obstacles_ptr, *_player_occupied_cells_ptr)
      && (_size == 1 || _direction != Snake::Direction::kLeft)) {
    neighbors.push_back(std::move(right_cell));
  }
  // up
  if (!SnakeCell(up_cell.x, up_cell.y) && !Game::OccupiedCell(up_cell.x, up_cell.y, *_obstacles_ptr, *_player_occupied_cells_ptr)
      && (_size == 1 || _direction != Snake::Direction::kDown)) {
    neighbors.push_back(std::move(up_cell));
  }
  // down
  if (!SnakeCell(down_cell.x, down_cell.y) && !Game::OccupiedCell(down_cell.x, down_cell.y, *_obstacles_ptr, *_player_occupied_cells_ptr)
      && (_size == 1 || _direction != Snake::Direction::kUp)) {
    neighbors.push_back(std::move(down_cell));
  }
}

Cell AutomatedSnake::NextCell(const SDL_Point& target, const std::vector<Cell>& neighbors)
{
  int g = 1; // equidistant map
  int h = ManhattanDistance(SDL_Point{ 0, 0 }, SDL_Point{ _grid_width, _grid_height }); //maximum distance
  int f = g + h;
  Cell next_cell;
  for (auto const& neighbor : neighbors) {
    int f_neighbor = g + ManhattanDistance(target, SDL_Point{ neighbor.x, neighbor.y });
    if (f_neighbor < f) {
      f = f_neighbor;
      next_cell = neighbor;
    } else if (f_neighbor == f && neighbor.direction == _direction) {
      f = f_neighbor;
      next_cell = neighbor;
    }

  }
  return next_cell;
}

int AutomatedSnake::ManhattanDistance(const SDL_Point& p1, const SDL_Point& p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}
