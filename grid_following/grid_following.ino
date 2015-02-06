const byte RsensorPin = 0;
const byte LsensorPin = 1;
const byte BsensorPin = 2;
const byte FsensorPin = 3;
const byte MaxWhite = 55; 
#include "Diff_steer.h"
DiffSteering MyWheel(13,9,10,5,6);
void setup(){
  pinMode(Rsensor, INPUT);
  pinMode(Lsensor, INPUT);
  pinMode(Msensor, INPUT);
  pinMode(Fsensor, INPUT));  
  Serial.begin(9600);
}

void loop(){
  int Fsensor = analogRead(FsensorPin);
  int Rsensor = analogRead(RsensorPin);
  int Lsensor = analogRead(LsensorPin);
  int Bsensor = analogRead(BsensorPin);
  byte num_inter = 0;
  byte num_turn = 0;
  if (Fsensor < MaxWhite){  //front sensor detects white
    if (Bsensor > MaxWhite){  //back sensor detects black
      if(Lsensor > MaxWhite && Rsensor < MaxWhite){
         MyWheel.Pivot_L(90);}
       else if(Rsensor > MaxWhite && Lsensor < MaxWhite){
         MyWheel.Pivot_R(90);}
       else if (Rsensor < MaxWhite && Lsensor<MaxWhitt){  //pause for now...
         MyWheel.Forward(0);}
       else{
         MyWheel.Forward(200);}
    }
    else{  //back sensor detects white 
      MyWheel.Forward(0);  //pause for now...}
  }
  else if(Fsensor > MaxWhite){  //front sensor detects black
     if (Rsensor > MaxWhite && Lsensor > MaxWhite && Bsensor > MaxWhite){  // @the intersection
       num_inter+= 1;
       MyWheel.Forward(0);
       DETERMIN_DIREC(XXX);
       if(DIRECT == 0){
         MyWheel.Pivot_R(90);
         num_turn += 1;}
       else if (DIRECT == 1){
         MyWheel.Pivot_L(90);
         num_turn += 1;}
       else{
         MyWheel.Forward(100);}
       }
     else{
       MyWheel.Forward(0);}
  }     
       
       
        
/*int det_direc(byte num_turn, byte num_inter){
  byte direct;
       if(num_inter == 2 && num_turn == 0){
         num_inter = 0;
         direc = 0;}
       else if(num_inter == 2 && num_turn     */     
