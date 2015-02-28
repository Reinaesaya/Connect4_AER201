#include <Stepper.h>

Stepper stepper = Stepper(48, 8,7,6,5);

void setup() {
  stepper.setSpeed(20);
}

void loop() {
  stepper.step(2);
  delay(500);
}
