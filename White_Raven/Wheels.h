#ifndef WHEELS_H
#define WHEELS_H

#include "Encoder.h"

class Wheels
{
  public:
    Wheels(int EN, int LM_F, int LM_B, int RM_F, int RM_B, Encoder& E);
    Encoder encoder;
    void Forward(int SPD);
    void Forward_straight(int SPD);
    void Forward(int SPD, signed long int ticks);
    void Backward(int SPD);
    void Backward_straight(int SPD);
    void Backward(int SPD, signed long int ticks);
    void Pivot_L(float angle);
    void Pivot_R(float angle);
    void Pivot_L(float angle, int SPD);
    void Pivot_R(float angle, int SPD);
    void Turn_L(unsigned long millisec, int inner, int outer);
    void Turn_R(unsigned long millisec, int inner, int outer);
    void Back_L(unsigned long millisec, int inner, int outer);
    void Back_L_Angle(float angle, int inner, int outer);
    void Back_R(unsigned long millisec, int inner, int outer);
    void Back_R_Angle(float angle, int inner, int outer);
    void Stop();
    
  private:
    const int enable_pin;
    const int left_motor_f;
    const int left_motor_b;
    const int right_motor_f;
    const int right_motor_b;
};

#endif
