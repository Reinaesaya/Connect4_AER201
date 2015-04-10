#include "Hopper.h"
#include "constants.h"

Hopper::Hopper(int initializer):
  side(2), corner(2), stop_forw(0), stop_side(0), dist_to_wall(0), tick_offset(0), angle(0) {}

void Hopper::update(int side, int corner, int orientation, int from_center)
{
  this->side = side;
  this->corner = corner;
  if (this->corner == NOT_CORNER_HOP)
  {
    if (orientation == FLAT_UP)
    {
      stop_forw = 0;
      angle = 180;
    }
    else if (orientation == FLAT_DOWN)
    {
      stop_forw = 5;
      angle = 0;
    }
    if (from_center == CLOSE_CENTER)
    {
      stop_side = 2;
      tick_offset = 20000;
      dist_to_wall = 70;
    }
    else if (from_center == FAR_CENTER)
    {
      stop_side = 1;
      tick_offset = 50000;
      dist_to_wall = 50;
    }
  }
  else if (this->corner == CORNER_HOP)
  {
    stop_forw = 5;
    stop_side = 1;
    tick_offset = 10;
    if (side == LEFT_BOARD) { angle = 45; }
    else if (side == RIGHT_BOARD) { angle = 135; }
    dist_to_wall = 20;
  }
}
