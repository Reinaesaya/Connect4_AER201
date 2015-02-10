/*const int RsensorPin = A15;
const int LsensorPin = A13;
const int BsensorPin = A14;
const int FsensorPin = A12;*/
const int MaxWhite = 990; 

#include "Diff_steer.h"

DiffSteering MyWheel(13,12,11,10,9);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  byte num_inter = 0;
  byte num_turn = 0;
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A13);
  int Lsensor = analogRead(A14);
  int Bsensor = analogRead(A12);
  Serial.print(Fsensor);
  Serial.print(' ');
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.print(Lsensor);
  Serial.print(' ');
  Serial.println(Bsensor); 
  // Collaboration 
  long collab_arrayR[10] = {0,0,0,0,0,0,0,0,0,0};
  long collab_arrayL[10] = {0,0,0,0,0,0,0,0,0,0};
  long collab_arrayB[10] = {0,0,0,0,0,0,0,0,0,0};
  long collab_arrayF[10] = {0,0,0,0,0,0,0,0,0,0};
  int MaxWhiteR = 0;
  int MaxWhiteL = 0;
  int MinBlackB = 0;
  int MinBlackF = 0;
        
//  while (millis() <2000)
//  {
//    shift_add(collab_arrayR, 10, Rsensor);
//    shift_add(collab_arrayL, 10, Lsensor);
//    shift_add(collab_arrayB, 10, Bsensor);
//    shift_add(collab_arrayF, 10, Fsensor);
//    MaxWhiteR = average(collab_arrayR, 10);
//    MaxWhiteL = average(collab_arrayL, 10);
//    MinBlackF = average(collab_arrayF, 10);
//    MinBlackB = average(collab_arrayB, 10);
//  }
    
  if (Fsensor < MaxWhite)
  { // If front sensor detects white
    //if (Bsensor > MaxWhite){      //back sensor detects black
    if(Lsensor > MaxWhite && Rsensor < MaxWhite)
    { // If left detects black and right detects white
      // Car front shifted right, this turn left
      MyWheel.Pivot_L(90);
      Serial.println("1");
    }
    else if(Rsensor > MaxWhite && Lsensor < MaxWhite)
    {
      MyWheel.Pivot_R(90);
      Serial.println("2");
    }
    else if (Rsensor < MaxWhite && Lsensor<MaxWhite)
    {        //pause for now...
      MyWheel.Forward(90);
      Serial.println("3");
    }
    else
    {
      MyWheel.Stop();
      Serial.println("4");
    }
    delay(500);
  }
    //}
    //else{  //back sensor detects white 
     // MyWheel.Forward(0);}  //pause for now...
  else if(Fsensor > MaxWhite)
  {  //front sensor detects black
    if (Rsensor > MaxWhite && Lsensor > MaxWhite && Bsensor > MaxWhite)
    {  // @the intersection
      num_inter+= 1;
       //DETERMIN_DIREC(XXX);
      Serial.println("5");
      byte DIRECT = 0;
      if(DIRECT == 1)
      {
        MyWheel.Pivot_R(90);
        Serial.println("6");
        num_turn += 1;
        delay(900);
      }
      else if (DIRECT == 2)
      {
        MyWheel.Pivot_L(90);
        Serial.println("7");
        num_turn += 1;
        delay(900);
      }
      else if (DIRECT == 0)
      {
        MyWheel.Forward(90);
        Serial.println("8");
        delay(1000);
      }
    }
    else if (Rsensor < MaxWhite && Lsensor < MaxWhite && Bsensor > MaxWhite)
    {    //line following...
      MyWheel.Forward(90);
      Serial.println("9");
    }
  }
}
  

void shift_add(long* arr, int length, long b) {
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

int average(long* arr, int length){
  int sum = 0;
  int average = 0;
  for (int i = 0; i != length; i++){
    sum += arr[i];}
  average = sum/length;
  return average;
}
       
        
/*int det_direc(byte num_turn, byte num_inter){
  byte direct;
       if(num_inter == 2 && num_turn == 0){
         num_inter = 0;
         direct = 1;}
       else if(num_inter == 2 && num_turn == 1){
         num_inter = 0;
         direct = 1;}
       else if(num_inter == 1 && num_turn == 2){
         num_inter = 0;
         direct = 2;}
       return direct;
}*/

