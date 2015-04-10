#include "Ultrasonic.h"
#include "Diff_steer.h"

#define FRONT_TRIG 22
#define FRONT_ECHO 23
#define LEFT_TRIG 24
#define LEFT_ECHO 25
#define RANGE_ARRAY_LEN 6

Ultrasonic us_f(FRONT_TRIG,FRONT_ECHO);
Ultrasonic us_l(LEFT_TRIG,LEFT_ECHO);
long range_arr_f[RANGE_ARRAY_LEN];
long range_arr_l[RANGE_ARRAY_LEN];

DiffSteering diffsteering(13,12,11,10,9);

long loops = 0;
int turned = 0;

void setup() {
  Serial.begin(9600);
  init_us(us_f, us_l, range_arr_f, range_arr_l);
  check_array(range_arr_f, RANGE_ARRAY_LEN, 30);
  check_array(range_arr_l, RANGE_ARRAY_LEN, 30);
  delay(5000);
}

void loop()
{
  long obj_range_front = us_f.Ranging(CM);
  long obj_range_left = us_l.Ranging(CM);
  shift_add(range_arr_f, RANGE_ARRAY_LEN, obj_range_front);
  shift_add(range_arr_l, RANGE_ARRAY_LEN, obj_range_left);
  Serial.println("Left");
  check_array(range_arr_l, RANGE_ARRAY_LEN, 30);
  Serial.println("Front");
  if (turned)
  {
    //while (loops < 200 )
    while (check_array(range_arr_f, RANGE_ARRAY_LEN, 63) != 1)
    {
      long obj_range_front = us_f.Ranging(CM);
      long obj_range_left = us_l.Ranging(CM);
      shift_add(range_arr_f, RANGE_ARRAY_LEN, obj_range_front);
      shift_add(range_arr_l, RANGE_ARRAY_LEN, obj_range_left);
      if (loops % 2 == 0) wall_straight_adjust(range_arr_l, 200);
      else wall_dist_adjust(range_arr_l, 40, 200);
      loops++;
    }
    diffsteering.Stop();
    delay(1000);
    diffsteering.Pivot_L(220);
    diffsteering.Stop();
    delay(100000);
  }
  else if (check_array(range_arr_f, RANGE_ARRAY_LEN, 30) == 1) {
    diffsteering.Stop();
    diffsteering.Pivot_R(210);
    diffsteering.Stop();
    turned = 1;
    init_us(us_f, us_l, range_arr_f, range_arr_l);
    loops = 0;
    delay(5000);
  }
  else {
    if (loops % 2 == 0) wall_straight_adjust(range_arr_l, 200);
    else wall_dist_adjust(range_arr_l, 15, 200);
    loops++;
  }
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

void wall_straight_adjust(long *arr, int SPD)
{
  int adjust = 0;
  int mid = ((RANGE_ARRAY_LEN+1)/2);
  for (int i = mid; i < RANGE_ARRAY_LEN; ++i)
  {
    for (int j = 0; j < mid; ++j)
    {
      if (arr[i] < arr[j]) --adjust;
      else if (arr[i] > arr[j]) ++adjust;
      else continue;
    }
  }
  Serial.print("Adjust Value: ");
  Serial.println(adjust);
  int thresh = (RANGE_ARRAY_LEN - (RANGE_ARRAY_LEN/2))*(RANGE_ARRAY_LEN/2) - (RANGE_ARRAY_LEN/2 + 1);
  if (adjust < (thresh*(-1)))    diffsteering.Turn_R(20, 0,150);
  else if (adjust > thresh)      diffsteering.Turn_L(20, 0,150);
  else {                         diffsteering.Forward(SPD);
                                 delay(10); }
}

void wall_dist_adjust(long *arr, long targ, int SPD)
{
  int adjust = 0;
  for (int i = 0; i < RANGE_ARRAY_LEN; ++i)
  {
    if (arr[i] < targ) --adjust;
    else if (arr[i] > targ) ++adjust;
    else continue;
  }
  Serial.print("Adjust Value: ");
  Serial.println(adjust);
  int thresh = RANGE_ARRAY_LEN - 2;
  if (adjust < (thresh*(-1)))    diffsteering.Turn_R(20, 100,150);
  else if (adjust > thresh)      diffsteering.Turn_L(20, 100,150);
  else {                         diffsteering.Forward(SPD);
                                 delay(10); }
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
