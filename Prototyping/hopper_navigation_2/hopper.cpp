#include "Wheels.h"
#include "hopper.h"
#include "Encoder.h"
#include "constants.h"

void i_hopper_navigation(Wheels& MyWheel, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val){
  int PRsensor = analogRead(9);
  int PLsensor = analogRead(8); 
  Serial.print("reading values:   ");
  Serial.print(PRsensor);
  Serial.print(' ');
  Serial.println(PLsensor);
  Serial.print("reference values: ");
  Serial.print(MaxPR);
  Serial.print(' ');
  Serial.println(MaxPL); 
  if (y_val == 0)
  {
    first_turn(MyWheel, num_inter, num_turn, mode, x_val, y_val);
  }
  else if((PRsensor < (MaxPR+50)) && (PLsensor < (MaxPL +50)))  line_following(MyWheel);
  else if (PRsensor > (MaxPR+100) || PLsensor > (MaxPL +100) )
  {  // @the intersection
  
      //double check the sensors
      long check_arrayPR[5] = {0,0,0,0,0};
      long check_arrayPL[5] = {0,0,0,0,0};
      for (int n = 0; n < 8; n++)
      {
         int PRsensor_Inter = analogRead(9);
         int PLsensor_Inter = analogRead(8); 
         shift_add(check_arrayPR, 5, PRsensor_Inter);   
         shift_add(check_arrayPL, 5, PLsensor_Inter);
      }
      // states equal to 1 if more than half of the readings are white 
      byte state1 = check_array(check_arrayPR, 5, (MaxPR+100));
      Serial.print("state 1 is: ");
      Serial.println(state1);
      byte state2 = check_array(check_arrayPL, 5, (MaxPL+100));
      Serial.print("state 2 is: ");
      Serial.println(state2);
      if ( ( (PRsensor > (MaxPR+100) ) && state1) || ( (PLsensor > (MaxPL+100) ) && state2) )  (*num_inter)++;
      else if (state1 == 0 && state2 == 0)  (*num_inter)++;
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(*num_inter);
      Serial.print("number of turns made: ");
      Serial.println(*num_turn);    
      if ( *num_inter == y_val && *num_turn == 0)
      {
        first_turn(MyWheel,num_inter, num_turn, mode, x_val,y_val);
      }
     else if ( *num_inter == x_val&& *num_turn == 1)
      {
        if (mode % 2 == 0)  
        {
          //TURN RIGHT
          *num_inter = 0;
          Serial.println("4");
          MyWheel.Pivot_R(90);
          MyWheel.Forward(75,TURNING_OFFSET);
          (*num_turn) ++;
        }
        else if (mode % 2 != 0) 
        {
          //TURN LEFT
          (*num_inter) = 0;
          Serial.println("3");
          MyWheel.Pivot_L(90);
          MyWheel.Forward(75,TURNING_OFFSET);
          (*num_turn) += 1;
        }
      }
      else
      {
        MyWheel.Forward(75);
        Serial.println("5");        
      }
    }
}

void first_turn(Wheels& MyWheel,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val){
  if (mode % 2 == 0)  
    { //Right Board
      //TURN LEFT
      (*num_inter) = 0;
      Serial.println("3");
      MyWheel.Pivot_L(90);
      MyWheel.Forward(75,TURNING_OFFSET);
      (*num_turn) += 1;
      if(x_val == 0)
      {
        *num_inter = 0;
        Serial.println("4");
        MyWheel.Pivot_R(90);
        MyWheel.Forward(75,TURNING_OFFSET);
        (*num_turn) ++;
      }
    }
  else if (mode % 2 != 0) 
  {  //Left Board
    //TURN RIGHT
    *num_inter = 0;
    Serial.println("4");
    MyWheel.Pivot_R(90);
    MyWheel.Forward(75,TURNING_OFFSET);
    (*num_turn) ++;
    if(x_val == 0)
    {
      *num_inter = 0;
      Serial.println("4");
      MyWheel.Pivot_R(90);
      MyWheel.Forward(75,TURNING_OFFSET);
      (*num_turn) ++;
    }
  }
}
void line_following(Wheels& MyWheel){
  Serial.println("LINE FOLLOWING...");
  int Rsensor = analogRead(13);
  int Lsensor = analogRead(14); 
  Serial.println("sensor readings: R,L");
  Serial.print("reading values:   ");
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.println(Lsensor);
  Serial.print("reference values: ");
  Serial.print(MaxR);
  Serial.print(' ');
  Serial.println(MaxL);
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
    else 
  {
    MyWheel.Forward(75);
    Serial.println("6");
  }
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

int choose_angle(int hopper, int L, int R, int CL, int CR){
  int angle;
  switch(hopper)
  {
    case 1:
      angle = CL;
      break;
    case 2:
      angle = CR+180;
      break;
    case 3:
      angle = L;
      break;
    case 4:
      angle = R+180;
      break;
  }
  return angle;
}

      
void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

void init_white_raven(byte *x_val, byte *y_val, byte *num_ball, int *angle){
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
  Serial.println("input angle of turning: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  *angle = Serial.read() - 48;
  Serial.println(*angle);
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
