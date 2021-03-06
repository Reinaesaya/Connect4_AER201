#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Ultrasonic.h"
#include "Wheels.h"
#include <Stepper.h>

void getBallcorner(Wheels& wheels, Stepper& stepper);
void getBall(int side, float& angle, Wheels& wheels, Stepper& stepper, Ultrasonic& front_US);
void ForwardtillStop(Wheels& wheels, int SPD);

int choose_column(int *dispense_order, int *dispense_count, int num_dispensed);

void dispense(int& num_dispensed, Ultrasonic& front_US, Wheels wheels, int SPD);

#endif
