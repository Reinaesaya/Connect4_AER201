#include "Arduino.h"
#include "Ultrasonic.h"
#include "Basic.h"

Ultrasonic::Ultrasonic(int TP, int EP)
{
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
}

long Ultrasonic::Timing()
{
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  duration = pulseIn(Echo_pin,HIGH);
  return duration;
}

long Ultrasonic::Ranging(int sys)
{
  Timing();
  distance_cm = duration / 29 / 2 ;
  distance_inc = duration / 74 / 2;
  Serial.println(distance_cm);
  if (sys)
  return distance_cm;
  else
  return distance_inc;
}

void Ultrasonic::initialize_array()
{
  for (int i=0; i<=(ARRAY_LEN*2); ++i) {
    long dist = this->Ranging(CM);
    shift_add(this->array, dist);
    my_delay(10); // my_delay disables interrupts after so use normal delay
  }
}

void Ultrasonic::update_array()
{
  long dist = this->Ranging(CM);
  shift_add(this->array, dist);
}
