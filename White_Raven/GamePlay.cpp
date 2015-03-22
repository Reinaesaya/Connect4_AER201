#include "GamePlay.h"
#include "Arduino.h"
#include "constants.h"
#include "Basic.h"
#include "Wall.h"

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

