#include "Wheels.h"
#include "hopper.h"
#include "Encoder.h"
#include "constants.h"


void line_following(Wheels& MyWheel, byte mode, byte *num_inter, byte *num_turn, 
                    byte CL_X_val, byte CL_Y_val,byte CR_X_val,byte CR_Y_val,byte L_X_val,byte L_Y_val,byte R_X_val,byte R_Y_val){
  Serial.println("LINE FOLLOWING...");
  int Rsensor = analogRead(13);
  int Lsensor = analogRead(14);
  int PRsensor = analogRead(9);
  int PLsensor = analogRead(8);  
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
        *num_inter++;
      }
      else if (state1 == 0 && state2 == 0)  *num_inter++;
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(*num_inter);
      Serial.print("number of turns made: ");
      Serial.println(*num_turn);
      
      //Determine the direction
      byte DIRECT = direct_det(MyWheel, mode, CL_X_val, CL_Y_val,CR_X_val,CR_Y_val,L_X_val,L_Y_val, R_X_val,R_Y_val,*num_turn,*num_inter);
      //start turning...
      if(DIRECT == 1)
      {  //LEFT TURN
        *num_inter = 0;
        Serial.println("3");
        MyWheel.Pivot_L(90);
        MyWheel.Forward(50,TURNING_OFFSET);
        *num_turn += 1;
      }
      if (DIRECT == 2)
      {  //LEFT RIGHT
          *num_inter = 0;
          Serial.println("4");
          MyWheel.Pivot_R(90);
          MyWheel.Forward(50,TURNING_OFFSET);
          *num_turn += 1;
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

byte direct_det(Wheels& MyWheel, byte mode, byte CL_X_val, byte CL_Y_val,byte CR_X_val,byte CR_Y_val,byte L_X_val,byte L_Y_val,byte R_X_val,byte R_Y_val,
                byte num_turn, byte num_inter){
        //Determine the direction
      byte DIRECT;
      switch(mode)
      {
        case 1:
          // the left center hopper
          DIRECT = input_coor(MyWheel, num_inter, num_turn, CL_X_val, CL_Y_val);
          break;
        case 2:
          // the right center hopper
          DIRECT = input_coor(MyWheel, num_inter, num_turn, CR_X_val, CR_Y_val);
          break;
        case 3:
          // the left side hopper
          DIRECT = input_coor(MyWheel, num_inter, num_turn, L_X_val, L_Y_val);
          break;
        case 4:
          // the right side hopper
          DIRECT = input_coor(MyWheel, num_inter, num_turn, R_X_val, R_Y_val);
          break;
        case 5:
          DIRECT = input_coor(MyWheel, num_inter, num_turn, 0,3);
      }
      return DIRECT;
}


void backward_align(Wheels& MyWheel){
  Serial.println("backward aligning...");
  int PRsensor = analogRead(9);
  int PLsensor = analogRead(8);  
  while ((PRsensor < (MaxPR+50)) && (PLsensor < (MaxPL +50)))
  {
    PRsensor = analogRead(9);
    PLsensor = analogRead(8); 
    MyWheel.Backward(50);
    Serial.print(PRsensor);
    Serial.print(' ');
    Serial.println(PLsensor);
  }
  MyWheel.Stop();
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

byte input_coor(Wheels& MyWheel, byte num_inter, byte num_turn, byte x_val, byte y_val){
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
