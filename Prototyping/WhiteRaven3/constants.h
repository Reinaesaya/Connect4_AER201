#ifndef CONSTANTS_H
#define CONSTANTS_H

// Robot Dimensions
#define LENGTH_ROBOT 26
#define WIDTH_ROBOT 18
#define PIVOT_TO_FRONT_US 16
#define PIVOT_TO_SIDE_US 11
#define TURN_DIFFERENCE (PIVOT_TO_FRONT_US - PIVOT_TO_SIDE_US)

// Playfield Dimensions
#define GRID_SPACING 20
#define GAMEBOARD_COL_SPACING 2
#define GAMEBOARD_CENTER_FROM_WALL 80

// Encoders pins and interrupts
#define R_ENC_PINA 21
#define R_ENC_PINB 20
#define L_ENC_PINA 19
#define L_ENC_PINB 18

#define R_ENC_PINA_INT 2
#define R_ENC_PINB_INT 3
#define L_ENC_PINA_INT 4
#define L_ENC_PINB_INT 5

#define TICKS_PER_90_DEG 81243
#define TICKS_PER_GRID 65700
// Wheel pins
#define WHEEL_ENABLE 13
#define LEFT_WHEEL_F 12
#define LEFT_WHEEL_B 11
#define RIGHT_WHEEL_F 10
#define RIGHT_WHEEL_B 9

// Wheel movements
#define LEFT_MOTOR_OFFSET 0
#define RIGHT_MOTOR_OFFSET 15
#define PIVOT_SPEED 100

// Ultrasonic sensor pins
#define FRONT_TRIG 22
#define FRONT_ECHO 23
#define LEFT_TRIG 24
#define LEFT_ECHO 25
#define RIGHT_TRIG 26
#define RIGHT_ECHO 27

// Stepper motor parameters and pins
#define STEPPER_STEPS 48
#define STEPPER_1 8
#define STEPPER_2 7
#define STEPPER_3 6
#define STEPPER_4 5

// Ball grabbing parameters
#define STEPPER_SPEED 60
#define STEPPER_NSTEPS 180
#define HOPPER_STOP_DIST 30
#define HOPPER_FORWARD_TICK 40000
#define BALL_GRAB_SPD 125
#define BACK_TURN_MILLI 1200
#define BACK_INNER_SPD 55
#define BACK_OUTER_SPD 150

// Wall following parameters
#define ARRAY_LEN 6
#define ADJUSTMENT_MILLIS 10
#define STRAIGHT_ADJ_INNER 50
#define STRAIGHT_ADJ_OUTER 100
#define DIST_ADJ_INNER 50
#define DIST_ADJ_OUTER 100

// Infrared sensor pins
#define FRONT_FRONT 15
#define FRONT_LEFT 14
#define FRONT_RIGHT 13
#define FRONT_BACK 12
#define BACK_RIGHT 9
#define BACK_LEFT 8

// Line following parameters
#define MAX_WHITE_F_R 750
#define MAX_WHITE_F_L 800
#define MAX_WHITE_B_R 46
#define MAX_WHITE_B_L 47
#define B_W_THRESH 100
#define LINE_FOLLOW_ADJUST 150

// Navigation parameters
#define LEFT_BOARD 0
#define RIGHT_BOARD 1

#define LINE_FOLLOW_SPEED 75
#define LINE_FOLLOW_FORWARD_TICK_OFFSET 7000
#define LEFT_TURN 0
#define RIGHT_TURN 1

#define WALL_FIND_SPEED 75
#define WALL_FOLLOW_SPEED 75
#define SIDE_WALL_FOLLOW_DIST 15
#define GAMEBOARD_WALL_FOLLOW_DIST 25

#define MIN_DIST_DISPENSE 10
#define BACK_UP_DISPENSE_DIST 25

// Hopper parameters

#define CORNER_HOP 1
#define NOT_CORNER_HOP 0
#define FLAT_UP 0
#define FLAT_DOWN 1
#define CLOSE_CENTER 0
#define FAR_CENTER 1


// Keypad pins
#define ROW_PIN_D 34
#define ROW_PIN_C 35
#define ROW_PIN_B 36
#define ROW_PIN_A 37
#define COL_PIN_D 38
#define COL_PIN_C 39
#define COL_PIN_B 40
#define COL_PIN_A 41

#endif
