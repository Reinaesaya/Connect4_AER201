#ifndef WALL_H
#define WALL_H

#include "Ultrasonic.h"
#include "Wheels.h"

void initialize_ultrasonic(Ultrasonic& US, long *array);
void wall_straight_adjust(long *arr, int SPD, Wheels& wheels);
void wall_dist_adjust(long *arr, long targ, int SPD, Wheels& wheels);
void shift_add(long* arr, long b);
int check_array(long* arr, long thresh);

#endif
