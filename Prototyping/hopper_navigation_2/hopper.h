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
//void hopper_navigation(Wheels& MyWheel, byte mode, byte *num_inter, byte *num_turn,
                   // byte CL_X_val, byte CL_Y_val,byte CR_X_val,byte CR_Y_val,byte L_X_val,byte L_Y_val,byte R_X_val,byte R_Y_val);
void line_following(Wheels& MyWheel);
void i_hopper_navigation(Wheels& MyWheel, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val);
void go_to_origin(Wheels& MyWheel);
void backward_align(Wheels& MyWheel);
void first_turn(Wheels& MyWheel,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val);


/* GENERAL HELPER FUNCTIONS*/
void shift_add(long* arr, int length, long b);
void init_white_raven(byte *x_val, byte *y_val, byte *num_ball, int *angle);
//byte input_coor(Wheels& MyWheel, byte num_inter, byte num_turn, byte x_val, byte y_val,int hopper);
int check_array(long* arr, int length, long thresh);
int us_check(long* arr, int length, long thresh);
//byte direct_det(Wheels& MyWHeel, byte mode, byte CL_X_val, byte CL_Y_val,byte CR_X_val,byte CR_Y_val,byte L_X_val,byte L_Y_val,byte R_X_val,byte R_Y_val,
                //byte num_turn, byte num_inter);
int choose_angle(int hopper, int L, int R, int CL, int CR);
#endif
