/*
  Class implementation of module adapted from:
    Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
    Created by ITead studio. Alex, Apr 20, 2010.
    iteadstudio.com
*/

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"
#include "constants.h"

#define CM 1
#define INC 0

class Ultrasonic
{
  public:
    Ultrasonic(int TP, int EP);
    long array[ARRAY_LEN];
    
    long Timing();
    long Ranging(int sys);
    
    void initialize_array();
    void update_array();

  private:
    int Trig_pin;
    int Echo_pin;
    long duration,distance_cm,distance_inc;
    
};

#endif
