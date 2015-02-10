/*const int RsensorPin = A15;
const int LsensorPin = A13;
const int BsensorPin = A14;
const int FsensorPin = A12;*/
const int MaxWhite = 990; 

#include "Diff_steer.h"

DiffSteering MyWheel(13,12,11,10,9);

void setup(){
  pinMode(Rsensor, INPUT);
  pinMode(Lsensor, INPUT);
  pinMode(Bsensor, INPUT);
  pinMode(Fsensor, INPUT); 
  Serial.begin(9600);
}

void loop(){
  byte num_inter = 0;
  byte num_turn = 0;
  int Fsensor = analogRead(A12);
  int Rsensor = analogRead(A15);
  int Lsensor = analogRead(A13);
  int Bsensor = analogRead(A14);
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.print(Lsensor);
  Serial.print(' ');
  Serial.print(Bsensor);
  Serial.print(' ');
  Serial.println(Fsensor); 
  // Collaboration 
  long collab_arrayR[10] = {0};
  long collab_arrayL[10] = {0};
  long collab_arrayB[10] = {0};
  long collab_arrayF[10] = {0};
  int MaxWhiteR = 0;
  int MaxWhiteL = 0;
  int MinBlackB = 0;
  int MinBlackF = 0;
        
  while (millis() <2000){
    shift_add(collab_arrayR, 10, Rsensor);
    shift_add(collab_arrayL, 10, Lsensor);
    shift_add(collab_arrayB, 10, Bsensor);
    shift_add(collab_arrayF, 10, Fsensor);
    MaxWhiteR = average(collab_arrayR, 10);
    MaxWhiteL = average(collab_arrayL, 10);
    MinBlackF = average(collab_arrayF, 10);
    MinBlackB = average(collab_arrayB, 10);
    
  if (Fsensor < MaxWhite){        //front sensor detects white
    //if (Bsensor > MaxWhite){      //back sensor detects black
      if(Lsensor > MaxWhite && Rsensor < MaxWhite){
         MyWheel.Pivot_L(90);}
       else if(Rsensor > MaxWhite && Lsensor < MaxWhite){
         MyWheel.Pivot_R(90);}
       else if (Rsensor < MaxWhite && Lsensor<MaxWhite){        //pause for now...
         MyWheel.Forward(25);
         Serial.println("third");}
       else{
         MyWheel.Forward(0);
         Serial.println("fourth");}
       delay(500);
    //}
    //else{  //back sensor detects white 
     // MyWheel.Forward(0);}  //pause for now...
  }
  else if(Fsensor > MaxWhite){  //front sensor detects black
     if (Rsensor > MaxWhite && Lsensor > MaxWhite && Bsensor > MaxWhite){  // @the intersection
       num_inter+= 1;
       MyWheel.Forward(0);
       //DETERMIN_DIREC(XXX);
       byte DIRECT = 0;
       if(DIRECT == 1){
         MyWheel.Pivot_R(90);
         num_turn += 1;
         delay(900);}
       else if (DIRECT == 2){
         MyWheel.Pivot_L(90);
         num_turn += 1;
         delay(900);}
       else if (DIRECT == 0){
         MyWheel.Forward(25);}
       }
     else if (Rsensor < MaxWhite && Lsensor < MaxWhite && Bsensor > MaxWhite){    //line following...
       MyWheel.Forward(25);}
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

