#include "GamePlay.h"
#include "Arduino.h"
#include "constants.h"
#include "Basic.h"
#include "Wall.h"
#include "MyLineFollow.h"

void getBallcorner(Wheels& wheels, Stepper& stepper)
{
  wheels.Pivot_L(5);
  my_delay(500);
  interrupts();
  stepper.step(STEPPER_NSTEPS);
  my_delay(500);
  noInterrupts();
  wheels.Forward(100,50000);
  my_delay(250);
  wheels.Pivot_R(10);
  wheels.Forward(200);
  my_delay(2000);
  wheels.Pivot_R(15);
  wheels.Backward(150,50000);
  my_delay(500);
  interrupts();
  stepper.step(-STEPPER_NSTEPS-30);
  my_delay(1000);
  noInterrupts();
}

void getBall(int side, float& angle, Wheels& wheels, Stepper& stepper, Ultrasonic& front_US)
{
  forward_to_dist(HOPPER_STOP_DIST, wheels, front_US, BALL_GRAB_SPD);
  Serial.println("Pivot");
  wheels.Pivot_L(7);
  my_delay(1000);
  interrupts();
  Serial.println("Down");
  stepper.step(STEPPER_NSTEPS);
    my_delay(1000);
  Serial.println("Forward");
  //ForwardtillStop(wheels, BALL_GRAB_SPD);
  
  int photoresistor_array[ARRAY_LEN];
  for (int i=0; i<=(ARRAY_LEN*2); ++i) {
    int value = analogRead(A0);
    shift_add(photoresistor_array, value);
    my_delay(10); // my_delay disables interrupts after so use normal delay
  }
  
  int thresh = analogRead(A0);
  
  while (check_array(photoresistor_array, thresh+4) == 1) {
    wheels.Forward_straight(BALL_GRAB_SPD);
    int value = analogRead(A0);
    Serial.println(value);
    my_delay(10);
    shift_add(photoresistor_array, value);
  }
  //my_delay(1000);
  wheels.Stop();
    my_delay(1000);
  
  Serial.println("Pivot");
  wheels.Pivot_R(15);
  
  Serial.println("Back");
  wheels.Backward(100, 75000);
  //wheels.Back_L(BACK_TURN_MILLI, BACK_INNER_SPD, BACK_OUTER_SPD);
  //angle = angle - 45;
    my_delay(1000);
  interrupts();
  Serial.println("Up");
  stepper.step(-STEPPER_NSTEPS-30);
    my_delay(1000);
  noInterrupts();
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
  /*int photoresistor_array[ARRAY_LEN];
  for (int i=0; i<=(ARRAY_LEN*2); ++i) {
    int value = analogRead(A0);
    shift_add(photoresistor_array, value);
    my_delay(10);
  }
  int thresh = analogRead(A0);
  Serial.println(thresh);
  
  while ( (check_array(photoresistor_array, thresh-4) != 1) && (check_array(photoresistor_array, thresh+4) == 1) )
  {
    wheels.Forward_straight(SPD);
    int value = analogRead(A0);
    Serial.println(value);
    my_delay(10);
    shift_add(photoresistor_array, value);
  }
  wheels.Stop();*/
  wheels.encoder.reset();
  front_US.initialize_array();
  while (check_array(front_US.array, (long)MIN_DIST_DISPENSE) != 1) {
    front_US.update_array();
    wheels.Forward_straight(SPD);
    my_delay(50);
    noInterrupts();
  }
  wheels.Stop();
  
  my_delay(1000);
  
  backward_align(wheels);
  backward_align(wheels);
  wheels.Forward(100,5000);

  my_delay(250);
  num_dispensed++;
}

