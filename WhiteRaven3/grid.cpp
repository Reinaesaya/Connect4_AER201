#include "Wheels.h"
#include "grid.h"
#include "Encoder.h"
#include <Keypad.h>
#include "constants.h"


void init_white_raven(Keypad& MyKeypad, byte *x_val, byte *y_val, byte *num_ball, float *angle){
  Serial.println("input x coordinate: ");
  *x_val = MyKeypad.waitForKey() - 48;
  Serial.println(*x_val);
  Serial.println("input y coordinate: ");
  *y_val = MyKeypad.waitForKey() - 48;
  Serial.println(*y_val);
  Serial.println("input number of balls: ");
  *num_ball = MyKeypad.waitForKey() - 48;
  Serial.println(*num_ball);
  Serial.println("input angle of turning: ");
  *angle = MyKeypad.waitForKey() -48;
  Serial.println(*angle);
}

 
void i_hopper_navigation(Wheels& wheels, byte mode, byte *num_inter, byte *num_turn, byte x_val, byte y_val,float angle){
  Serial.println("i_hopper_navigating");
  int PRsensor = analogRead(9);
  int PLsensor = analogRead(8); 
  Serial.print("reading values:  ");
  Serial.print(PRsensor);
  Serial.print(' ');
  Serial.println(PLsensor);
  Serial.print("reference values: ");
  Serial.print(MAX_WHITE_B_R);
  Serial.print(' ');
  Serial.println(MAX_WHITE_B_L); 
  //wheels.encoder.reset();
  while(*num_turn != 2)
  {
    PRsensor = analogRead(9);
    PLsensor = analogRead(8); 
    Serial.print("reading values:  ");
    Serial.print(PRsensor);
    Serial.print(' ');
    Serial.println(PLsensor);
    Serial.print("reference values: ");
    Serial.print(MAX_WHITE_B_R);
    Serial.print(' ');
    Serial.println(MAX_WHITE_B_L); 
    interrupts();
    signed long int pos =wheels.encoder.getPosRight() + wheels.encoder.getPosLeft();
    noInterrupts();
    if (x_val == 0)
    {
      first_turn(wheels, num_inter, num_turn, mode, x_val, y_val);
    }
    if (PRsensor > (MAX_WHITE_B_R+100) || PLsensor > (MAX_WHITE_B_L +100) )
    {  // @the intersection
        //double check the sensors
        long check_arrayPR[5] = {0,0,0,0,0};
        long check_arrayPL[5] = {0,0,0,0,0};
        for (int n = 0; n < 10; n++)
        {
           int PRsensor_Inter = analogRead(9);
           int PLsensor_Inter = analogRead(8); 
           shift_add(check_arrayPR, 5, PRsensor_Inter);   
           shift_add(check_arrayPL, 5, PLsensor_Inter);
        }
        // states equal to 1 if more than half of the readings are white 
        byte state1 = check_array(check_arrayPR, 5, MAX_WHITE_B_R);
        Serial.print("state 1 is: ");
        Serial.println(state1);
        byte state2 = check_array(check_arrayPL, 5, MAX_WHITE_B_L);
        Serial.print("state 2 is: ");
        Serial.println(state2);
        if ( ( (PRsensor > (MAX_WHITE_B_R+50) ) && state1) || ( (PLsensor > (MAX_WHITE_B_L+50) ) && state2) )  {(*num_inter)++; wheels.encoder.reset();}
        else if (state1 == 0 && state2 == 0)  {(*num_inter)++; wheels.encoder.reset();}
        else if (PRsensor > (MAX_WHITE_B_R +30) && PLsensor > (MAX_WHITE_B_L + 30)) {(*num_inter) ++; wheels.encoder.reset();}
        else if (pos > 65800)  {(*num_inter)++; wheels.encoder.reset();}
        wheels.Stop();
        Serial.print("number of inters encounterted: ");
        Serial.println(*num_inter);
        Serial.print("number of turns made: ");
        Serial.println(*num_turn);    
        if ( *num_inter == x_val && *num_turn == 0)
        {
          first_turn(wheels,num_inter, num_turn, mode, x_val,y_val);
        }
       else if ( *num_inter == y_val && *num_turn == 1)
        {
          if (mode % 2 == 0)  
          {
            //RIGHT BOARD
            *num_inter = 0;
            Serial.println("4");
            wheels.Pivot_L(90);
            (*num_turn) ++;
          }
          else if (mode % 2 != 0) 
          {
            //LEFT BOARD
            (*num_inter) = 0;
            Serial.println("3");
            wheels.Pivot_R(90);
            (*num_turn) += 1;
          }
        }
        else
        {
          wheels.Forward(LINE_FOLLOW_SPEED);
          Serial.println("5");        
        }
      }
      line_following(wheels);
  }
  *num_turn = 0;
  Serial.println("near the hopper");
  if (mode % 2 == 0)
  {
    delay(10);
    if (angle > 180)  wheels.Pivot_R(angle-180);
    else if(angle != 0) wheels.Pivot_L(180-angle);
    else wheels.Stop();
  }
  else if(mode %2 != 0)
  {
    if(angle >180)  wheels.Pivot_R(360-angle);
    else if(angle != 0) wheels.Pivot_L(angle);
    else wheels.Stop();
  }
  delay(1000);
  return;
}

void first_turn(Wheels& wheels,  byte *num_inter, byte *num_turn, byte mode, byte x_val, byte y_val){
  if (mode % 2 == 0)  
    { //Right Board
      //TURN LEFT
      (*num_inter) = 0;
      Serial.println("3");
      wheels.Pivot_R(90);
      if(y_val != 0){
        for (int i = 0; i <5 ; i++)  
        {
        line_following(wheels); 
        Serial.println("turining delay");
        }
      }
      (*num_turn) += 1;
      if(y_val == 0)
      {
        *num_inter = 0;
        Serial.println("4");
        wheels.Pivot_L(90);
        (*num_turn) ++;
      }
    }
  if (mode % 2 != 0) 
  {  //Left Board
    //TURN RIGHT
    *num_inter = 0;
    Serial.println("4");
    wheels.Pivot_L(90);
    if(y_val != 0){
      for (int i = 0; i <5 ; i++)  
      {
        line_following(wheels); 
        Serial.println("turining delay");
      }
    }
    (*num_turn) ++;
    if(y_val == 0)
    {
      *num_inter = 0;
      Serial.println("4");
      wheels.Pivot_R(90);
      (*num_turn) ++;
    }
  }
}
void reset_origin(Wheels& wheels, byte hopper, byte *num_inter, byte *num_turn){
  backward_align(wheels);
  
  if ((hopper % 2) == 0)  
  {
    //right board
    wheels.Pivot_R(90); 
    i_hopper_navigation(wheels, hopper, num_inter, num_turn, 0, 3, 0);
    wheels.Pivot_R(90);
    Serial.println("at the origin");     
  }
  else if( (hopper%2) != 0)
  {
    //left board
    wheels.Pivot_L(90);
    i_hopper_navigation(wheels, hopper, num_inter, num_turn, 0, 3, 0);
    wheels.Pivot_L(90);
    Serial.println("at the origin"); 
  }
}
  
  
void line_following(Wheels& wheels){
  Serial.println("LINE FOLLOWING...");
  int Rsensor = analogRead(13);
  int Lsensor = analogRead(14); 
  Serial.println("sensor readings: R,L");
  Serial.print("reading values:   ");
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.println(Lsensor);
  Serial.print("reference values: ");
  Serial.print(MAX_WHITE_F_R);
  Serial.print(' ');
  Serial.println(MAX_WHITE_F_L);
  if (Lsensor > (MAX_WHITE_F_L + 100) && Rsensor < (MAX_WHITE_F_R + 100))
  {
      wheels.Turn_L(LINE_FOLLOW_ADJUST,35,90);
      Serial.println("1");
  }
  else if (Rsensor > (MAX_WHITE_F_R + 100) && Lsensor < (MAX_WHITE_F_L + 100))
  {
    wheels.Turn_R(LINE_FOLLOW_ADJUST,35,90);
    Serial.println("2");
  }
    else 
  {
    wheels.Forward(LINE_FOLLOW_SPEED);
    Serial.println("6");
  }
}


void backward_align(Wheels& wheels){
  Serial.println("backward aligning...");
  int PRsensor = analogRead(9);
  int PLsensor = analogRead(8);  
  while ((PRsensor < (MAX_WHITE_B_R+50)) && (PLsensor < (MAX_WHITE_B_L +50)))
  {
    PRsensor = analogRead(9);
    PLsensor = analogRead(8); 
    wheels.Backward_straight(75);
    noInterrupts();
    Serial.print(PRsensor);
    Serial.print(' ');
    Serial.println(PLsensor);
  }
  wheels.Stop();
}

      
void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
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
