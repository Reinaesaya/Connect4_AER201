#ifndef MY_LINE_FOLLOW_H
#define MY_LINE_FOLLOW_H

#include "Wheels.h"
#include "Hopper.h"
#include "Ultrasonic.h"
#include "Wall.h"

void line_follow_intersection(Wheels& wheels, int& num_intersections);
void line_follow_n_intersections(Wheels& wheels, int num);
void grid_follow(Wheels& wheels, int turn_dir, int forward_inter, int side_inter);
void get_to_hopper_pos(Wheels& wheels, Hopper& hopper, Ultrasonic& front_US, Ultrasonic& right_US, Ultrasonic& left_US, float& angle);
void get_to_origin_from_gameboard(Wheels& wheels, Hopper& hopper);

#endif
