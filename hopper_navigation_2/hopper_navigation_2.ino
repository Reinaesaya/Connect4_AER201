#include "Wheels.h"
#include "Encoder.h"
#include "constants.h"
#include "hopper.h"
#include "Ultrasonic.h"


Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels MyWheel(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

byte L_X_val, L_Y_val, num_ball_L, R_X_val, R_Y_val,num_ball_R;
byte CL_X_val, CL_Y_val, num_ball_CL, CR_X_val, CR_Y_val, num_ball_CR;
int angle_CL, angle_CR, angle_L, angle_R;
byte hopper_array[22] = {1,2,2,1,1,2,2,1,1,2,3,3,4,2,1,4,3,1,2,1,3,4};
byte side;
int ball_count = 0;
byte num_inter = 0;
byte num_turn = 0;
long range_array[5] = {100,100,100,100,100};
int Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor;

void setup(){
  Serial.begin(9600);
  init_white_raven(&L_X_val,&L_Y_val,&num_ball_L,&angle_L);
  init_white_raven(&R_X_val,&R_Y_val,&num_ball_R,&angle_R);
  init_white_raven(&CL_X_val,&CL_Y_val,&num_ball_CL,&angle_CL);
  init_white_raven(&CR_X_val,&CR_Y_val,&num_ball_CR,&angle_CR); 
  ball_count = 0;
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  Serial.println("start...");
  
}

void loop(){
  //int ball_dispensed = 0;/*22-(num_ball_L + num_ball_R + num_ball_CL + num_ball_CR);*/
  /*backward_align(MyWheel);
  int hopper = hopper_array[ball_count+ball_dispensed];
  Serial.print("go to hopper #");
  Serial.println(hopper);
  //Reset to origin
  if ((hopper % 2) == 0)  
  {
    //right board
    MyWheel.Pivot_R(90); 
    while(num_turn != 2){
    i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, 1, 3);
    }
    num_turn = 0;
    MyWheel.Pivot_L(180);
    Serial.println("at the origin");     
  }
  else if( (hopper%2) != 0)
  {
    //left board
    MyWheel.Pivot_L(90);
    while(num_turn != 2){
    i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, 1, 3);
    }
    num_turn = 0;
    MyWheel.Pivot_L(180);
    Serial.println("at the origin"); 
  }
  
  //Navigate to the hopper
  while(num_turn != 2){
    switch(hopper)
      {
        case 1:
          // the left center hopper
          i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, CL_X_val, CL_Y_val);
          break;
        case 2:
          // the right center hopper
          i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, CR_X_val, CR_Y_val);
          break;
        case 3:
          // the left side hopper
          i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, L_X_val, L_Y_val);
          break;
        case 4:
          // the right side hopper
          i_hopper_navigation(MyWheel, hopper, &num_inter, &num_turn, R_X_val, R_Y_val);
          break;
      }
  }
  num_turn = 0;
  int turning = choose_angle(hopper, angle_L, angle_R, angle_CL, angle_CR);
  MyWheel.Pivot_R(turning);
  ball_count++;
  Serial.print("number of ball dispensed: ");
  Serial.println(ball_count);
  MyWheel.Stop();*/
  while(num_turn != 2){
    i_hopper_navigation(MyWheel, 2, &num_inter, &num_turn, CL_X_val, CL_Y_val);
    }
  while(1){
  MyWheel.Stop();}
}

void doEncoder_L_A() {MyWheel.encoder.update_L_A();}
void doEncoder_L_B() {MyWheel.encoder.update_L_B();}
void doEncoder_R_A() {MyWheel.encoder.update_R_A();}
void doEncoder_R_B() {MyWheel.encoder.update_R_B();}

