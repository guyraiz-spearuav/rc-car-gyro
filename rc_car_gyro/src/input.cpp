#include <Arduino.h>
#include "input.h"
#include "mpu.h"
#include "control.h"

int map_to_regulated_values(int value, int min, int max);

const int STEERING_IN_PIN = 4;
const int THROTTLE_IN_PIN = 5;
const int AUX_IN_PIN = 6;
int min_sig_from_rc_steering = 1200;
int max_sig_from_rc_steering = 1800;
int min_sig_from_rc_throttle = 1200;
int max_sig_from_rc_throttle = 1800;

const int MID_SIG_TO_CONTROL = 1500;
const int MIN_SIG_TO_CONTROL = 800;
const int MAX_SIG_TO_CONTROL = 2200;

int steering_in_value;
int throttle_in_value;
int rotational_rate;
bool aux_in_value;

void input_setup()
{
  pinMode(STEERING_IN_PIN, INPUT);
  pinMode(THROTTLE_IN_PIN, INPUT);
  pinMode(AUX_IN_PIN, INPUT);
}

void input_do()
{
  steering_in_value = pulseIn(STEERING_IN_PIN, HIGH);
  throttle_in_value = pulseIn(THROTTLE_IN_PIN, HIGH);
  aux_in_value = pulseIn(AUX_IN_PIN, HIGH) > MID_SIG_TO_CONTROL;
  if (steering_in_value > max_sig_from_rc_steering)
    max_sig_from_rc_steering = steering_in_value;
  if (steering_in_value < min_sig_from_rc_steering)
    min_sig_from_rc_steering = steering_in_value;
  if (throttle_in_value > max_sig_from_rc_throttle)
    max_sig_from_rc_throttle = throttle_in_value;
  if (throttle_in_value < min_sig_from_rc_throttle)
    min_sig_from_rc_throttle = throttle_in_value;
  steering_in_value = map_to_regulated_values(steering_in_value, min_sig_from_rc_steering, max_sig_from_rc_steering);
  throttle_in_value = map_to_regulated_values(throttle_in_value, min_sig_from_rc_throttle, max_sig_from_rc_throttle);
  rotational_rate = mpu_get_rate();
  pass_values_to_control(rotational_rate, steering_in_value, throttle_in_value, aux_in_value);
}

int map_to_regulated_values(int value, int min, int max)
{
  return (map(value, min, max, MIN_SIG_TO_CONTROL, MAX_SIG_TO_CONTROL));
}