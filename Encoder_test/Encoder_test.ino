#include "Diff_steer.h"
#include "Encoder.h"
#include "constants.h"

Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
DiffSteering diffsteering(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

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
      diffsteering.Stop();
    }
  }
  if (go != 48) {
    encoder.reset();
    diffsteering.Pivot_L(60);
    diffsteering.Stop();
    delay(1000);
    encoder.reset();
    go = 48;
  }
}

void doEncoder_L_A() {diffsteering.encoder.update_L_A();}
void doEncoder_L_B() {diffsteering.encoder.update_L_B();}
void doEncoder_R_A() {diffsteering.encoder.update_R_A();}
void doEncoder_R_B() {diffsteering.encoder.update_R_B();}
