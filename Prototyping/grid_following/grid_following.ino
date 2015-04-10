
#include "Diff_steer.h"
#include <LiquidCrystal.h>
#include "Ultrasonic.h"

LiquidCrystal lcd(14,15,16,17,18,19);
DiffSteering MyWheel(13,12,11,10,9);
Ultrasonic ultrasonic(23,22);

byte num_inter = 0;
byte num_turn = 0;

// Collaboration 
long collab_arrayR[10] = {0,0,0,0,0,0,0,0,0,0};
long collab_arrayL[10] = {0,0,0,0,0,0,0,0,0,0};
long collab_arrayB[10] = {0,0,0,0,0,0,0,0,0,0};
long collab_arrayF[10] = {0,0,0,0,0,0,0,0,0,0};
long range_array[5] = {100,100,100,100,100};

void setup(){
  lcd.begin(16,2);
  Serial.begin(9600);
  while (millis() <5000){
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A13);
  int Lsensor = analogRead(A14);
  int Bsensor = analogRead(A12);
  shift_add(collab_arrayR, 10, Rsensor);    //Right has a white range
  shift_add(collab_arrayL, 10, Lsensor);    //left has a white range 
  shift_add(collab_arrayB, 10, Bsensor);    // back has a black range
  shift_add(collab_arrayF, 10, Fsensor);    //front has a black range
  }
  for (int i = 0; i < 10; i++){
    Serial.print(collab_arrayR[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 10; i++){
    Serial.print(collab_arrayL[i]);
    Serial.print(' ');
    delay(100);
  }
    Serial.println(' ');

  for (int i = 0; i < 10; i++){
    Serial.print(collab_arrayF[i]);
    Serial.print(' ');
    delay(100);
  }
  Serial.println(' ');
  for (int i = 0; i < 10; i++){
    Serial.print(collab_arrayB[i]);
    Serial.print(' ');
    delay(100);
  }

}

void loop(){
  int Fsensor = analogRead(A15);
  int Rsensor = analogRead(A13);
  int Lsensor = analogRead(A14);
  int Bsensor = analogRead(A12);
  lcd.print("sensor reading: ");
  lcd.setCursor(0,1);
  Serial.print(Rsensor);
  Serial.print(' ');
  Serial.print(Lsensor);
  Serial.print(' ');
  Serial.print(Fsensor);
  Serial.print(' ');
  Serial.println(Bsensor); 
  int MaxR = maximum(collab_arrayR, 10);
  int MinR = minimum(collab_arrayR, 10);
  int MaxL = maximum(collab_arrayL, 10);
  int MinL = minimum(collab_arrayL, 10);
  int MaxF = maximum(collab_arrayF, 10);
  int MinF = minimum(collab_arrayF, 10);
  int MaxB = maximum(collab_arrayB, 10);
  int MinB = minimum(collab_arrayB, 10);

  int Fval = check_array(Fsensor, 10, MaxF, MinF);
  int Bval = check_array(Bsensor, 10, MaxB, MinB);
  int Rval = check_array(Rsensor, 10, MaxR, MinR);
  int Lval = check_array(Lsensor, 10, MaxL, MinL);
  Serial.print(Rval);
  Serial.print(' ');
  Serial.print(Lval);
  Serial.print(' ');
  Serial.print(Fval);
  Serial.print(' ');
  Serial.println(Bval);
  //Using Ultrasonic sensor to avoid obstcles
  long obj_range = ultrasonic.Ranging(CM);
  
  shift_add(range_array, 5, obj_range);
  
  if (check_array_ultra(range_array, 5, 40) == 1) {
    MyWheel.Pivot_L(90);
  }
  
  delay(10);
  
  //Grid Following using IR sensors 
  if (Fval == 0)
  { // If front sensor is not in the range, it detects white
    if(Lval == 0 && Rval == 1)
    { // If left detects black and right detects white
      // Car front shifted right, this turn left
      MyWheel.Pivot_L(90);
      Serial.println("1");
    }
    else if(Rval == 0 && Lval == 1)
    {
      MyWheel.Pivot_R(90);
      Serial.println("2");
    }
    else if (Lval == 1 && Rval == 1)
    {        //pause for now...
      MyWheel.Forward(90);
      Serial.println("3");
    }
    else
    {
      MyWheel.Forward(200);
      Serial.println("4");
    }
    delay(500);
  }
  
  else if(Fval == 1)
  {  //front sensor detects black
    if (Rval == 0 && Lval == 0 && Bval == 1){
      MyWheel.Forward(200);}
    {  // @the intersection
      num_inter+= 1;
       //DETERMIN_DIREC(XXX);
      //Serial.println("5");
      byte DIRECT = 1;
      if(DIRECT == 1)
      {
        while (Bval == 0 && Fval == 0)
        {
          MyWheel.Pivot_R(90);
        }
        Serial.println("6");
        num_turn += 1;
      }
      else if (DIRECT == 2)
      {
        while (Bval == 0 && Fval == 0)
        {
          MyWheel.Pivot_L(90);
        }
        Serial.println("7");
        num_turn += 1;
      }
      else if (DIRECT == 0)
      {
        MyWheel.Forward(90);
        Serial.println("8");
      }*/
    }
    if (Rval == 1 && Lval == 1 && Bval == 1)
    {    //line following...
      MyWheel.Forward(90);
      Serial.println("9");
    }
  }
  

void shift_add(long* arr, int length, long b){
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

int minimum(long* arr, int length){
  int minval = arr[0];
  for (int i = 0; i != length; i++){
    if (arr[i] < minval){
      minval = arr[i];}
  }
 return minval;
}

int maximum(long* arr, int length){
  int maxval = arr[0];
  for (int i = 0; i != length; i++){
    if (arr[i] > maxval){
      maxval = arr[i];}
  }
 return maxval;
}        

int check_array(int sensor_val, int length, int Max, int Min) {
  
  int ret = 1;
  if (sensor_val > (Max +50) || sensor_val < (Min-50)) 
  {
    ret = 0;
   }
  return ret;
}

int check_array_ultra(long* arr, int length, long thresh) {
  
  int ret = 1;
  
  for (int i = 0; i != length; i++) {
    if (arr[i] >= thresh) {
      ret = 0;
    }
  }
  return ret;
}
/*byte det_direc(byte num_turn, byte num_inter){
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

