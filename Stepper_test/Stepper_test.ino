#include <Stepper.h>

Stepper stepper = Stepper(48, 8,9);

void setup() {
  stepper.setSpeed(20);
}

void loop() {
  stepper.step(2);
  delay(500);
}
