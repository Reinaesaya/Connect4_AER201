#ifndef WHEELS_H
#define WHEELS_H

class Wheels
{
  public:
    Wheels(int EN, int LM_F, int LM_B, int RM_F, int RM_B);
    void Forward(int SPD);
    void Backward(int SPD);
    void Pivot_L(int SPD, int turn_time);
    void Pivot_R(int SPD, int turn_time);
    void Turn_L(int SPD, int turn_time);
    void Turn_R(int SPD, int turn_time);
    void Stop();
    
  private:
    int enable_pin;
    int left_motor_f;
    int left_motor_b;
    int right_motor_f;
    int right_motor_b;
};

#endif
