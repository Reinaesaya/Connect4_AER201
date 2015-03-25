#include "GamePlay.h"
#include "Arduino.h"
#include "constants.h"
#include "Basic.h"
#include "Wall.h"

void getBall(int side, float& angle, Wheels& wheels, Stepper& stepper, Ultrasonic& front_US)
{
  forward_to_dist(HOPPER_STOP_DIST, wheels, front_US, BALL_GRAB_SPD);
  Serial.println("Forward");
  my_delay(1000);
  interrupts();
  Serial.println("Down");
  stepper.step(STEPPER_NSTEPS);
    my_delay(1000);
  Serial.println("Forward");
  //ForwardtillStop(wheels, BALL_GRAB_SPD);
  wheels.Forward(BALL_GRAB_SPD, HOPPER_FORWARD_TICK);
    my_delay(1000);
  Serial.println("Back");
  if (side == LEFT_BOARD)
  {
    wheels.Back_L(BACK_TURN_MILLI, BACK_INNER_SPD, BACK_OUTER_SPD);
    angle = angle - 45;
  }
  else if (side == RIGHT_BOARD)
  {
    wheels.Back_R(BACK_TURN_MILLI, BACK_INNER_SPD, BACK_OUTER_SPD);
    angle = angle + 45;
  }
    my_delay(1000);
  interrupts();
  Serial.println("Up");
  stepper.step(-STEPPER_NSTEPS);
    my_delay(1000);
}

void ForwardtillStop(Wheels& wheels, int SPD)
{
  interrupts();
  signed long int lastleftwheel = wheels.encoder.getPosLeft();
  signed long int lastrightwheel = wheels.encoder.getPosRight();
  wheels.Forward(SPD);
  my_delay(100);
  while ((lastleftwheel < wheels.encoder.getPosLeft()) && (lastrightwheel < wheels.encoder.getPosRight()))
  {
    wheels.Forward(SPD);
    my_delay(10);
  }
  wheels.Stop();
  noInterrupts();
}

int choose_column(int *dispense_order, int *dispense_count, int num_dispensed)
{
  int chosen;
  if (dispense_order[num_dispensed] == 0)
  {
    chosen = random(1,8);
    while (dispense_count[chosen-1] >= 5) { chosen = random(1,8); }
  }
  else { chosen = dispense_order[num_dispensed]; }
  dispense_count[chosen-1]++;
  return chosen;
}

void dispense(int& num_dispensed, Ultrasonic& front_US, Wheels wheels, int SPD)
{
  front_US.initialize_array();
  while (check_array(front_US.array, (long)MIN_DIST_DISPENSE) != 1) {
    front_US.update_array();
    wheels.Forward(SPD);
  }
  wheels.Stop();
  
  my_delay(1000);
  
  while (check_array(front_US.array, (long)BACK_UP_DISPENSE_DIST) == 1) {
    front_US.update_array();
    wheels.Backward(SPD);
  }
  wheels.Stop();
  num_dispensed++;
}

