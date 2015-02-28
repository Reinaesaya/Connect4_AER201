/*
The following code implements differential steering
for two DC motors connected to a L293D H-bridge circuit.

The system also uses the HC-SR04 Ultrasonic Sensor to prevent
the robot from crashing into the walls.
*/

#include "Ultrasonic.h"
#include "Diff_steer.h"

/* Instantiate ultrasonic(trig,echo)
*/
Ultrasonic ultrasonic(23,22);

/* Instantiate DiffSteering(enable, LM_F, LM_B, RM_F, RM_B)
*/

DiffSteering diffsteering(13,12,11,10,9);
// Arduino UNO PWM pins are 3,5,6,9,10,11
// Arduino Mega PWM pins are 2-13, 44-46

long range_array[5] = {100,100,100,100,100};

void setup() {
  Serial.begin(9600);
}

void loop()
{
  long obj_range = ultrasonic.Ranging(CM);
  
  Serial.print("Object Range: \t");
  Serial.println(obj_range);
  
  shift_add(range_array, 5, obj_range);
  
  if (check_array(range_array, 5, 40) == 1) {
    diffsteering.Forward(200);
  }
  else {
    diffsteering.Forward(200);
  }
  delay(10);
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
