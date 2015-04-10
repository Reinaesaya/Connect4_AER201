#include "Diff_steer.h"
#include "Ultrasonic.h"
DiffSteering MyWheel(13,12,11,10,9);
const int MaxR = 650;
const int MaxL = 610;
const int MinF = 950;
const int MinB = 950;
const int MaxPR = 46;
const int MaxPL = 47;
int x_val;
int y_val;
int mode;
byte num_inter = 0;
byte num_turn = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("input x coordinate: ");
  while (Serial.available() > 0)
  {
    x_val = Serial.read() -48;
  }
  Serial.println("input y coordinate: ");
  while (Serial.available() <= 0)
  {
    y_val = Serial.read() - 48;
  }
  y_val = Serial.read() - 48;
  Serial.print("going to: [");
  Serial.print(x_val);
  Serial.print(" , ");
  Serial.print(y_val);
  Serial.println(" ]");
  Serial.println("choose operating mode: ");
  while (Serial.available() <= 0)
  {
    continue;
  }
  mode = Serial.read() -48;
  Serial.println("start...");
}



void loop(){
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A13);
  int Lsensor = analogRead(A14);
  int Bsensor = analogRead(A12);
  int PRsensor = analogRead(A9);
  int PLsensor = analogRead(A8);  
  Serial.println("sensor readings: R,L,F,B,PR,PL");
  Serial.print("reading values:   ");
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.print(Lsensor);
  Serial.print(' ');
  Serial.print(Fsensor);
  Serial.print(' ');
  Serial.print(Bsensor); 
  Serial.print(' ');
  Serial.print(PRsensor);
  Serial.print(' ');
  Serial.println(PLsensor);
  Serial.print("reference values: ");
  Serial.print(MaxR);
  Serial.print(' ');
  Serial.print(MaxL);
  Serial.print(' ');
  Serial.print(MinF);
  Serial.print(' ');
  Serial.print(MinB);
  Serial.print(' ');
  Serial.print(MaxPR);
  Serial.print(' ');
  Serial.println(MaxPL);
  if (Lsensor > (MaxL + 100) && Rsensor < (MaxR + 100))
  {
      MyWheel.Turn_L(100,0,90);
      Serial.println("1");
  }
  else if (Rsensor > (MaxR +100) && Lsensor < (MaxL + 100))
  {
    MyWheel.Turn_R(100,0,90);
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
         num_inter+= 1;
      }
      byte interval = 80;
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(num_inter);
      Serial.print("number of turns made: ");
      Serial.println(num_turn);
      
      //Determine the direction
      byte DIRECT = 1;
     
      
      //start turning...
      while(DIRECT == 1)
      {  //LEFT TURN
        num_inter = 0;
        Fsensor = analogRead(A15);
        Serial.println("3");
        MyWheel.Pivot_L(90);
        Serial.println(Fsensor);
        unsigned long prevMillis = millis();
        unsigned long currMillis = millis();
        while(currMillis - prevMillis < interval)
        {  
          Fsensor = analogRead(A15);
          Serial.println("is waiting 1...");
          MyWheel.Pivot_L(20);
          currMillis = millis();
          Serial.println(currMillis);
        }
        Serial.println(Fsensor);
        int count = 0;
        while(count < 9 && ((Fsensor < MinF - 100)||(Bsensor < MinB - 100)))
        {
          count += 1;
          Fsensor = analogRead(A15);
          Bsensor = analogRead(A12);
          prevMillis = currMillis;
          Serial.println(Fsensor);
          Serial.print("looped");
          Serial.println(count);  
          MyWheel.Pivot_L(25); 
        }
        num_turn += 1;
        MyWheel.Stop();
        break;
      }
      while (DIRECT == 2)
      {  //LEFT RIGHT
          num_inter = 0;
          Fsensor = analogRead(A15);
          Serial.println("4");
          MyWheel.Pivot_R(90);
          Serial.println(Fsensor);
          unsigned long prevMillis = millis();
          unsigned long currMillis = millis();
          while(currMillis - prevMillis < interval)
          {  
              Fsensor = analogRead(A15);
              Serial.println("is waiting 1...");
              MyWheel.Pivot_R(20);
              currMillis = millis();
              Serial.println(currMillis);
          }
          Serial.println(Fsensor);
          int count = 0;
          while( count < 9 && ((Fsensor < MinF - 100)||(Bsensor < MinB -100)))
          {
              count += 1;
              Fsensor = analogRead(A15);
              Bsensor = analogRead(A12);
              prevMillis = currMillis;
              Serial.println(Fsensor);
              Serial.print("looped");
              Serial.print(count);
              MyWheel.Pivot_R(25); 
          }
          num_turn += 1;
          break;
      }
      while (DIRECT == 0)
      {
        Fsensor = analogRead(A15);
        MyWheel.Forward(150);
        Serial.println("5");
        break;
        
      }
    }
  else 
  {
    MyWheel.Forward(150);
    Serial.println("6");
  }
}
void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

byte square(byte num_inter, byte num_turn){
  byte direct = 0;
  if (num_inter % 2 == 0 && num_inter != 0)
  {
    direct = 1;
  } 
  return direct;
}


  
byte hopper(byte num_inter, byte num_turn){
  byte direct = 0;
  if (num_inter == 1 && num_turn == 0  && num_inter != 0)
  {
    direct = 1;
  }
  else if(num_inter = 3 && num_turn == 1  && num_inter != 0)
  {
    direct = 1;
  }
  return direct;
}

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
