/*
Differential steering library for two DC motors attached to
L293D H-bridge design.
*/

#include "Arduino.h"
#include "Wheels.h"
#include "Encoder.h"
#include "Basic.h"
#include "constants.h"

Wheels::Wheels(int EN, int LM_F, int LM_B, int RM_F, int RM_B, Encoder& E) :
  enable_pin (EN), left_motor_f (LM_F), left_motor_b (LM_B), right_motor_f (RM_F), right_motor_b (RM_B), encoder (E)
{
   pinMode(enable_pin,OUTPUT);
   pinMode(left_motor_f,OUTPUT);
   pinMode(left_motor_b,OUTPUT);
   pinMode(right_motor_f,OUTPUT);
   pinMode(right_motor_b,OUTPUT);
}

void Wheels::Forward(int SPD)
{
  analogWrite(left_motor_f, SPD + LEFT_MOTOR_OFFSET);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, SPD + RIGHT_MOTOR_OFFSET);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Forward(int SPD, signed long int ticks)
{
  interrupts();
  encoder.reset();
  while ((encoder.getPosLeft() + encoder.getPosRight())/2 <= ticks)
  { this->Forward(SPD);  }
  noInterrupts();
  this->Stop();
}

void Wheels::Backward(int SPD)
{
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, SPD + LEFT_MOTOR_OFFSET);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, SPD + RIGHT_MOTOR_OFFSET);
  digitalWrite(enable_pin, HIGH);
}

void Wheels::Backward(int SPD, signed long int ticks)
{
  interrupts();
  encoder.reset();
  while (((-1)*(encoder.getPosLeft() + encoder.getPosRight())/2) <= ticks)
  { this->Backward(SPD);  }
  noInterrupts();
  this->Stop();
}

void Wheels::Pivot_L(float angle)
{
  interrupts();
  encoder.reset();
  while (encoder.getPivotAngle() > ((-1)*angle))
  {
    analogWrite(left_motor_f, 0);
    analogWrite(left_motor_b, PIVOT_SPEED + LEFT_MOTOR_OFFSET);
    analogWrite(right_motor_f, PIVOT_SPEED + RIGHT_MOTOR_OFFSET);
    analogWrite(right_motor_b, 0);
    digitalWrite(enable_pin, HIGH);
  }
  noInterrupts();
  this->Stop();
}

void Wheels::Pivot_R(float angle)
{
  interrupts();
  encoder.reset();
  while (encoder.getPivotAngle() < angle)
  {
    analogWrite(left_motor_f, PIVOT_SPEED + LEFT_MOTOR_OFFSET);
    analogWrite(left_motor_b, 0);
    analogWrite(right_motor_f, 0);
    analogWrite(right_motor_b, PIVOT_SPEED + RIGHT_MOTOR_OFFSET);
    digitalWrite(enable_pin, HIGH);
  }
  noInterrupts(); 
  this->Stop();
}

void Wheels::Turn_L(unsigned long millisec, int inner, int outer)
{
  analogWrite(left_motor_f, inner + LEFT_MOTOR_OFFSET);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, outer + RIGHT_MOTOR_OFFSET);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  my_delay(millisec);
  this->Stop();
}

void Wheels::Turn_R(unsigned long millisec, int inner, int outer)
{
  analogWrite(left_motor_f, outer + LEFT_MOTOR_OFFSET);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, inner + RIGHT_MOTOR_OFFSET);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, HIGH);
  my_delay(millisec);
  this->Stop();
}

void Wheels::Back_L(unsigned long millisec, int inner, int outer)
{
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, inner + LEFT_MOTOR_OFFSET);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, outer + RIGHT_MOTOR_OFFSET);
  digitalWrite(enable_pin, HIGH);
  my_delay(millisec);
  this->Stop();
}

void Wheels::Back_R(unsigned long millisec, int inner, int outer)
{
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, outer + LEFT_MOTOR_OFFSET);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, inner + RIGHT_MOTOR_OFFSET);
  digitalWrite(enable_pin, HIGH);
  my_delay(millisec);
  this->Stop();
}

void Wheels::Stop()
{ // Complete restart, not just pause
  analogWrite(left_motor_f, 0);
  analogWrite(left_motor_b, 0);
  analogWrite(right_motor_f, 0);
  analogWrite(right_motor_b, 0);
  digitalWrite(enable_pin, LOW);
}
