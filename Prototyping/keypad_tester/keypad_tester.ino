#include <Keypad.h>
#define ROW_PIN_D 34
#define ROW_PIN_C 35
#define ROW_PIN_B 36
#define ROW_PIN_A 37
#define COL_PIN_D 38
#define COL_PIN_C 39
#define COL_PIN_B 40
#define COL_PIN_A 41
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

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
