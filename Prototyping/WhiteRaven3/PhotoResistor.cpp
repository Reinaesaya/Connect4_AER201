/* PhotoResistor Library */

#include "PhotoResistor.h"
#include "Arduino.h"

PhotoResistor::PhotoResistor(int PIN): resistor_pin (PIN)
{
  pinMode(resistor_pin, INPUT);
}

int PhotoResistor::ball_present()
{
}
