#include "game.h"
#include <iostream>
#include <future>
#include "SDL.h"

std::mutex Game::_mtx_occupied_cells;

Game::Game(std::size_t grid_width, std::size_t grid_height) : _grid_width{grid_width}, _grid_height{grid_height}
{
  _player_snake = Snake(0.1F, _grid_width / 2.0F, _grid_height / 3.0F);
  _cpu_snake = AutomatedSnake(0.05F, _grid_width / 3.0F, _grid_height / 2.0F);
  _engine = std::mt19937(_dev());
  _random_w = std::uniform_int_distribution<int>(0, static_cast<int>(grid_width - 1));
  _random_h = std::uniform_int_distribution<int>(0, static_cast<int>(grid_height - 1));
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, _player_snake);
    Update();
    renderer.Render(_player_snake, _cpu_snake, _food, _obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(_player_score, _cpu_score, _level, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = _random_w(_engine);
    y = _random_h(_engine);

    if (Game::OccupiedCell(x, y, _obstacles, player_occupied_cells))
      continue;

    _food.x = x;
    _food.y = y;
    return;
  }
}

void Game::PlaceObstacles() {
  _obstacles.clear();
  int x, y;
  while (true) {
    x = _random_w(_engine);
    y = _random_h(_engine);

    if (Game::OccupiedCell(x, y, _obstacles, player_occupied_cells) || (x != _food.x && y != _food.y))
      continue;
    SDL_Point obstacle{ x, y };
    _obstacles.emplace_back(obstacle);

    float occupiedCells = static_cast<float>(std::max(_level * 0.005, 0.015));
    if (_obstacles.size() >= occupiedCells * _grid_width * _grid_height)
      return;
  }
}

void Game::Update() {

    if (!_player_snake.GetState())
      return;
    if (!_cpu_snake.GetState())
      return;
  player_occupied_cells.clear();
  
  // update player snake
  //auto ftr_player_snake = std::async(&Snake::Update, &_player_snake, &_obstacles, &player_occupied_cells);
  //ftr_player_snake.wait();
  _player_snake.Update(&_obstacles, &player_occupied_cells);
  std::pair<float, float> player_new_head = _player_snake.GetHeadPosition();
  int player_new_x = static_cast<int>(player_new_head.first);
  int player_new_y = static_cast<int>(player_new_head.second);

  // check if player snake died
  if (_cpu_snake.SnakeCell(player_new_x, player_new_y)) 
    _player_snake.SetState(false);
  
  if (!_player_snake.GetState())
    return;
  
  // Check if player reached food
  bool foodAquired = false;
  if (_food.x == player_new_x && _food.y == player_new_y) {
    _player_score++;
    // Grow snake and increase speed.
    _player_snake.GrowBody();
    _player_snake.SetSpeed(_player_snake.GetSpeed() + 0.01);
    foodAquired = true;
  }

  // update automated snake
  //auto ftr_cpu_snake = std::async(&AutomatedSnake::Simulate, &_cpu_snake, _food, &_obstacles, &player_occupied_cells);
  //ftr_cpu_snake.wait();
  _cpu_snake.Simulate(_food, &_obstacles, &player_occupied_cells);
  std::pair<float, float> cpu_new_head = _cpu_snake.GetHeadPosition();
  int cpu_new_x = static_cast<int>(cpu_new_head.first);
  int cpu_new_y = static_cast<int>(cpu_new_head.second);
  
  // check if automated snake died
  if (_player_snake.SnakeCell(cpu_new_x, cpu_new_y))
    _cpu_snake.SetState(false);

  if (!_cpu_snake.GetState())
    return;

  // Check if automated snake reached food
  if (_food.x == cpu_new_x && _food.y == cpu_new_y) {
    _cpu_score++;
    // Grow snake and increase speed.
    _cpu_snake.GrowBody();
    //cpu_snake.speed += 0.01;
    foodAquired = true;
   }

  _level = std::max(_player_score, _cpu_score) / _points_per_level;
   if (foodAquired) {
     if (_level > 2)
       PlaceObstacles();
     PlaceFood();
   }
}
bool Game::OccupiedCell(
    const int& x, const int& y, const std::vector<SDL_Point>& obstacles, const std::vector<SDL_Point>& player_occupied_cells)
{
  for (auto obstacle : obstacles) {
    if (x == obstacle.x && y == obstacle.y)
      return true;
  }

  for (auto occupied_cell : player_occupied_cells) {
    if (x == occupied_cell.x && y == occupied_cell.y)
      return true;
  }
  return false;
}
int Game::GetScore() const { return _player_score; }
int Game::GetSize() const { return _player_snake.GetSize(); }