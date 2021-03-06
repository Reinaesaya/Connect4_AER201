#include "Encoder.h"
#include "Arduino.h"

Encoder::Encoder(int LPinA, int LPinB, int RPinA, int RPinB) :
  left_pin_a (LPinA), left_pin_b (LPinB), right_pin_a (RPinA), right_pin_b (RPinB)
{
  pinMode(left_pin_a, INPUT);
  pinMode(left_pin_b, INPUT);
  pinMode(right_pin_a, INPUT);
  pinMode(right_pin_b, INPUT);
  digitalWrite(left_pin_a, HIGH);
  digitalWrite(left_pin_b, HIGH);
  digitalWrite(right_pin_a, HIGH);
  digitalWrite(right_pin_b, HIGH);
  reset();
}

void Encoder::update_L_A()
{
  L_A_set = (digitalRead(left_pin_a) == HIGH);
  left_pos += (L_A_set != L_B_set) ? +1 : -1;
}

void Encoder::update_L_B()
{
  L_B_set = (digitalRead(left_pin_b) == HIGH);
  left_pos += (L_A_set == L_B_set) ? +1 : -1;
}

void Encoder::update_R_A()
{
  R_A_set = (digitalRead(right_pin_a) == HIGH);
  right_pos += (R_A_set != R_B_set) ? +1 : -1;
}

void Encoder::update_R_B()
{
  R_B_set = (digitalRead(right_pin_b) == HIGH);
  right_pos += (R_A_set == R_B_set) ? +1 : -1;
}

signed long int Encoder::getPosLeft() { return left_pos; }

signed long int Encoder::getPosRight() { return right_pos; }

float Encoder::getPivotAngle()
{
  // 90 degrees is 20000 on each side
  // positive turn clockwise, negative turn counterclockwise
  float angle = (left_pos + (-1*right_pos)) * 90 / 40000;
  return angle;
}

void Encoder::reset()
{
  left_pos = 0;
  right_pos = 0;
  L_A_set = false;
  L_B_set = false;
  R_A_set = false;
  R_B_set = false;
  Serial.println("Encoder Reset");
}

