#include "Wheels.h"
#include "Encoder.h"
#include "constants.h"
#include "Ultrasonic.h"


Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
Wheels MyWheel(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);
Ultrasonic ultrasonic(22,23);

byte L_X_val, L_Y_val, num_ball_L, R_X_val, R_Y_val,num_ball_R;
byte CL_X_val, CL_Y_val, num_ball_CL, CR_X_val, CR_Y_val, num_ball_CR;
byte mode;
byte side;
byte num_inter = 0;
byte num_turn = 0;
long range_array[5] = {100,100,100,100,100};
int Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor;



void setup(){
  Serial.begin(9600);
  init_white_raven(&L_X_val,&L_Y_val,&num_ball_L);
  init_white_raven(&R_X_val,&R_Y_val,&num_ball_R);
  init_white_raven(&CL_X_val,&CL_Y_val,&num_ball_CL);
  init_white_raven(&CR_X_val,&CR_Y_val,&num_ball_CR); 
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  Serial.println("start...");
}

void loop(){
  
  backward_align(PLsensor, PRsensor);
  mode = choose_hopper(CL_Y_val, CL_X_val, num_ball_CL,CR_Y_val, CR_X_val, num_ball_CR, num_ball_L, num_ball_R);
  if (mode % 2 == 0)  MyWheel.Pivot_R(90);
  else MyWheel.Pivot_L(90);
  
  while(num_turn != 2){
    line_following(Lsensor,Rsensor,PLsensor,PRsensor);
  }
  num_turn = 0;
  MyWheel.Stop();
}

void doEncoder_L_A() {MyWheel.encoder.update_L_A();}
void doEncoder_L_B() {MyWheel.encoder.update_L_B();}
void doEncoder_R_A() {MyWheel.encoder.update_R_A();}
void doEncoder_R_B() {MyWheel.encoder.update_R_B();}

void line_following(int Lsensor, int Rsensor, int PLsensor, int PRsensor, byte mode){
  Serial.println("LINE FOLLOWING");
  Rsensor = analogRead(13);
  Lsensor = analogRead(14);
  PRsensor = analogRead(9);
  PLsensor = analogRead(8);  
  Serial.println("sensor readings: R,L,F,B,PR,PL");
  Serial.print("reading values:   ");
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.print(Lsensor);
  Serial.print(' ');
  Serial.print(PRsensor);
  Serial.print(' ');
  Serial.println(PLsensor);
  Serial.print("reference values: ");
  Serial.print(MaxR);
  Serial.print(' ');
  Serial.print(MaxL);
  Serial.print(' ');
  Serial.print(MaxPR);
  Serial.print(' ');
  Serial.println(MaxPL);
  if (Lsensor > (MaxL + 100) && Rsensor < (MaxR + 100))
  {
      MyWheel.Turn_L(50,0,90);
      Serial.println("1");
  }
  else if (Rsensor > (MaxR +100) && Lsensor < (MaxL + 100))
  {
    MyWheel.Turn_R(50,0,90);
    Serial.println("2");
  }
  else if (PRsensor > (MaxPR+100) || PLsensor > (MaxPL +100) )
    {  // @the intersection
      //double check the sensors
      long check_arrayPR[5] = {0,0,0,0,0};
      long check_arrayPL[5] = {0,0,0,0,0};
      int PRsensor_Inter;
      int PLsensor_Inter;
      for (int n = 0; n < 8; n++)
      {
         int PRsensor_Inter = analogRead(9);
         int PLsensor_Inter = analogRead(8); 
         shift_add(check_arrayPR, 5, PRsensor_Inter);   
         shift_add(check_arrayPL, 5, PLsensor_Inter);
      }
      // states equal to 1 if more than half of the readings are white 
      byte state1 = check_array(check_arrayPR, 5, (MaxPR+100));
      byte state2 = check_array(check_arrayPL, 5, (MaxPL+100));
      Serial.print("state 1 is: ");
      Serial.println(state1);
      Serial.print("state 2 is: ");
      Serial.println(state2);
      if (((PRsensor > (MaxPR+100)) && state1) || ((PLsensor > (MaxPL+100)) && state2) )  
      {
        num_inter++;
      }
      else if (state1 == 0 && state2 == 0)  num_inter++;
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(num_inter);
      Serial.print("number of turns made: ");
      Serial.println(num_turn);
      
      //Determine the direction
      byte DIRECT;
      switch(mode)
      {
        case 1:
          // the left center hopper
          DIRECT = input_coor(num_inter, num_turn, CL_X_val, CL_Y_val);
          break;
        case 2:
          // the right center hopper
          DIRECT = input_coor(num_inter, num_turn, CR_X_val, CR_Y_val);
          break;
        case 3:
          // the left side hopper
          DIRECT = input_coor(num_inter, num_turn, L_X_val, L_Y_val);
          break;
        case 4:
          // the right side hopper
          DIRECT = input_coor(num_inter, num_turn, R_X_val, R_Y_val);
          break;
      }
      //start turning...
      if(DIRECT == 1)
      {  //LEFT TURN
        num_inter = 0;
        Serial.println("3");
        MyWheel.Pivot_L(90);
        MyWheel.Forward(50,TURNING_OFFSET);
        num_turn += 1;
      }
      if (DIRECT == 2)
      {  //LEFT RIGHT
          num_inter = 0;
          Serial.println("4");
          MyWheel.Pivot_R(90);
          MyWheel.Forward(50,TURNING_OFFSET);
          num_turn += 1;
      }
      if (DIRECT == 0)
      {
        MyWheel.Forward(75);
        Serial.println("5");        
      }
    }
  else 
  {
    MyWheel.Forward(75);
    Serial.println("6");
  }
}

void backward_align(int PRsensor, int PLsensor){
  Serial.println("backward aligning");
  PRsensor = analogRead(9);
  PLsensor = analogRead(8); 
  while ((PRsensor < (MaxPR+50)) && (PLsensor < (MaxPL +50)))
  {
    PRsensor = analogRead(9);
    PLsensor = analogRead(8); 
    MyWheel.Backward(50);
    Serial.print(PRsensor);
    Serial.print(' ');
    Serial.println(PLsensor);
  }
  MyWheel.Forward(50,TURNING_OFFSET);
  MyWheel.Stop();
}
  
  
byte choose_hopper(byte CL_Y, byte CL_X, byte CL_B, byte CR_Y, byte CR_X, byte CR_B, byte SL_B, byte SR_B){
  byte side
  L_dist = L_Y + L_X;
  R_dist = R_Y + R_X;
  if (R_dist < L_dist && R_B > 0)  side = 2;
  else if (R_B == 0 && L_B > 0)  side = 1;
  else if (R_B == 0 && L_B == 0) side = 3
  else side = 4;
  return side 
}
    
void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

void init_white_raven(byte *x_val, byte *y_val, byte *num_ball){
  Serial.println("input x coordinate: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  *x_val = Serial.read() -48;
  Serial.println(*x_val);
  Serial.println("input y coordinate: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  *y_val = Serial.read() - 48;
  Serial.println(*y_val);
  Serial.println("input number of balls: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  *num_ball = Serial.read() - 48;
  Serial.println(*num_ball);
}

byte input_coor(byte num_inter, byte num_turn, byte x_val, byte y_val){
  byte direct = 0;
  if (num_inter == y_val && num_turn == 0  && num_inter != 0)
  {
    direct = 2;
  }
  else if (num_inter == x_val && num_turn == 1  && num_inter != 0)
  {
      direct = 1; 
  }
  MyWheel.Stop();
  return direct;
}

 
int check_array(long* arr, int length, long thresh) {
  
  int ret = 1;
  int count = 0;
  
  for (int i = 0; i != length; i++) {
    Serial.print(arr[i]);
    Serial.print("\t");
    if (arr[i] >= thresh) {
      count += 1;
    }
  }
  if (count >= length/2)
  {
    ret = 0;
  }
  Serial.print("\n");
  return ret;
}


int us_check(long* arr, int length, long thresh) {
  
  int ret = 1;
  
  for (int i = 0; i != length; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
    if (arr[i] >= thresh) {
      ret = 0;
    }
  }
  Serial.print("\n");
  return ret;
}
