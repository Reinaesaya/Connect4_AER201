#include "Wheels.h"
#include "Encoder.h"
#include "Ultrasonic.h"
#include "Wall.h"
#include "constants.h"

// Instantiate Encoder and Wheels
Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels wheels(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

// Instantiate Ultrasonic uses
Ultrasonic front_ultra(FRONT_TRIG,FRONT_ECHO);
Ultrasonic left_ultra(LEFT_TRIG,LEFT_ECHO);
Ultrasonic right_ultra(RIGHT_TRIG,RIGHT_ECHO);
long range_arr_f[RANGE_ARRAY_LEN];
long range_arr_l[RANGE_ARRAY_LEN];
long range_arr_r[RANGE_ARRAY_LEN];

int go = 48;

void setup() {
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  //noInterrupts();
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
    get_to_gameboard(RIGHT_BOARD, 135, 4, wheels, front_ultra, left_ultra, right_ultra, range_arr_f, range_arr_l, range_arr_r);
//    interrupts();
//    Serial.println(go);
//    encoder.reset();
//    wheels.Pivot_L(180);
//    wheels.Stop();
//    delay(1000);
//    encoder.reset();
//    //noInterrupts();
    go = 48;
  }
}

void doEncoder_L_A() {wheels.encoder.update_L_A();}
void doEncoder_L_B() {wheels.encoder.update_L_B();}
void doEncoder_R_A() {wheels.encoder.update_R_A();}
void doEncoder_R_B() {wheels.encoder.update_R_B();}
