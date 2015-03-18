#ifndef WALL_H
#define WALL_H

void initialize_ultrasonic(Ultrasonic US, long *array);
void wall_straight_adjust(long *array, int SPD);
void wall_dist_adjust(long *arr, long targ, int SPD);
void shift_add(long* arr, long b);
int check_array(long* arr, long thresh);

#endif
