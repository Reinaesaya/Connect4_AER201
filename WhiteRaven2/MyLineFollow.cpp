#include "MyLineFollow.h"
#include "Arduino.h"
#include "Basic.h"
#include "Wall.h"
#include "constants.h"

void line_follow_intersection(Wheels& wheels, int& num_intersections)
{
  int intersection_hit = 0;
  while (!intersection_hit)
  {
    int F_R = analogRead(FRONT_RIGHT);
    int F_L = analogRead(FRONT_LEFT);
    int B_R = analogRead(BACK_RIGHT);
    int B_L = analogRead(BACK_LEFT);
    
    if ((F_L > (MAX_WHITE_F_L+B_W_THRESH)) && (F_R < (MAX_WHITE_F_R+B_W_THRESH)))
    {
      wheels.Turn_L(50,0,90);
    }
    else if ((F_R > (MAX_WHITE_F_R+B_W_THRESH)) && (F_L < (MAX_WHITE_F_L+B_W_THRESH)))
    {
      wheels.Turn_R(50,0,90);
    }
    else if ((B_R > (MAX_WHITE_B_R+B_W_THRESH)) || (B_L > (MAX_WHITE_B_L+B_W_THRESH)))
    {
      int B_R_check[ARRAY_LEN];
      int B_L_check[ARRAY_LEN];
      for (int i=0; i<=(ARRAY_LEN*2); ++i)
      {
         int B_R_val = analogRead(BACK_RIGHT);
         int B_L_val = analogRead(BACK_LEFT); 
         shift_add(B_R_check, B_R_val);   
         shift_add(B_L_check, B_L_val);
      }
      int state1 = check_array_half(B_R_check, MAX_WHITE_B_R+B_W_THRESH);
      int state2 = check_array_half(B_L_check, MAX_WHITE_B_L+B_W_THRESH);
      if ( ((B_R > (MAX_WHITE_B_R+B_W_THRESH)) && state1) ||
           ((B_L > (MAX_WHITE_B_L+B_W_THRESH)) && state2) ||
           (state1 == 0 && state2 == 0) )  
      {
        intersection_hit = 1;
        wheels.Stop();
      }
    }
    else { wheels.Forward(LINE_FOLLOW_SPEED); }
  }
  num_intersections++;
}

void line_follow_n_intersections(Wheels& wheels, int num)
{
  Serial.println(num);
  int num_intersections = 0;
  while (num_intersections < num)
  {
    wheels.Forward(LINE_FOLLOW_SPEED, LINE_FOLLOW_FORWARD_TICK_OFFSET);
    line_follow_intersection(wheels, num_intersections);
    Serial.println(num_intersections);
  }
  wheels.Stop();
}

void grid_follow(Wheels& wheels, int turn_dir, int forward_inter, int side_inter)
{
  line_follow_n_intersections(wheels, forward_inter);
  if (turn_dir == LEFT_TURN) { wheels.Pivot_L(90); }
  else if (turn_dir == RIGHT_TURN) { wheels.Pivot_R(90); }
  line_follow_n_intersections(wheels, side_inter);
}

void get_to_hopper_pos(Wheels& wheels, Hopper& hopper, Ultrasonic& front_US, Ultrasonic& right_US, Ultrasonic& left_US, float& angle)
{
  if (hopper.side == LEFT_BOARD)
  {
    grid_follow(wheels, LEFT_TURN, hopper.stop_forw, hopper.stop_side);
    wheels.Forward(75, hopper.tick_offset);
    if (hopper.corner == CORNER_HOP) { wheels.Pivot_R(135); }
    else if (hopper.corner == NOT_CORNER_HOP)
    {
      if (hopper.stop_forw > 3)
      {
        wheels.Pivot_L(90);
        forward_to_dist_wall(hopper.side, HOPPER_STOP_DIST + 10, hopper.dist_to_wall, wheels, front_US, left_US, 125);
      }
      else 
      {
        wheels.Pivot_R(90);
        forward_to_dist_wall(!hopper.side, HOPPER_STOP_DIST + 10, hopper.dist_to_wall, wheels, front_US, right_US, 125);
      }
    }
  }
  else if (hopper.side == RIGHT_BOARD)
  {
    Serial.println("Got here");
    grid_follow(wheels, RIGHT_TURN, hopper.stop_forw, hopper.stop_side);
    wheels.Forward(75, hopper.tick_offset);
    if (hopper.corner == CORNER_HOP) { wheels.Pivot_L(135); }
    else if (hopper.corner == NOT_CORNER_HOP)
    {
      if (hopper.stop_forw > 3)
      {
        wheels.Pivot_R(90);
        forward_to_dist_wall(hopper.side, HOPPER_STOP_DIST + 5, hopper.dist_to_wall, wheels, front_US, right_US, 125);
      }
      else 
      {
        wheels.Pivot_L(90);
        forward_to_dist_wall(!hopper.side, HOPPER_STOP_DIST + 5, hopper.dist_to_wall, wheels, front_US, left_US, 125);
      }
    }
  }
  angle = hopper.angle;
}

void get_to_origin_from_gameboard(Wheels& wheels, Hopper& hopper)
{
  if (hopper.side == LEFT_BOARD)
  {
    wheels.Pivot_L(90);
    line_follow_n_intersections(wheels, 3);
    wheels.Pivot_R(90);
  }
  else if (hopper.side == RIGHT_BOARD)
  {
    wheels.Pivot_R(90);
    line_follow_n_intersections(wheels, 3);
    wheels.Pivot_L(90);
  }
}
