#include "Wheels.h"
#include "Encoder.h"
#include "Ultrasonic.h"
#include "constants.h"

// Instantiate Encoder and Wheels
Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels wheels(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

// Instantiate Ultrasonic uses
Ultrasonic front_ultra(FRONT_TRIG,FRONT_ECHO);
Ultrasonic left_ultra(LEFT_TRIG,LEFT_ECHO);
Ultrasonice right_ultra(RIGHT_TRIG,RIGHT_ECHO);
long range_arr_f[RANGE_ARRAY_LEN];
long range_arr_l[RANGE_ARRAY_LEN];
long range_arr_r[RANGE_ARRAY_LEN];

int go = 48;

void setup() {
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  Serial.begin(9600);
}


void loop(){ 
  if (Serial.available() > 0) {
    go = Serial.read();
    if (go == 48) {
      wheels.Stop();
    }
  }
  if (go != 48) {
    encoder.reset();
    wheels.Pivot_L(180);
    wheels.Stop();
    delay(1000);
    encoder.reset();
    go = 48;
  }
}

void doEncoder_L_A() {wheels.encoder.update_L_A();}
void doEncoder_L_B() {wheels.encoder.update_L_B();}
void doEncoder_R_A() {wheels.encoder.update_R_A();}
void doEncoder_R_B() {wheels.encoder.update_R_B();}
