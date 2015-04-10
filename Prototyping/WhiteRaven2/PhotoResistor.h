#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include "Encoder.h"

class PhotoResistor
{
  public:
    PhotoResistor(int PIN);
    int ball_present();
    
  private:
    const int resistor_pin;
};

#endif PHOTORESISTOR_H
