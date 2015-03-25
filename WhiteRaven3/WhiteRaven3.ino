#include "Basic.h"
#include "Wheels.h"
#include "Encoder.h"
#include "Ultrasonic.h"
#include <Stepper.h>
#include "Wall.h"
#include "GamePlay.h"
#include "MyLineFollow.h"
#include "grid.h"
#include "Hopper.h"
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

// Instantiate Hoppers
Hopper H1(0);
Hopper H2(0);
Hopper H3(0);
Hopper H4(0);
Hopper hoppers[4] = {H1, H2, H3, H4};

int hopper_order[28]   = {0,0,0,3,3,3,1,2,1,2,1,2,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
int dispense_order[28] = {4,5,4,3,3,3,6,4,5,2,2,5,2,6,6,1,7,1,7,1,7,4,3,5,2,6,1,7};
int dispense_count[7] = {0,0,0,0,0,0,0};
int num_dispensed = 0;
byte L_X_val, L_Y_val, num_ball_L, R_X_val, R_Y_val,num_ball_R;
byte CL_X_val, CL_Y_val, num_ball_CL, CR_X_val, CR_Y_val, num_ball_CR;
float angle_CL, angle_CR, angle_L, angle_R;
byte hopper_array[22] = {1,2,2,1,1,2,2,1,1,2,3,3,4,2,1,4,3,1,2,1,3,4};
byte side;
int ball_count = 0;
byte num_inter = 0;
byte num_turn = 0;
long range_array[5] = {100,100,100,100,100};
int go = 48;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter Number of Balls Dispensed");
  num_dispensed = get_input_number();
  Serial.println("Enter Left Hopper Orientation (Flat Up = 0, Flat Down = 1)");
  int L_Orient = get_input_number();
  Serial.println("Enter Left Hopper Center Relation (Close = 0, Far = 1)");
  int L_Close = get_input_number();
  Serial.println("Enter Right Hopper Orientation (Flat Up = 0, Flat Down = 1)");
  int R_Orient = get_input_number();
  Serial.println("Enter Right Hopper Center Relation (Close = 0, Far = 1)");
  int R_Close = get_input_number();
  
  //input coordinates of the hoppers
  init_white_raven(&L_X_val,&L_Y_val,&num_ball_L,&angle_L);
  init_white_raven(&R_X_val,&R_Y_val,&num_ball_R,&angle_R);
  init_white_raven(&CL_X_val,&CL_Y_val,&num_ball_CL,&angle_CL);
  init_white_raven(&CR_X_val,&CR_Y_val,&num_ball_CR,&angle_CR); 
  ball_count = 0;
  
  hoppers[0].update(LEFT_BOARD, CORNER_HOP, 0,0);
  hoppers[1].update(LEFT_BOARD, NOT_CORNER_HOP, L_Orient, L_Close);
  hoppers[2].update(RIGHT_BOARD, NOT_CORNER_HOP, R_Orient, R_Close);
  hoppers[3].update(RIGHT_BOARD, CORNER_HOP, 0,0);
 
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  
  stepper.setSpeed(STEPPER_SPEED);
  
  noInterrupts();
}


void loop() {
    int num;
    if (ball_count == 0)
    {
      Serial.println("first ball");
      float angle = 225;
      i_hopper_navigation(wheels, 4, &num_turn, &num_inter, 2, 1, 45);
      getBall(side, angle, wheels, stepper, front_ultra);
      my_delay(500);
      update_angle(angle);
      num = choose_column(dispense_order, dispense_count, num_dispensed);
      my_delay(500);
      get_to_gameboard(side, angle, num, wheels, front_ultra, left_ultra, right_ultra);
      dispense(num_dispensed, front_ultra, wheels, 75);
      my_delay(500);
    }
      
    int ball_dispensed = 22-(num_ball_L + num_ball_R + num_ball_CL + num_ball_CR);
    int hopper = hopper_array[ball_count+ball_dispensed];
    
    Serial.println("Reset to origin");
    reset_origin(wheels, hopper, &num_inter, &num_turn);
    
    //Navigate to the hopper
    Serial.println("Get to Hopper #");
    Serial.println(hopper);
    byte x_val, y_val;
    float angle;
    int  side;
    switch(hopper)
      {
        case 1:
          // the left center hopper
          {x_val=CL_X_val; y_val = CL_Y_val, angle = angle_CL, side = LEFT_BOARD;
          break;}
        case 2:
          // the right center hopper
          {x_val=CR_X_val; y_val = CR_Y_val, angle = angle_CR, side = RIGHT_BOARD;
          break;}
        case 3:
          // the left side hopper
          {x_val=L_X_val; y_val = L_Y_val, angle = angle_L, side = LEFT_BOARD;
          break;}
        case 4:
          // the right side hopper
          {x_val=R_X_val; y_val = R_Y_val, angle = angle_R, side = RIGHT_BOARD;
          break;}
      }
    i_hopper_navigation(wheels, hopper, &num_inter, &num_turn, x_val, y_val, angle);
    ball_count++;
    Serial.print("number of ball dispensed: ");
    Serial.println(ball_count);
    Serial.println("GetBall");
    getBall(side, angle, wheels, stepper, front_ultra);
    my_delay(500);
    Serial.println("Update Angle");
    update_angle(angle);
    Serial.println(angle);
    Serial.print("Choose column: ");
    num = choose_column(dispense_order, dispense_count, num_dispensed);
    Serial.println(num);
    my_delay(500);
    Serial.println("Get to Gameboard");
    get_to_gameboard(side, angle, num, wheels, front_ultra, left_ultra, right_ultra);
    Serial.println("Dispense");
    dispense(num_dispensed, front_ultra, wheels, 75);
    my_delay(500);
    while (true)
    {
      continue;
    }
}


void doEncoder_L_A() {wheels.encoder.update_L_A();}
void doEncoder_L_B() {wheels.encoder.update_L_B();}
void doEncoder_R_A() {wheels.encoder.update_R_A();}
void doEncoder_R_B() {wheels.encoder.update_R_B();}
