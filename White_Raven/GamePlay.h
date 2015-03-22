#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Ultrasonic.h"
#include "Wheels.h"

int choose_column(int *dispense_order, int *dispense_count, int num_dispensed);

void dispense(int& num_dispensed, Ultrasonic& front_US, Wheels wheels, int SPD);

#endif
