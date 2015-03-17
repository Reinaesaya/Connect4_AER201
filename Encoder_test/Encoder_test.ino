#include "Diff_steer.h"
#include "Encoder.h"

DiffSteering diffsteering(13,12,11,10,9);
int go = 48;

enum PinAssignments {
  RPinA       = 21,
  RPinA_int   = 2,
  RPinB       = 20,
  RPinB_int   = 3,
  LPinA      = 19,
  LPinA_int  = 4,
  LPinB      = 18,
  LPinB_int  = 5,
};

Encoder encoder(LPinA, LPinB, RPinA, RPinB);

signed long int lastL = 1;
signed long int lastR = 1;

void setup() {

  attachInterrupt(LPinA_int, doEncoder_L_A, CHANGE);
  attachInterrupt(LPinB_int, doEncoder_L_B, CHANGE);
  attachInterrupt(RPinA_int, doEncoder_R_A, CHANGE);
  attachInterrupt(RPinB_int, doEncoder_R_B, CHANGE);
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
    diffsteering.Pivot_R(100);
    diffsteering.Stop();
    delay(1000);
    Serial.println(encoder.getPosLeft());
    Serial.println(encoder.getPosRight());
    Serial.println(encoder.getPivotAngle());
    encoder.reset();
    go = 48;
  }
  
  if ((lastL != encoder.getPosLeft()) || (lastR != encoder.getPosRight())) {
    Serial.print("Index:");
    Serial.print(encoder.getPosLeft(), DEC);
    Serial.print(" ");
    Serial.print(encoder.getPosRight(), DEC);
    Serial.println();
    lastL = encoder.getPosLeft();
    lastR = encoder.getPosRight();
  }
}

void doEncoder_L_A() {encoder.update_L_A();}
void doEncoder_L_B() {encoder.update_L_B();}
void doEncoder_R_A() {encoder.update_R_A();}
void doEncoder_R_B() {encoder.update_R_B();}
