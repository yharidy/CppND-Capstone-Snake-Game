#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "params.h"

int main() {
  Renderer renderer(Params::kScreenWidth, Params::kScreenHeight, Params::kGridWidth, Params::kGridHeight);
  Controller controller;
  Game game(Params::kGridWidth, Params::kGridHeight);
  game.Run(controller, renderer, Params::kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}