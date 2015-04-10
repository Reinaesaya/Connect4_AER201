#ifndef GRID_H
#define GRID_H

#include "Wheels.h"
#include "Encoder.h"
#include "constants.h"
#include "Arduino.h"

/* ENCODER INTERRUPT*/
void doEncoder_L_A();
void doEncoder_L_B();
void doEncoder_R_A();
void doEncoder_R_B();

/* NAVIGATION USING IR SENSOR*/
void line_following(Wheels& MyWheel);
void i_hopper_navigation(Wheels& MyWheel, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val, int angle);
void backward_align(Wheels& MyWheel);
void first_turn(Wheels& MyWheel,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val);
void reset_origin(Wheels& MyWheel, byte hopper, byte *num_inter, byte *num_turn);


/* GENERAL HELPER FUNCTIONS*/
void shift_add(long* arr, int length, long b);
void init_white_raven(byte *x_val, byte *y_val, byte *num_ball, int *angle);
int check_array(long* arr, int length, long thresh);
int us_check(long* arr, int length, long thresh);
#endif
