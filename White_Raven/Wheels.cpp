/*
Differential steering library for two DC motors attached to
L293D H-bridge design.
*/

#include "Arduino.h"
#include "Wheels.h"

Wheels::Wheels( // Constructor
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

void Wheels::Pivot_L(int SPD, int turn_time)
{
  Serial.println("Pivot Left");
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, SPD); // Arbitrary speed
  analogWrite(right_motor_f, SPD);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(turn_time); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Pivot_R(int SPD, int turn_time)
{
  Serial.println("Pivot Right");
  analogWrite(left_motor_f, SPD); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, SPD);
  digitalWrite(enable_pin, HIGH);
  delay(turn_time); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Turn_L(int SPD, int turn_time)
{
  Serial.println("Turn Left");
  analogWrite(left_motor_f, 0); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, SPD);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(turn_time); // Arbitrary estimation of time
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Turn_R(int SPD, int turn_time)
{
  Serial.println("Turn Right");
  analogWrite(left_motor_f, SPD); // Arbitrary speed
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  delay(turn_time); // Arbitrary estimation of time
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
