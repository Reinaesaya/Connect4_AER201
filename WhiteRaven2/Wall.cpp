#include "Wall.h"
#include "Arduino.h"
#include "constants.h"
#include "Basic.h"


/* BASIC WALL FOLLOWING FUNCTIONS */

void wall_straight_adjust(int side, Ultrasonic& side_US, int SPD, Wheels& wheels)
/* Performs one iteration of adjustment for wall following.
   Follows the wall at any arbitrary distance but maintains a straight line.
*/
{
  long *arr = side_US.array;
  int adjust = 0;
  int mid = ((ARRAY_LEN+1)/2);
  for (int i=mid; i<ARRAY_LEN; ++i)
  {
    for (int j=0; j<mid; ++j)
    {
      if (arr[i] < arr[j]) --adjust;
      else if (arr[i] > arr[j]) ++adjust;
      else continue;
    }
  }
  int thresh = (ARRAY_LEN/2)*(ARRAY_LEN/2) - (ARRAY_LEN/2 + 1);
  
  if (side == LEFT_BOARD)
  {
    if (adjust < (thresh*(-1)))    wheels.Turn_R(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
    else if (adjust > thresh)      wheels.Turn_L(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
    else                           wheels.Forward(SPD);
  }
  else if (side == RIGHT_BOARD)
  {
    if (adjust < (thresh*(-1)))    wheels.Turn_L(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
    else if (adjust > thresh)      wheels.Turn_R(ADJUSTMENT_MILLIS,STRAIGHT_ADJ_INNER,STRAIGHT_ADJ_OUTER);
    else                           wheels.Forward(SPD);
  }
}

void wall_dist_adjust(int side, Ultrasonic& side_US, long targ, int SPD, Wheels& wheels)
/* Performs one iteration of adjustment for wall following.
   Follows the wall at a specific distance.
*/
{
  long *arr = side_US.array;
  int adjust = 0;
  for (int i=0; i<ARRAY_LEN; ++i)
  {
    if (arr[i] < targ) --adjust;
    else if (arr[i] > targ) ++adjust;
    else continue;
  }
  int thresh = ARRAY_LEN - 2;
  if (side == LEFT_BOARD)
  {
    if (adjust < (thresh*(-1)))    wheels.Turn_R(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
    else if (adjust > thresh)      wheels.Turn_L(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
    else                           wheels.Forward(SPD);
  }
  else if (side == RIGHT_BOARD)
  {
    if (adjust < (thresh*(-1)))    wheels.Turn_L(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
    else if (adjust > thresh)      wheels.Turn_R(ADJUSTMENT_MILLIS,DIST_ADJ_INNER,DIST_ADJ_OUTER);
    else                           wheels.Forward(SPD);
  }
}

/* WALL POSITIONING AND NAVIGATION */

void forward_to_dist(long stop_dist, Wheels& wheels, Ultrasonic& front_US, int SPD)
// Move robot forward until front sensor hits certain distance
{
  front_US.initialize_array();
  while (check_array(front_US.array, stop_dist) != 1)
  {
    front_US.update_array();
    wheels.Forward(SPD);
  }
  wheels.Stop();
  my_delay(1000);
}

void forward_to_dist_wall(int side, long stop_dist, long wall_dist, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& side_US, int SPD)
// Wall follow until front sensor hits certain distance
{
  front_US.initialize_array();
  side_US.initialize_array();
  long loops = 0;
  while (check_array(front_US.array, stop_dist) != 1)
  {
    front_US.update_array();
    side_US.update_array();
    Serial.println("Side");
    check_array(side_US.array, wall_dist);
    if (loops % 2 == 0) wall_straight_adjust(side, side_US, SPD, wheels);
    else wall_dist_adjust(side, side_US, wall_dist, SPD, wheels);
    loops++;
  }
  wheels.Stop();
}

void find_wall_and_position(int side, float& angle, Wheels& wheels, Ultrasonic& front_US)
// Finds the wall, navigates to it, and positions robot along the wall ready to move towards gameboard
// Changes angle to that its facing (towards gameboard)
{
  if (side == LEFT_BOARD)
  {
    if (angle <= 180) { wheels.Pivot_R(angle); }
    else if (angle > 180) { wheels.Pivot_L(360-angle); }
    forward_to_dist(SIDE_WALL_FOLLOW_DIST - TURN_DIFFERENCE, wheels, front_US, WALL_FIND_SPEED);
    wheels.Pivot_R(90);
  }
  else if (side == RIGHT_BOARD)
  {
    if (angle <= 180) { wheels.Pivot_L(180-angle); }
    else if (angle > 180) { wheels.Pivot_L(angle-180); }
    forward_to_dist(SIDE_WALL_FOLLOW_DIST - TURN_DIFFERENCE, wheels, front_US, WALL_FIND_SPEED);
    wheels.Pivot_L(90);
  }
  angle = 180;
}

void get_to_gameboard(int side, float& angle, int game_column, Wheels& wheels, Ultrasonic& front_US, Ultrasonic& left_US, Ultrasonic& right_US)
// Navigate to gameboard and positions to selected column position
{
  find_wall_and_position(side, angle, wheels, front_US);
  if (side == LEFT_BOARD)
  {
    forward_to_dist_wall(side, GAMEBOARD_WALL_FOLLOW_DIST - TURN_DIFFERENCE, SIDE_WALL_FOLLOW_DIST, wheels,
                         front_US, left_US, WALL_FOLLOW_SPEED);
    wheels.Pivot_R(90);
    long stop_dist = (long)((-1)*(((unsigned int)game_column - 4) * GAMEBOARD_COL_SPACING) + GAMEBOARD_CENTER_FROM_WALL);
    forward_to_dist_wall(side, stop_dist - PIVOT_TO_FRONT_US, GAMEBOARD_WALL_FOLLOW_DIST, wheels,
                         front_US, left_US, WALL_FOLLOW_SPEED);
    wheels.Pivot_L(90);
  }
  else if (side == RIGHT_BOARD)
  {
    forward_to_dist_wall(side, GAMEBOARD_WALL_FOLLOW_DIST - TURN_DIFFERENCE, SIDE_WALL_FOLLOW_DIST, wheels,
                         front_US, right_US, WALL_FOLLOW_SPEED);
    wheels.Pivot_L(90);
    long stop_dist = (long)((((unsigned int)game_column - 4) * GAMEBOARD_COL_SPACING) + GAMEBOARD_CENTER_FROM_WALL);
    forward_to_dist_wall(side, stop_dist - PIVOT_TO_FRONT_US, GAMEBOARD_WALL_FOLLOW_DIST, wheels,
                         front_US, right_US, WALL_FOLLOW_SPEED);
    wheels.Pivot_R(90);
  }
  angle = 180;
}
