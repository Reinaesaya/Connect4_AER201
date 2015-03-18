#ifndef CONSTANTS_H
#define CONSTANTS_H

// Encoders
#define R_ENC_PINA 21
#define R_ENC_PINB 20
#define L_ENC_PINA 19
#define L_ENC_PINB 18

#define R_ENC_PINA_INT 2
#define R_ENC_PINB_INT 3
#define L_ENC_PINA_INT 4
#define L_ENC_PINB_INT 5

#define TICKS_PER_90_DEG 80000 // This needs to be changed

// Wheels
#define WHEEL_ENABLE 13
#define LEFT_WHEEL_F 12
#define LEFT_WHEEL_B 11
#define RIGHT_WHEEL_F 10
#define RIGHT_WHEEL_B 9

// Ultrasonic sensors
#define FRONT_TRIG 22
#define FRONT_ECHO 23
#define LEFT_TRIG 24
#define LEFT_ECHO 25
#define RIGHT_TRIG 26
#define RIGHT_ECHO 27

// Wall following parameters
#define RANGE_ARRAY_LEN 6
#define ADJUSTMENT_MILLIS 20
#define STRAIGHT_ADJ_INNER 0
#define STRAIGHT_ADJ_OUTER 75
#define DIST_ADJ_INNER 50
#define DIST_ADJ_OUTER 75

//

#endif
