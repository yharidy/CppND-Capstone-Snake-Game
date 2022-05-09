#include "cell.h"

Cell::Cell(int pos_x, int pos_y, Snake::Direction d)
{
  x = pos_x;
  y = pos_y;
  direction = d;
}

Cell::~Cell() {}