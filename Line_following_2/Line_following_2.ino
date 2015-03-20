#include "Diff_steer.h"
#include "Encoder.h"
#include "constants.h"
#include "Ultrasonic.h"
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {ROW_PIN_D, ROW_PIN_C, ROW_PIN_B, ROW_PIN_A}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {COL_PIN_D, COL_PIN_C, COL_PIN_B, COL_PIN_A}; //connect to the column pinouts of the keypad
int Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor;
long range_array[5] = {100,100,100,100,100};
byte L_X_val, L_Y_val, num_ball_L, R_X_val, R_Y_val,num_ball_R;
byte CL_X_val, CL_Y_val, num_ball_CL, CR_X_val, CR_Y_val, num_ball_CR;
byte num_inter = 0;
byte num_turn = 0;

Encoder encoder(L_ENC_PINA,L_ENC_PINB,R_ENC_PINA,R_ENC_PINB);
DiffSteering MyWheel(WHEEL_ENABLE,LEFT_WHEEL_F,LEFT_WHEEL_B,RIGHT_WHEEL_F,RIGHT_WHEEL_B,encoder);
Ultrasonic ultrasonic(23,22);
Keypad MyKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  attachInterrupt(L_ENC_PINA_INT, doEncoder_L_A, CHANGE);
  attachInterrupt(L_ENC_PINB_INT, doEncoder_L_B, CHANGE);
  attachInterrupt(R_ENC_PINA_INT, doEncoder_R_A, CHANGE);
  attachInterrupt(R_ENC_PINB_INT, doEncoder_R_B, CHANGE);
  input_coord(&L_X_val,&L_Y_val,&num_ball_L);
  input_coord(&R_X_val,&R_Y_val,&num_ball_R);
  input_coord(&CL_X_val,&CL_Y_val,&num_ball_CL);
  input_coord(&CR_X_val,&CR_Y_val,&num_ball_CR);  
  Serial.println("start...");
}


void loop(){
  long obj_range = ultrasonic.Ranging(CM);
  shift_add(range_array, 5, obj_range);
  while (us_check(range_array, 5, 35) == 0 ){
    line_following(Fsensor, Bsensor, Lsensor, Rsensor, PLsensor, PRsensor);
  }
  MyWheel.Pivot_R(90);
}

void doEncoder_L_A() {MyWheel.encoder.update_L_A();}
void doEncoder_L_B() {MyWheel.encoder.update_L_B();}
void doEncoder_R_A() {MyWheel.encoder.update_R_A();}
void doEncoder_R_B() {MyWheel.encoder.update_R_B();}

void line_following(int Fsensor, int Bsensor, int Lsensor, int Rsensor, int PLsensor, int PRsensor){
  Serial.println(MyWheel.encoder.getPosLeft());
  Serial.println(MyWheel.encoder.getPosRight());
  Fsensor = analogRead(A15);
  Rsensor = analogRead(A13);
  Lsensor = analogRead(A14);
  Bsensor = analogRead(A12);
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
      byte DIRECT = 0;
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
  
void input_coord(byte x_coor, byte y_coor,byte num_ball){
  
  Serial.println("input x coordinate: ");
  x_coor = MyKeypad.waitForKey();
  Serial.println("input y coordinate: ");
  y_coor = MyKeypad.waitForKey();
  Serial.println("number of balls in there: ");
  num_ball = MyKeypad.waitForKey();
  Serial.print("going to: [");
  Serial.print(x_coor);
  Serial.print(" , ");
  Serial.print(y_coor);
  Serial.println(" ]");
  Serial.print("number of balls is :");
  Serial.println(num_ball);
}


void shift_add(long* arr, int length, long b){
  for (int i = 0; i != length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

//byte direct_det(byte num_inter, byte num_turn, byte num_hopper){
  //byte direct = 0;
  

byte hopper(byte num_inter, byte num_turn, byte x_val, byte y_val){
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
      MyWheel.Pivot_L(90);
      while (1){
        MyWheel.Stop();
      }
    }
  }
  return direct;
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
