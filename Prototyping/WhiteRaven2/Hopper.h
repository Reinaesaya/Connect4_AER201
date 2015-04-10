#ifndef HOPPER_H
#define HOPPER_H

class Hopper
{
  public:
    Hopper(int initializer);
    void update(int side, int corner, int orientation, int from_center);
    int side;
    int corner;
    int stop_forw;
    int stop_side;
    long dist_to_wall;
    signed long int tick_offset;
    float angle;
};

#endif
