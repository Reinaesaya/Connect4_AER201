/* Encoder Implementation for differential steering
Adapted from:
Interrupt Example: Class Wrapper, by mikkoh [01/2010]
Interrupt Example: Tighten up the ISR's
http://playground.arduino.cc/Main/RotaryEncoders
*/

#ifndef ENCODER_H
#define ENCODER_H

class Encoder
{
  public:
    Encoder(int LPinA, int LPinB, int RPinA, int RPinB);
    void update_L_A();
    void update_L_B();
    void update_R_A();
    void update_R_B();
    signed long int getPosLeft();
    signed long int getPosRight();
    float getPivotAngle();
    void reset();
    
  private:
    volatile const int left_pin_a;
    volatile const int left_pin_b;
    volatile const int right_pin_a;
    volatile const int right_pin_b;
    volatile bool L_A_set;
    volatile bool L_B_set;
    volatile bool R_A_set;
    volatile bool R_B_set;
    volatile signed long int left_pos;
    volatile signed long int right_pos;
};


#endif
