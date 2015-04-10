#ifndef BASIC_H
#define BASIC_H

#include <Keypad.h>
/* Basic data structure low level functions used throughout program */

void shift_add(long* arr, long b);
void shift_add(int* arr, int b);
int check_array(long* arr, long thresh);
int check_array(int* arr, int thresh);
int check_array_half(long* arr, long thresh);
int check_array_half(int* arr, int thresh);
int get_input_number(Keypad MyKeypad);
void my_delay(unsigned long milli);
void update_angle(float& angle);

#endif
