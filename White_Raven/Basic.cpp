#include "Basic.h"
#include "Arduino.h"
#include "constants.h"

void shift_add(long* arr, long b)
// Append element to end of array and remove first one
{
  for (int i=0; i<RANGE_ARRAY_LEN; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[RANGE_ARRAY_LEN-1] = b;
}

int check_array(long* arr, long thresh)
// Return 1 if every element in array <= threshold value
// Also prints every element in the array
{
  int ret = 1;
  for (int i=0; i!=RANGE_ARRAY_LEN; i++) {
    //Serial.print(arr[i]);
    //Serial.print(" ");
    if (arr[i] >= thresh) {
      ret = 0;
    }
  }
  //Serial.print("\n");
  return ret;
}

int get_input_number()
{
  while (Serial.available() <= 0) continue;
  return ((int)Serial.read() - 48);
}

void my_delay(unsigned long milli)
{
  interrupts();
  unsigned long start_time = millis();
  unsigned long end_time = millis();
  while ((end_time - start_time) < milli) 
  {
    end_time = millis();
  }
  noInterrupts();
}
