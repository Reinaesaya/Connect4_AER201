#include "Wall.h"
#include "Arduino.h"
#include "constants.h"

void initialize_ultrasonic(Ultrasonic& US, long *array)
// Initialize ultrasonic sensor at current position (fills range array)
{
  for (int i=0; i<=(RANGE_ARRAY_LEN*2); ++i) {
    long r_US = US.Ranging(CM);
    shift_add(array, r_US);
    delay(10);
  }
}

void wall_straight_adjust(long *arr, int SPD, Wheels& wheels)
/* Performs one iteration of adjustment for wall following.
   Follows the wall at any arbitrary distance but maintains a straight line.
*/
{
  int adjust = 0;
  int mid = ((RANGE_ARRAY_LEN+1)/2);
  for (int i=mid; i<RANGE_ARRAY_LEN; ++i)
  {
    for (int j=0; j<mid; ++j)
    {
      if (arr[i] < arr[j]) --adjust;
      else if (arr[i] > arr[j]) ++adjust;
      else continue;
    }
  }
  Serial.print("Straight Adjust Value: ");
  Serial.println(adjust);
  int thresh = (RANGE_ARRAY_LEN/2)*(RANGE_ARRAY_LEN/2) - (RANGE_ARRAY_LEN/2 + 1);
  if (adjust < (thresh*(-1)))    wheels.Turn_R(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
  else if (adjust > thresh)      wheels.Turn_L(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
  else {                         wheels.Forward(SPD);
                                 delay(10); }
}

void wall_dist_adjust(long *arr, long targ, int SPD, Wheels& wheels)
/* Performs one iteration of adjustment for wall following.
   Follows the wall at a specific distance.
*/
{
  int adjust = 0;
  for (int i=0; i<RANGE_ARRAY_LEN; ++i)
  {
    if (arr[i] < targ) --adjust;
    else if (arr[i] > targ) ++adjust;
    else continue;
  }
  Serial.print("Distance Adjust Value: ");
  Serial.println(adjust);
  int thresh = RANGE_ARRAY_LEN - 2;
  if (adjust < (thresh*(-1)))    wheels.Turn_R(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
  else if (adjust > thresh)      wheels.Turn_L(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
  else {                         wheels.Forward(SPD);
                                 delay(10); }
}

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
    Serial.print(arr[i]);
    Serial.print(" ");
    if (arr[i] >= thresh) {
      ret = 0;
    }
  }
  Serial.print("\n");
  return ret;
}
