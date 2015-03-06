#include "Ultrasonic.h"
#include "Diff_steer.h"

#define FRONT_TRIG 22
#define FRONT_ECHO 23
#define LEFT_TRIG 44
#define LEFT_ECHO 45
#define RANGE_ARRAY_LEN 6
#define DIST_BTW_COLUMNS 2

Ultrasonic us_f(FRONT_TRIG,FRONT_ECHO);
Ultrasonic us_l(LEFT_TRIG,LEFT_ECHO);
long range_arr_f[RANGE_ARRAY_LEN];
long range_arr_l[RANGE_ARRAY_LEN];

DiffSteering diffsteering(13,12,11,10,9);

void setup() {
  Serial.begin(9600);
  init_us(us_f, us_l, range_arr_f, range_arr_l);
  check_array(range_arr_f, RANGE_ARRAY_LEN, 30);
  check_array(range_arr_l, RANGE_ARRAY_LEN, 30);
  delay(1000);
}

void loop()
{
  int incomingByte = 0;
  int column = 0;
  long obj_range_front = us_f.Ranging(CM);
  long obj_range_left = us_l.Ranging(CM);
  shift_add(range_arr_f, RANGE_ARRAY_LEN, obj_range_front);
  shift_add(range_arr_l, RANGE_ARRAY_LEN, obj_range_left);
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    column = incomingByte - 48;
    Serial.print("Column Selected: ");
    Serial.println(column);
    
    long current_dist = us_f.Ranging(CM);
    Serial.println("current_dist");
    if (column < 4)
    {
      diffsteering.Pivot_L(220);
      init_us(us_f, us_l, range_arr_f, range_arr_l);
      long targ = range_arr_f[3] - ((4-column)*DIST_BTW_COLUMNS);
      move_2_dist(us_f, range_arr_f, targ, diffsteering);
      diffsteering.Pivot_R(220);
      diffsteering.Stop();
      delay(1000);
      init_us(us_f, us_l, range_arr_f, range_arr_l);
      dispense(us_f, range_arr_f, diffsteering, 20, current_dist);
    }
    else if (column > 4)
    {
      diffsteering.Pivot_R(220);
      init_us(us_f, us_l, range_arr_f, range_arr_l);
      long targ = range_arr_f[3] - ((column-4)*DIST_BTW_COLUMNS);
      move_2_dist(us_f, range_arr_f, targ, diffsteering);
      diffsteering.Pivot_L(220);
      diffsteering.Stop();
      delay(1000);
      init_us(us_f, us_l, range_arr_f, range_arr_l);
      dispense(us_f, range_arr_f, diffsteering, 20, current_dist);
    }
    else
    {
      dispense(us_f, range_arr_f, diffsteering, 20, current_dist);
    }
  }
}

void move_2_dist(Ultrasonic us, long *arr, long targ, DiffSteering wheels)
{
  while (check_array(arr, RANGE_ARRAY_LEN, targ) != 1) {
    long obj_range_front = us.Ranging(CM);
    shift_add(arr, RANGE_ARRAY_LEN, obj_range_front);
    wheels.Forward(100);
    delay(10);
  }
  wheels.Stop();
  delay(1000);
}

void init_us(Ultrasonic front, Ultrasonic left, long *afront, long *aleft)
{
  for (int i = 0; i <= (RANGE_ARRAY_LEN*2); ++i) {
    long r_front = front.Ranging(CM);
    long r_left = left.Ranging(CM);
    shift_add(afront, RANGE_ARRAY_LEN, r_front);
    shift_add(aleft, RANGE_ARRAY_LEN, r_left);
    delay(10);
  }
}

void dispense(Ultrasonic front, long *afront, DiffSteering wheels, long dist, long end_d)
{
  while (check_array(afront, RANGE_ARRAY_LEN, dist) != 1) {
    long obj_range_front = front.Ranging(CM);
    shift_add(afront, RANGE_ARRAY_LEN, obj_range_front);
    wheels.Forward(150);
    delay(10);
  }
  wheels.Stop();
  delay(1000);
  while (check_array(afront, RANGE_ARRAY_LEN, end_d) == 1) {
    long obj_range_front = front.Ranging(CM);
    shift_add(afront, RANGE_ARRAY_LEN, obj_range_front);
    wheels.Backward(150);
    delay(10);
  }
  wheels.Stop();
  delay(1000);
}

void shift_add(long* arr, int length, long b) {
  for (int i = 0; i < length; i++) {
    *(arr + i) = *(arr + i + 1);
  }
  arr[length-1] = b;
}

int check_array(long* arr, int length, long thresh) {
  
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
