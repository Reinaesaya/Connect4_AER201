#ifndef DIFF_STEER_H
#define DIFF_STEER_H

class DiffSteering
{
  public:
    DiffSteering(int EN, int LM_F, int LM_B, int RM_F, int RM_B);
    void Forward(int SPD);
    void Backward(int SPD);
    void Pivot_L(int angle);
    void Pivot_R(int angle);
    void Turn_L(int angle);
    void Turn_R(int angle);
    void Stop();
    
  private:
    int enable_pin;
    int left_motor_f;
    int left_motor_b;
    int right_motor_f;
    int right_motor_b;
};

#endif
