#include <Arduino.h>
#include <PID_v1.h>
#include "control.h"
#include "output.h"

double map_to_viable_rate_value(int value);
double map_to_viable_pwm_value(int value);
void control_do();

const int MIN_REQUESTED_RATE = -500;
const int MAX_REQUESTED_RATE = 500;
const int MIN_RATE_FROM_GYRO = -500;
const int MAX_RATE_FROM_GYRO = 500;
const int MIN_PPM_DELTA = -500;
const int MAX_PPM_DELTA = 500;
const int MIN_INPUT_PPM_US = 800;
const int MAX_INPUT_PPM_US = 2200;

double setpoint, input, output;
double my_Kp = 1;
double my_Ki = 0;
double my_Kd = 0;

PID steeringPID(&input, &output, &setpoint, my_Kp, my_Ki, my_Kd, DIRECT);

int my_rotational_rate;
int my_steering_value;
int my_throttle_value;
int my_aux_value;
bool my_control_active;
bool my_failsafe;

int steering_output;
int throttle_output;

void pass_pid_values(double Kp, double Ki, double Kd)
{
  my_Kp = Kp / 100;
  my_Ki = Ki / 100;
  my_Kd = Kd / 100;
}
void control_setup()
{
  setpoint = 0;
  steeringPID.SetSampleTime(20);
  steeringPID.SetOutputLimits(-500, 500);
  steeringPID.SetMode(AUTOMATIC);
}

void pass_values_to_control(double rotational_rate, int steering_in_value, int throttle_in_value, int aux_in_value, bool failsafe)
{
  my_rotational_rate = map_to_viable_rate_value(rotational_rate);
  my_steering_value = map_to_viable_pwm_value(steering_in_value);
  my_throttle_value = map_to_viable_pwm_value(throttle_in_value);
  my_aux_value = aux_in_value;
  my_failsafe = failsafe;
  control_do();
  Serial.println(aux_in_value);
}

void control_do()
{
  my_control_active = my_aux_value > 1500;
  if (my_control_active)
  {
    input = double(my_rotational_rate);
    setpoint = double(my_steering_value);
    steeringPID.Compute();
    steering_output = output;
  }
  else
  {
    steering_output = my_steering_value;
  }
  throttle_output = my_throttle_value;
  if (my_failsafe)
  {
    steering_output = 0;
    throttle_output = 0;
  }
  update_output(steering_output, throttle_output);
}

double map_to_viable_rate_value(int value)
{
  return (double(map(value, MIN_RATE_FROM_GYRO, MAX_RATE_FROM_GYRO, MIN_REQUESTED_RATE, MAX_REQUESTED_RATE)));
}
double map_to_viable_pwm_value(int value)
{
  return (double(map(value, MIN_INPUT_PPM_US, MAX_INPUT_PPM_US, MIN_PPM_DELTA, MAX_PPM_DELTA)));
}