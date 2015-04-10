#include "Diff_steer.h"
#include "Encoder.h"
#include "constants.h"
#include "Ultrasonic.h"


Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
DiffSteering MyWheel(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);


int x_val;
int y_val;
int mode;
byte num_inter = 0;
byte num_turn = 0;
long range_array[5] = {100,100,100,100,100};
int Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor;



void setup(){
  Serial.begin(9600);
Serial.println("input x coordinate: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  x_val = Serial.read() -48;
  Serial.println("input y coordinate: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  y_val = Serial.read() - 48;
  Serial.print("going to: [");
  Serial.print(x_val);
  Serial.print(" , ");
  Serial.print(y_val);
  Serial.println(" ]");
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  Serial.println("start...");
}



void loop(){

    line_following(Lsensor, Rsensor, PLsensor, PRsensor);

}

void doEncoder_L_A() {MyWheel.encoder.update_L_A();}
void doEncoder_L_B() {MyWheel.encoder.update_L_B();}
void doEncoder_R_A() {MyWheel.encoder.update_R_A();}
void doEncoder_R_B() {MyWheel.encoder.update_R_B();}

void line_following(int Lsensor, int Rsensor, int PLsensor, int PRsensor){
  Serial.println("LINE FOLLOWING");
  Rsensor = analogRead(A13);
  Lsensor = analogRead(A14);
  PRsensor = analogRead(A9);
  PLsensor = analogRead(A8);  
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
      MyWheel.Turn_L(80,0,90);
      Serial.println("1");
  }
  else if (Rsensor > (MaxR +100) && Lsensor < (MaxL + 100))
  {
    MyWheel.Turn_R(80,0,90);
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
         int PRsensor_Inter = analogRead(A9);
         int PLsensor_Inter = analogRead(A8); 
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
      else if (PRsensor > (MaxPR+100) && PLsensor > (MaxPL+100))
      {
        num_inter++;
      }
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(num_inter);
      Serial.print("number of turns made: ");
      Serial.println(num_turn);
      
      //Determine the direction
      byte DIRECT = input_coor(num_inter, num_turn);
      /*switch(mode)
      {
        case 1:
          DIRECT = square(num_inter, num_turn);
          break;
        case 2:
          DIRECT = backup(num_inter, num_turn);
          break;
        case 3:
          DIRECT = input_coor(num_inter, num_turn);
          break;
        case 4:
          DIRECT = hopper(num_inter, num_turn);
          break;
      }*/
      //start turning...
      if(DIRECT == 1)
      {  //LEFT TURN
        num_inter = 0;
        Serial.println("3");
        MyWheel.Pivot_L(90);
        num_turn += 1;
        MyWheel.Stop();
      }
      if (DIRECT == 2)
      {  //LEFT RIGHT
          num_inter = 0;
          Serial.println("4");
          MyWheel.Pivot_R(90);
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

void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

//byte direct_det(byte num_inter, byte num_turn, byte num_hopper){
  //byte direct = 0;
  

byte input_coor(byte num_inter, byte num_turn){
  byte direct = 0;
  if (num_inter == y_val && num_turn == 0  && num_inter != 0)
  {
    if (x_val > 0)
    {
      direct = 1;
    }
  }
  else if (num_inter == x_val && num_turn == 1  && num_inter != 0)
  {
    while(1)
    {
      MyWheel.Pivot_L(150);
      while (1){
        MyWheel.Stop();
      }
    }
  }
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
