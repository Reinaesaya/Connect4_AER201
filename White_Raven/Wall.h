#ifndef WALL_H
#define WALL_H

#include "Ultrasonic.h"
#include "Wheels.h"

/* BASIC WALL FOLLOWING FUNCTIONS */
void shift_add(long* arr, long b);
int check_array(long* arr, long thresh);
void initialize_ultrasonic(Ultrasonic& US, long *array);
void wall_straight_adjust(long *arr, int SPD, Wheels& wheels);
void wall_dist_adjust(long *arr, long targ, int SPD, Wheels& wheels);

/* WALL POSITIONING AND NAVIGATION */
void forward_to_dist(long dist, Wheels& wheels, Ultrasonic& front_US, long *front_array, int SPD);
void forward_to_dist_wall(long stop_dist, long wall_dist, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& side_US,
                          long *front_array, long *side_array, int SPD);
float find_wall_and_position(int side, float angle, Wheels& wheels, Ultrasonic& front_US, long *front_array);
float get_to_gameboard(int side, float angle, int game_column, Wheels& wheels,
                       Ultrasonic& front_US, Ultrasonic& left_US, Ultrasonic& right_US,
                       long *front_array, long *left_array, long *right_array);

#endif
