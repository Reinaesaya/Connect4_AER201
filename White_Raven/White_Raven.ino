#include "Basic.h"
#include "Wheels.h"
#include "Encoder.h"
#include "Ultrasonic.h"
#include <Stepper.h>
#include "Wall.h"
#include "Gameplay.h"
#include "constants.h"

// Instantiate Encoder and Wheels
Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels wheels(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

// Instantiate Ultrasonic uses
Ultrasonic front_ultra(FRONT_TRIG,FRONT_ECHO);
Ultrasonic left_ultra(LEFT_TRIG,LEFT_ECHO);
Ultrasonic right_ultra(RIGHT_TRIG,RIGHT_ECHO);

// Instantiate Stepper
Stepper stepper = Stepper(STEPPER_STEPS,STEPPER_1,STEPPER_2,STEPPER_3,STEPPER_4);

int dispense_order[28] = {4,5,4,3,3,3,6,4,5,2,2,5,2,6,6,1,7,1,7,1,7,4,3,5,2,6,1,7};
int dispense_count[7] = {0,0,0,0,0,0,0};
int num_dispensed = 0;

int go = 48;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter Number of Balls Dispensed");
  
  num_dispensed = get_input_number();
  
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  
  stepper.setSpeed(STEPPER_SPEED);
  
  noInterrupts();
}


void loop(){
  
//  while (num_dispensed < 28)
//  {
//    int num = choose_column(dispense_order, dispense_count, num_dispensed);
//    Serial.print(num_dispensed);
//    Serial.print(" ");
//    Serial.println(num);
//    
//    num_dispensed++;
//    my_delay(500);
//    //dispense(num_dispensed, front_ultra, wheels, 75);
//  }
  
//  if (Serial.available() > 0) {
//    go = Serial.read();
//    if (go == 48) {
//      wheels.Stop();
//    }
//  }
  //if (go != 48) {
    float angle = 45;
//    Serial.println("Starting");
//    get_to_gameboard(LEFT_BOARD, angle, 4, wheels, front_ultra, left_ultra, right_ultra);
//    dispense(front_ultra, wheels, 75);
    getBall(LEFT_BOARD, angle, wheels, stepper, front_ultra);
    while (true)
    {
      continue;
    }
//    interrupts();
//    Serial.println(go);
//    encoder.reset();
//    wheels.Pivot_L(180);
//    wheels.Stop();
//    delay(1000);
//    encoder.reset();
//    //noInterrupts();
    //go = 48;
  //}
}

void doEncoder_L_A() {wheels.encoder.update_L_A();}
void doEncoder_L_B() {wheels.encoder.update_L_B();}
void doEncoder_R_A() {wheels.encoder.update_R_A();}
void doEncoder_R_B() {wheels.encoder.update_R_B();}

void getBall(int side, float& angle, Wheels& wheels, Stepper& stepper, Ultrasonic& front_US)
{
  //forward_to_dist(HOPPER_STOP_DIST, wheels, front_US, BALL_GRAB_SPD);
  Serial.println("Starting Ball Extraction");
  my_delay(1000);
    //Serial.println("Down");
  interrupts();
  stepper.step(STEPPER_NSTEPS);
    my_delay(1000);
  /*  Serial.println("Forward");
  wheels.Forward(BALL_GRAB_SPD, HOPPER_FORWARD_TICK);
    my_delay(1000);
    Serial.println("Back");
  if (side == LEFT_BOARD)
  {
    wheels.Back_L(BACK_TURN_MILLI, BACK_INNER_SPD, BACK_OUTER_SPD);
    angle = angle - 45;
  }
  else if (side == RIGHT_BOARD)
  {
    wheels.Back_R(BACK_TURN_MILLI, BACK_INNER_SPD, BACK_OUTER_SPD);
    angle = angle + 45;
  }
    my_delay(1000);*/
    Serial.println("Up");
  interrupts();
  stepper.step(-STEPPER_NSTEPS);
    delay(1000);
    Serial.println("End Ball Extraction");
}
