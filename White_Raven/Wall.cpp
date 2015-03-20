#include "Wall.h"
#include "Arduino.h"
#include "constants.h"


/* BASIC WALL FOLLOWING FUNCTIONS */

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

/* WALL POSITIONING AND NAVIGATION */

void forward_to_dist(long stop_dist, Wheels& wheels, Ultrasonic& front_US, long *front_array, int SPD)
// Move robot forward until front sensor hits certain distance
{
  initialize_ultrasonic(front_US, front_array);
  while (check_array(front_array, stop_dist) != 1)
  {
    long obj_range_front = front_US.Ranging(CM);
    shift_add(front_array, obj_range_front);
    wheels.Forward(SPD);
  }
  wheels.Stop();
}

void forward_to_dist_wall(long stop_dist, long wall_dist, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& side_US,
                          long *front_array, long *side_array, int SPD)
// Wall follow until front sensor hits certain distance
{
  initialize_ultrasonic(front_US, front_array);
  initialize_ultrasonic(side_US, side_array);
  long loops = 0;
  while (check_array(front_array, stop_dist) != 1)
  {
    long obj_range_front = front_US.Ranging(CM);
    long obj_range_side = side_US.Ranging(CM);
    shift_add(front_array, obj_range_front);
    shift_add(side_array, obj_range_side);
    if (loops % 2 == 0) wall_straight_adjust(side_array, SPD, wheels);
    else wall_dist_adjust(side_array, wall_dist, SPD, wheels);
    loops++;
  }
  wheels.Stop();
}

float find_wall_and_position(int side, float angle, Wheels& wheels, Ultrasonic& front_US, long *front_array)
// Finds the wall, navigates to it, and positions robot along the wall ready to move towards gameboard
// Returns angle direction which the robot is pointing
{
  if (side == LEFT_BOARD)
  {
    if (angle <= 180) { wheels.Pivot_R(angle); }
    else if (angle > 180) { wheels.Pivot_L(360-angle); }
    forward_to_dist(SIDE_WALL_FOLLOW_DIST - TURN_DIFFERENCE, wheels, front_US, front_array, WALL_FIND_SPEED);
    wheels.Pivot_R(90);
  }
  else if (side == RIGHT_BOARD)
  {
    if (angle <= 180) { wheels.Pivot_L(180-angle); }
    else if (angle > 180) { wheels.Pivot_L(angle-180); }
    forward_to_dist(SIDE_WALL_FOLLOW_DIST - TURN_DIFFERENCE, wheels, front_US, front_array, WALL_FIND_SPEED);
    wheels.Pivot_L(90);
  }
  return (float)180;
}

float get_to_gameboard(int side, float angle, int game_column, Wheels& wheels,
                       Ultrasonic& front_US, Ultrasonic& left_US, Ultrasonic& right_US,
                       long *front_array, long *left_array, long *right_array)
{
  float current_angle = find_wall_and_position(side, angle, wheels, front_US, front_array);
  if (side == LEFT_BOARD)
  {
    forward_to_dist_wall(GAMEBOARD_WALL_FOLLOW_DIST - TURN_DIFFERENCE, SIDE_WALL_FOLLOW_DIST, wheels,
                         front_US, left_US, front_array, left_array, WALL_FOLLOW_SPEED);
    wheels.Pivot_R(90);
    long stop_dist = (long)((-1)*(((unsigned int)game_column - 4) * GAMEBOARD_COL_SPACING) + GAMEBOARD_CENTER_FROM_WALL);
    Serial.print("Stop Distance: ");
    Serial.println(stop_dist);
    forward_to_dist_wall(stop_dist - PIVOT_TO_FRONT_US, GAMEBOARD_WALL_FOLLOW_DIST, wheels,
                         front_US, left_US, front_array, left_array, WALL_FOLLOW_SPEED);
    wheels.Pivot_L(90);
  }
  else if (side == RIGHT_BOARD)
  {
    forward_to_dist_wall(GAMEBOARD_WALL_FOLLOW_DIST - TURN_DIFFERENCE, SIDE_WALL_FOLLOW_DIST, wheels,
                         front_US, right_US, front_array, right_array, WALL_FOLLOW_SPEED);
    wheels.Pivot_L(90);
    long stop_dist = (long)((((unsigned int)game_column - 4) * GAMEBOARD_COL_SPACING) + GAMEBOARD_CENTER_FROM_WALL);
    Serial.print("Stop Distance: ");
    Serial.println(stop_dist);
    forward_to_dist_wall(stop_dist - PIVOT_TO_FRONT_US, GAMEBOARD_WALL_FOLLOW_DIST, wheels,
                         front_US, right_US, front_array, right_array, WALL_FOLLOW_SPEED);
    wheels.Pivot_R(90);
  }
  return (float)180;
}
