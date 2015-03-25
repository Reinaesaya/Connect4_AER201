#ifndef WALL_H
#define WALL_H

#include "Ultrasonic.h"
#include "Wheels.h"

/* BASIC WALL FOLLOWING FUNCTIONS */
void wall_straight_adjust(int side, Ultrasonic& side_US, int SPD, Wheels& wheels);
void wall_dist_adjust(int side, Ultrasonic& side_US, long targ, int SPD, Wheels& wheels);

/* WALL POSITIONING AND NAVIGATION */
void forward_to_dist(long dist, Wheels& wheels, Ultrasonic& front_US, int SPD);
void forward_to_dist_wall(int side, long stop_dist, long wall_dist, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& side_US, int SPD);
void find_wall_and_position(int side, float& angle, Wheels& wheels, Ultrasonic& front_US);
void get_to_gameboard(int side, float& angle, int game_column, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& left_US, Ultrasonic& right_US);

#endif
