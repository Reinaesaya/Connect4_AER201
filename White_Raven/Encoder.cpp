#include "Encoder.h"
#include "Arduino.h"
#include "constants.h"

Encoder::Encoder(int LPinA, int LPinB, int RPinA, int RPinB) :
  left_pin_a (LPinA), left_pin_b (LPinB), right_pin_a (RPinA), right_pin_b (RPinB)
{
  pinMode(this->left_pin_a, INPUT);
  pinMode(this->left_pin_b, INPUT);
  pinMode(this->right_pin_a, INPUT);
  pinMode(this->right_pin_b, INPUT);
  digitalWrite(this->left_pin_a, HIGH);
  digitalWrite(this->left_pin_b, HIGH);
  digitalWrite(this->right_pin_a, HIGH);
  digitalWrite(this->right_pin_b, HIGH);
}

void Encoder::update_L_A()
{
  this->L_A_set = (digitalRead(this->left_pin_a) == HIGH);
  this->left_pos += (this->L_A_set != this->L_B_set) ? +1 : -1;
}

void Encoder::update_L_B()
{
  this->L_B_set = (digitalRead(this->left_pin_b) == HIGH);
  this->left_pos += (this->L_A_set == this->L_B_set) ? +1 : -1;
}

void Encoder::update_R_A()
{
  this->R_A_set = (digitalRead(this->right_pin_a) == HIGH);
  this->right_pos += (this->R_A_set != this->R_B_set) ? +1 : -1;
}

void Encoder::update_R_B()
{
  this->R_B_set = (digitalRead(this->right_pin_b) == HIGH);
  this->right_pos += (this->R_A_set == this->R_B_set) ? +1 : -1;
}

signed long int Encoder::getPosLeft() { return this->left_pos; }

signed long int Encoder::getPosRight() { return this->right_pos; }

float Encoder::getPivotAngle()
{
  // positive turn clockwise, negative turn counterclockwise
  float angle = (this->left_pos + (-1*this->right_pos)) * 90 / TICKS_PER_90_DEG;
  return angle;
}

void Encoder::reset()
{
  this->left_pos = 0;
  this->right_pos = 0;
  this->L_A_set = false;
  this->L_B_set = false;
  this->R_A_set = false;
  this->R_B_set = false;
  Serial.println("Encoder Reset");
}
