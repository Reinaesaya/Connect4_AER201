#include "Diff_steer.h"
#include "Ultrasonic.h"
DiffSteering MyWheel(13,12,11,10,9);
const int MaxR = 650;
const int MaxL = 610;
const int MinF = 950;
const int MinB = 950;
const int MaxPR = 46;
const int MaxPL = 47;
byte num_inter = 0;
byte num_turn = 0;
void setup(){
  Serial.begin(9600);
  delay (1000);
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
  //MyWheel.Pivot_L(200);
  if (Lsensor > (MaxL + 100) && Rsensor < (MaxR + 100))
  {
      MyWheel.Turn_L(10);
      Serial.println("1");
  }
  else if (Rsensor > (MaxR +100) && Lsensor < (MaxL + 100))
  {
    MyWheel.Turn_R(10);
    Serial.println("2");
  }
  else if (PRsensor > (MaxPR+100) || PLsensor > (MaxPL +100) )
    {  // @the intersection
      byte interval = 80;
      num_inter+= 1;
      MyWheel.Stop();
      Serial.print("number of inters encounterted: ");
      Serial.println(num_inter);
      Serial.print("number of turns made: ");
      Serial.println(num_turn);
      byte DIRECT = square(num_inter, num_turn);
      //byte DIRECT = 0;
      while(DIRECT == 1)
      {  //LEFT TURN
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
        while(count < 3)
        {
          count += 1;
          Fsensor = analogRead(A15);
          Bsensor = analogRead(A12);
          prevMillis = currMillis;
          Serial.println(Fsensor);
          Serial.print("looped");
          Serial.println(count);  
          MyWheel.Pivot_L(45); 
          /*while(currMillis - prevMillis < interval)
            {  
              Fsensor = analogRead(A15);
              Serial.println("is waiting 2...");
              Serial.println(Fsensor);
              MyWheel.Stop();
              currMillis = millis();
            }    */
        }
        num_turn += 1;
        MyWheel.Stop();
        delay(3000);
        break;
      }
      while (DIRECT == 2)
      {  //LEFT RIGHT
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
          while( Fsensor < (MinF - 100))
          {
              Fsensor = analogRead(A15);
              prevMillis = currMillis;
              Serial.println(Fsensor);
              Serial.println("is in the loop");
              MyWheel.Pivot_R(30); 
          }
          num_turn += 1;
          break;
      }
      while (DIRECT == 0)
      {
        Fsensor = analogRead(A15);
        MyWheel.Forward(255);
        Serial.println("5");
        break;
        
      }
    }
  else 
  {
    MyWheel.Forward(255);
    Serial.println("6");
  }
}
int maximum(long* arr, int length){
  int maxval = arr[0];
  for (int i = 0; i != length; i++){
    if (arr[i] > maxval){
      maxval = arr[i];}
  }
 return maxval;
}  
void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}
int minimum(long* arr, int length){
  int minval = arr[0];
  for (int i = 0; i != length; i++){
    if (arr[i] < minval){
      minval = arr[i];}
  }
 return minval;
}

byte square(byte num_inter, byte num_turn){
  byte direct = 0;
  if (num_inter % 3 == 0)
  {
    direct = 1;
  } 
  return direct;
}
  
/*byte gameboard(byte num_inter, byte num_turn){
  byte direct = 0;*/
  

