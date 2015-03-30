#ifndef GRID_H
#define GRID_H

#include "Wheels.h"
#include <Keypad.h>
#include "Encoder.h"
#include "constants.h"
#include "Arduino.h"


/* NAVIGATION USING IR SENSOR*/
void line_following(Wheels& wheels);
void i_hopper_navigation(Wheels& wheels, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val, float angle);
void backward_align(Wheels& wheels);
void first_turn(Wheels& wheels,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val);
void reset_origin(Wheels& wheels, byte hopper, byte *num_inter, byte *num_turn);


/* GENERAL HELPER FUNCTIONS*/
void shift_add(long* arr, int length, long b);
void init_white_raven(Keypad& Mykeypad, byte *x_val, byte *y_val, byte *num_ball, float *angle);
int check_array(long* arr, int length, long thresh);
int us_check(long* arr, int length, long thresh);
#endif
