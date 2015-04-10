#include "Basic.h"
#include "Wheels.h"
#include "Encoder.h"
#include "Ultrasonic.h"
#include <Stepper.h>
#include "Wall.h"
#include "GamePlay.h"
#include "MyLineFollow.h"
#include "Hopper.h"
#include "constants.h"
#include <Keypad.h>

// Initialize keypad
const byte KEYROWS = 4;
const byte KEYCOLS = 4;
char hexaKeys[KEYROWS][KEYCOLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[KEYROWS] = {ROW_PIN_D, ROW_PIN_C, ROW_PIN_B, ROW_PIN_A};
byte colPins[KEYCOLS] = {COL_PIN_D, COL_PIN_C, COL_PIN_B, COL_PIN_A};
Keypad MyKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, KEYROWS, KEYCOLS);

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

int hopper_order[28]   = {0,0,0,0,3,3,3,3,1,2,1,2,1,2,1,2,0,1,2,3,0,1,2,3,0,1,2,3};
int dispense_order[28] = {4,5,4,3,3,3,6,4,5,2,2,5,2,6,6,1,7,1,7,1,7,4,3,5,2,6,1,7};
int dispense_count[7] = {0,0,0,0,0,0,0};
int num_dispensed = 0;
int left_up = 1;
float angle = 0;
byte first_ball = 1;

int go = 48;

void setup() {
  Serial.begin(9600);
  
//  MyKeypad.setDebounceTime(500);
//  Serial.println("Enter Number of Balls Dispensed");
//  num_dispensed = get_input_number(MyKeypad);
//  Serial.println("Enter Left Hopper Orientation (Flat Up = 0, Flat Down = 1)");
//  int L_Orient = get_input_number(MyKeypad);
//  Serial.println("Enter Left Hopper Center Relation (Close = 0, Far = 1)");
//  int L_Close = get_input_number(MyKeypad);
//  Serial.println("Enter Right Hopper Orientation (Flat Up = 0, Flat Down = 1)");
//  int R_Orient = get_input_number(MyKeypad);
//  Serial.println("Enter Right Hopper Center Relation (Close = 0, Far = 1)");
//  int R_Close = get_input_number(MyKeypad);
  Serial.println("Enter Left-Up (1) or Left_Down (0)");
  left_up = get_input_number(MyKeypad);
  
  num_dispensed = 0;
  int L_Orient = 0;
  int L_Close = 1;
  int R_Orient = 0;
  int R_Close = 1;
  //left_up = 1;
  
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
    if (first_ball)
    {
      forward_start(left_up, wheels);
      if (left_up)
      {
        wheels.Pivot_L(45);
        angle = 45;
      }
      else
      {
        wheels.Pivot_R(45);
        angle = 135;
      }
      getBallcorner(wheels, stepper);
      update_angle(angle);
      if (left_up)
      {
        get_to_gameboard(LEFT_BOARD, angle, 4, wheels, front_ultra, left_ultra, right_ultra);
      }
      else
      {
        get_to_gameboard(RIGHT_BOARD, angle, 4, wheels, front_ultra, left_ultra, right_ultra);
      }
      dispense(num_dispensed, front_ultra, wheels, 75);
      get_to_origin_from_gameboard(wheels, hoppers[hopper_order[num_dispensed]]);
      first_ball = 0;
    }
    get_to_hopper_pos(wheels, hoppers[hopper_order[num_dispensed]], front_ultra, right_ultra, left_ultra, angle);
    //get_to_corner_hopper(hoppers[hopper_order[num_dispensed]], angle, wheels, front_ultra, left_ultra, right_ultra);
    getBallcorner(wheels, stepper);
    update_angle(angle);
    int num = choose_column(dispense_order, dispense_count, num_dispensed);
    get_to_gameboard(hoppers[hopper_order[num_dispensed]].side, angle, num, wheels, front_ultra, left_ultra, right_ultra);
    dispense(num_dispensed, front_ultra, wheels, 75);
    get_to_origin_from_gameboard(wheels, hoppers[hopper_order[num_dispensed]]);
}


void doEncoder_L_A() {wheels.encoder.update_L_A();}
void doEncoder_L_B() {wheels.encoder.update_L_B();}
void doEncoder_R_A() {wheels.encoder.update_R_A();}
void doEncoder_R_B() {wheels.encoder.update_R_B();}
