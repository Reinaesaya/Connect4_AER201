/*
The following code implements differential steering
for two DC motors connected to a L293D H-bridge circuit.

The system also uses the HC-SR04 Ultrasonic Sensor to prevent
the robot from crashing into the walls.
*/

#include "Ultrasonic.h"
#include "Diff_steer.h"
#include <Stepper.h>

#define STEPPER_SPEED 30
#define STEPPER_STEPS 200

/*Instantiate stepper*/

Stepper stepper = Stepper(48,8,7,6,5);

/* Instantiate ultrasonic(trig,echo)
*/
Ultrasonic ultrasonic1(22,23);

/* Instantiate DiffSteering(enable, LM_F, LM_B, RM_F, RM_B)
*/

DiffSteering diffsteering(13,12,11,10,9);
// Arduino UNO PWM pins are 3,5,6,9,10,11
// Arduino Mega PWM pins are 2-13, 44-46

long range_array_1[5] = {100,100,100,100,100};

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(STEPPER_SPEED);
}

void loop()
{
  long obj_range_1 = ultrasonic1.Ranging(CM);
  shift_add(range_array_1, 5, obj_range_1);
  
  if (check_array(range_array_1, 5, 35) == 1) {
    getBall(diffsteering, stepper);
    while (true) continue;
  }
  else {
    //diffsteering.Forward(90);
  }
  delay(10);
}

void getBall(DiffSteering wheels, Stepper stepper) {
    Serial.println("Starting Ball Extraction");
      //diffsteering.Stop();
      delay(1000);
    Serial.println("Down");
      stepper.step(STEPPER_STEPS);
      delay(1000);
    Serial.println("Forward");
      //diffsteering.Forward(90);
      delay(2000);
      //diffsteering.Stop();
      //delay(1000);
    Serial.println("Side");
      //diffsteering.Turn_R(750, 0, 90);
      //diffsteering.Stop();
      delay(2000);
    Serial.println("Backward");
      //diffsteering.Backward(90);
      //delay(2000);
      //diffsteering.Stop();
    Serial.println("Up");
      stepper.step(-STEPPER_STEPS);
      delay(1000);
    Serial.println("Reset");
      long range_array_1[5] = {100,100,100,100,100}; // Reset
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
