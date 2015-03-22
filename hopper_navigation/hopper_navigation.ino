#include "Wheels.h"
#include "Encoder.h"
#include "constants.h"
#include "hopper.h"
#include "Ultrasonic.h"


Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels MyWheel(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);

byte L_X_val, L_Y_val, num_ball_L, R_X_val, R_Y_val,num_ball_R;
byte CL_X_val, CL_Y_val, num_ball_CL, CR_X_val, CR_Y_val, num_ball_CR;
byte mode[22] = {2,2,2,1,1,2,2,1,1,2,3,3,4,2,1,4,3,1,2,1,3,4};
byte side;
int ball_count = 0;
byte num_inter = 0;
byte num_turn = 0;
long range_array[5] = {100,100,100,100,100};
int Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor;

void setup(){
  Serial.begin(9600);
  //init_white_raven(&L_X_val,&L_Y_val,&num_ball_L);
  //init_white_raven(&R_X_val,&R_Y_val,&num_ball_R);
  //init_white_raven(&CL_X_val,&CL_Y_val,&num_ball_CL);
  init_white_raven(&CR_X_val,&CR_Y_val,&num_ball_CR); 
  ball_count = 0;
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  Serial.println("start...");
}

void loop(){
  int ball_dispensed = 0;/*22-(num_ball_L + num_ball_R + num_ball_CL + num_ball_CR);*/
  backward_align(MyWheel);
  int hopper = mode[ball_count+ball_dispensed];
  Serial.print("go to hopper #");
  Serial.println(hopper);
  if ((hopper % 2) == 0)  
  {
    MyWheel.Pivot_R(90); 
    MyWheel.Forward(50,TURNING_OFFSET);
    while(num_turn != 2){
    line_following(MyWheel,hopper, &num_inter, &num_turn,
                   CL_X_val, CL_Y_val,CR_X_val,CR_Y_val,L_X_val,L_Y_val, R_X_val,R_Y_val);
    }
    num_turn = 0;
    Serial.println("at the origin"); 
  }

  else if( (hopper%2) != 0)
  {
    MyWheel.Pivot_L(90);
    MyWheel.Forward(50,TURNING_OFFSET);
    while(num_turn != 2){
    line_following(MyWheel,hopper, &num_inter, &num_turn,
                   CL_X_val, CL_Y_val,CR_X_val,CR_Y_val,L_X_val,L_Y_val, R_X_val,R_Y_val);
    }
    num_turn = 0;
    Serial.println("at the origin"); 
  }
  MyWheel.Pivot_L(180);
  while(num_turn != 2){
    line_following(MyWheel,hopper, &num_inter, &num_turn,
                   CL_X_val, CL_Y_val,CR_X_val,CR_Y_val,L_X_val,L_Y_val, R_X_val,R_Y_val);
  }
  num_turn = 0;
  ball_count++;
  MyWheel.Stop();
}

void doEncoder_L_A() {MyWheel.encoder.update_L_A();}
void doEncoder_L_B() {MyWheel.encoder.update_L_B();}
void doEncoder_R_A() {MyWheel.encoder.update_R_A();}
void doEncoder_R_B() {MyWheel.encoder.update_R_B();}

