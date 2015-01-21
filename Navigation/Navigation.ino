#include "Ultrasonic.h"

// Instatiate ultrasonic(trig,echo)
Ultrasonic ultrasonic(8,9);
int left_motor = 5;
int right_motor = 3;

long range_array[5] = {100,100,100,100,100};

void setup() {
  Serial.begin(9600);
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
}

void loop()
{
  long obj_range = ultrasonic.Ranging(CM);
  
  Serial.print("Object Range: \t");
  Serial.println(obj_range);
  
  shift_add(range_array, 5, obj_range);
  
  if (check_array(range_array, 5, 40) == 1) {
    analogWrite(left_motor, 0);
    analogWrite(right_motor, 0);
    
    delay(100);
    turn('l', 500);
  }
  else {
    analogWrite(right_motor, 100);
    analogWrite(left_motor, 100);
  }
  delay(10);
}

void turn(char direction, int time) {
  Serial.println("TURN");
  analogWrite(left_motor, 0);
  analogWrite(right_motor, 0);
  
  if (direction == 'l') {
    Serial.println("LEFT");
    analogWrite(right_motor, 100);
  }
  else {
    Serial.println("RIGHT");
    analogWrite(left_motor, 100);
  }
  Serial.println("Gothere");
  delay(time);
}

void shift_add(long* arr, int length, long b) {
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[4] = b;
}

int check_array(long* arr, int length, long thresh) {
  
  int ret = 1;
  
  for (int i = 0; i != length; i++) {
    Serial.print(arr[i]);
    Serial.print("\t");
    if (arr[i] >= thresh) {
      ret = 0;
    }
  }
  Serial.print("\n");
  return ret;
}
