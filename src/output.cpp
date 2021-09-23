#include <Arduino.h>
#include <PWMServo.h>
#include "output.h"

void output_do();
int map_to_readable_output(int value);
int constrain_output(int value);

const int STEERING_SERVO_PIN = 2;
const int THROTTLE_SERVO_PIN = 3;
const int MIN_PWM_OUTPUT_US = 800;
const int MID_PWM_OUTPUT_US = 1500;
const int MAX_PWM_OUTPUT_US = 2200;
const int MIN_ANGLE_OUTPUT_DEG = 0;
const int MAX_ANGLE_OUTPUT_DEG = 180;
const int SAFETY_TIME = 5000;

int steering_output_value;
int throttle_output_value;
int my_steering_from_control;
int my_throttle_from_control;

PWMServo steering_servo;
PWMServo throttle_servo;

void output_setup()
{
  steering_servo.attach(STEERING_SERVO_PIN, MIN_PWM_OUTPUT_US, MAX_PWM_OUTPUT_US);
  throttle_servo.attach(THROTTLE_SERVO_PIN, MIN_PWM_OUTPUT_US, MAX_PWM_OUTPUT_US);
}

void update_output(int steering_output, int throttle_output)
{
  my_steering_from_control = steering_output;
  my_throttle_from_control = throttle_output;
  output_do();
}

void output_do()
{
  bool safe_timer_passed = millis() > SAFETY_TIME;
  steering_output_value = MID_PWM_OUTPUT_US + my_steering_from_control;
  throttle_output_value = MID_PWM_OUTPUT_US + my_throttle_from_control;
  if (safe_timer_passed)
  {
    steering_servo.write(map_to_readable_output(constrain_output(steering_output_value)));
    throttle_servo.write(map_to_readable_output(constrain_output(throttle_output_value)));
  }
}

int map_to_readable_output(int value)
{
  return map(value, MIN_PWM_OUTPUT_US, MAX_PWM_OUTPUT_US, MIN_ANGLE_OUTPUT_DEG, MAX_ANGLE_OUTPUT_DEG);
}
int constrain_output(int value)
{
  return constrain(value, MIN_PWM_OUTPUT_US, MAX_PWM_OUTPUT_US);
}