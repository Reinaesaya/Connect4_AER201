/*
Differential steering library for two DC motors attached to
L293D H-bridge design.
*/

#include "Arduino.h"
#include "Diff_steer.h"

DiffSteering::DiffSteering( // Constructor
  int EN, int LM_F, int LM_B, int RM_F, int RM_B)
{
   pinMode(EN,OUTPUT);
   pinMode(LM_F,OUTPUT);
   pinMode(LM_B,OUTPUT);
   pinMode(RM_F,OUTPUT);
   pinMode(RM_B,OUTPUT);
   enable_pin = EN;
   left_motor_f = LM_F;
   left_motor_b = LM_B;
   right_motor_f = RM_F;
   right_motor_b = RM_B;
}

void DiffSteering::Forward(int SPD)
{
  analogWrite(left_motor_f, SPD);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, SPD+50);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Backward(int SPD)
{
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, SPD);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, SPD);
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Pivot_L(int angle)
{
  Serial.println("Pivot Left");
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, 150); // Arbitrary speed
  analogWrite(right_motor_f, 150);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(angle*10); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Pivot_R(int angle)
{
  Serial.println("Pivot Right");
  analogWrite(left_motor_f, 150); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, 150);
  digitalWrite(enable_pin, HIGH);
  delay(angle*10); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Turn_L(int millisec, int inner, int outer)
{
  Serial.println("Turn Left");
  analogWrite(left_motor_f, inner); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, outer+50);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(millisec); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Turn_R(int millisec, int inner, int outer)
{
  Serial.println("Turn Right");
  analogWrite(left_motor_f, outer); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, inner+50);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(millisec); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void DiffSteering::Stop()
{ // Complete restart, not just pause
  Serial.println("Stop");
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, LOW);
}
