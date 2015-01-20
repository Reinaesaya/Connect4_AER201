#include "Ultrasonic.h"

// Instatiate ultrasonic(trig,echo)
Ultrasonic ultrasonic(8,9);
int left_motor = 5;
int right_motor = 4;

void setup() {
  Serial.begin(9600);
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
  digitalWrite(left_motor, HIGH);
  digitalWrite(right_motor, HIGH);
}

void loop()
{
  long obj_range = ultrasonic.Ranging(CM);
  Serial.println(obj_range);
  if (obj_range < 10) {
    digitalWrite(left_motor, LOW);
    digitalWrite(right_motor, LOW);
  }
  else {
    digitalWrite(left_motor, HIGH);
    digitalWrite(right_motor, HIGH);
  }
  delay(100);
}




