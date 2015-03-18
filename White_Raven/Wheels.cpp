/*
Differential steering library for two DC motors attached to
L293D H-bridge design.
*/

#include "Arduino.h"
#include "Wheels.h"
#include "Encoder.h"

Wheels::Wheels(int EN, int LM_F, int LM_B, int RM_F, int RM_B, Encoder E) :
  enable_pin (EN), left_motor_f (LM_F), left_motor_b (LM_B), right_motor_f (RM_F), right_motor_b (RM_B), encoder (E)
{
   pinMode(enable_pin,OUTPUT);
   pinMode(left_motor_f,OUTPUT);
   pinMode(left_motor_b,OUTPUT);
   pinMode(right_motor_f,OUTPUT);
   pinMode(right_motor_b,OUTPUT);
   Serial.println("Wheels initialized");
}

void Wheels::Forward(int SPD)
{
  analogWrite(left_motor_f, SPD);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, SPD);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Backward(int SPD)
{
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, SPD);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, SPD);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Pivot_L(float angle)
{
  Serial.println("Pivot Left");
  encoder.reset();
  while (encoder.getPivotAngle() > ((-1)*angle))
  {
    analogWrite(left_motor_f, 0);
    analogWrite(left_motor_b, 75);
    analogWrite(right_motor_f, 75);
    analogWrite(right_motor_b, 0);
    digitalWrite(enable_pin, HIGH);
    delay(1);
  }
  this->Stop();
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Pivot_R(float angle)
{
  Serial.println("Pivot Right");
  encoder.reset();
  while (encoder.getPivotAngle() < angle)
  {
    analogWrite(left_motor_f, 75);
    analogWrite(left_motor_b, 0);
    analogWrite(right_motor_f, 0);
    analogWrite(right_motor_b, 75);
    digitalWrite(enable_pin, HIGH);
    delay(1);
  }
  this->Stop();
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Turn_L(int millisec, int inner, int outer)
{
  Serial.println("Turn Left");
  analogWrite(left_motor_f, inner);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, outer);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(millisec);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Turn_R(int millisec, int inner, int outer)
{
  Serial.println("Turn Right");
  analogWrite(left_motor_f, outer);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, inner);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(millisec);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Stop()
{ // Complete restart, not just pause
  Serial.println("Stop");
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, LOW);
}
