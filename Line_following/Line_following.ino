#include "Diff_steer.h"
#include "Ultrasonic.h"
DiffSteering MyWheel(13,9,10,11,12);
long collab_arrayR[5] = {0,0,0,0,0};
long collab_arrayL[5] = {0,0,0,0,0};
long collab_arrayB[5] = {0,0,0,0,0};
long collab_arrayF[5] = {0,0,0,0,0};
long collab_arrayPR[5] = {0,0,0,0,0};
long collab_arrayPL[5] = {0,0,0,0,0};
long range_array[5] = {100,100,100,100,100};
byte num_inter = 0;
byte num_turn = 0;
void setup(){
  Serial.begin(9600);
  while (millis() <2000){
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A14);
  int Lsensor = analogRead(A13);
  int Bsensor = analogRead(A12);
  int PRsensor = analogRead(A9);
  int PLsensor = analogRead(A8);  
  shift_add(collab_arrayR, 5, Rsensor);    //Right has a white range
  shift_add(collab_arrayL, 5, Lsensor);    //left has a white range 
  shift_add(collab_arrayB, 5, Bsensor);    // back has a black range
  shift_add(collab_arrayF, 5, Fsensor);    //front has a black range
  shift_add(collab_arrayPR, 5, PRsensor);   
  shift_add(collab_arrayPL, 5, PLsensor);
  }
  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayR[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayL[i]);
    Serial.print(' ');
    delay(100);
  }
    Serial.println(' ');

  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayF[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayB[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayPR[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 5; i++){
    Serial.print(collab_arrayPL[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
}
void loop(){
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A14);
  int Lsensor = analogRead(A13);
  int Bsensor = analogRead(A12);
  int PRsensor = analogRead(A9);
  int PLsensor = analogRead(A8);  
  int MaxR = maximum(collab_arrayR, 5);
  int MaxL = maximum(collab_arrayL, 5);
  int MinF = minimum(collab_arrayF, 5);
  int MinB = minimum(collab_arrayB, 5);
  int MaxPR = maximum(collab_arrayPR, 5);
  int MaxPL = maximum(collab_arrayPL, 5);
  Serial.println(' ');
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
  Serial.print(PLsensor);
  Serial.println("Compare");
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
      MyWheel.Turn_L(10);
      Serial.println("1");
  }
  else if (Rsensor > (MaxR +100) && Lsensor < (MaxL + 100))
  {
    MyWheel.Turn_R(10);
    Serial.println("2");
  }
  else if (PRsensor > (MaxPR+100) && PLsensor > (MaxPL +100))
    {  // @the intersection
      byte interval = 150;
      num_inter+= 1;
      MyWheel.Stop();
      byte DIRECT = direct_det(num_turn, num_inter);
      while(DIRECT == 1)
      {  //LEFT TURN
        Fsensor = analogRead(A15);
        Serial.println("4");
        MyWheel.Pivot_L(90);
        Serial.println(Fsensor);
        unsigned long prevMillis = millis();
        unsigned long currMillis = millis();
        while(currMillis - prevMillis < interval)
        {  
          Fsensor = analogRead(A15);
          Serial.println("is waiting 1...");
          MyWheel.Stop();
          currMillis = millis();
          Serial.println(currMillis);
        }
        Serial.println(Fsensor);
        while( Fsensor < (MinF - 90))
        {
          Fsensor = analogRead(A15);
          prevMillis = currMillis;
          Serial.println(Fsensor);
          Serial.println("is in the loop");
          MyWheel.Pivot_L(30); 
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
        break;
      }
      while (DIRECT == 2)
      {  //RIGHT TURN
        Serial.println("4");
        MyWheel.Pivot_R(90);
        MyWheel.Stop();
        while( Fsensor < (MinF -90))
        {
          Serial.println("is in the loop");
          MyWheel.Pivot_R(20);
        }
        num_turn += 1;
        break;
      }
      while (DIRECT == 0)
      {
        MyWheel.Forward(200);
        Serial.println("8");
        break;
        
      }
    }
  else 
  {
    MyWheel.Forward(200);
    Serial.println("3");
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

byte direct_det(byte num_turn, byte num_inter){
  byte direct = 0;
  if (num_turn == 0)
  {
    direct = 1;
  }
  return direct;
}
  

