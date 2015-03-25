#ifndef HOPPER_H
#define HOPPER_H

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
void line_following(Wheels& wheels);
void i_hopper_navigation(Wheels& wheels, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val, float angle);
void backward_align(Wheels& wheels);
void first_turn(Wheels& wheels,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val);
void reset_origin(Wheels& wheels, byte hopper, byte *num_inter, byte *num_turn);


/* GENERAL HELPER FUNCTIONS*/
void shift_add(long* arr, int length, long b);
void init_white_raven(byte *x_val, byte *y_val, byte *num_ball, float *angle);
int check_array(long* arr, int length, long thresh);
int us_check(long* arr, int length, long thresh);
#endif
