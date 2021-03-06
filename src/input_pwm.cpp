#include <Arduino.h>
#include "input_pwm.h"
#include "mpu.h"
#include "control.h"

void steering_pin_interrupt();
void throttle_pin_interrupt();
void aux_pin_interrupt();

const int STEERING_IN_PIN = 4;
const int THROTTLE_IN_PIN = 5;
const int AUX_IN_PIN = 6;
const int PULSEIN_TIMEOUT = 100;

volatile long unsigned int steering_pulse_begin;
volatile long unsigned int steering_pulse_end;
volatile bool steering_pulse_available;
volatile long unsigned int throttle_pulse_begin;
volatile long unsigned int throttle_pulse_end;
volatile bool throttle_pulse_available;
volatile long unsigned int aux_pulse_begin;
volatile long unsigned int aux_pulse_end;
volatile bool aux_pulse_available;

const int MID_SIG_TO_CONTROL = 1500;
const int MIN_SIG_TO_CONTROL = 800;
const int MAX_SIG_TO_CONTROL = 2200;

int steering_in_value;
int throttle_in_value;
int aux_in_value;
int steering_out_value;
int throttle_out_value;
int aux_out_value;
int rotational_rate;
bool failsafe;

void input_pwm_setup()
{
  pinMode(STEERING_IN_PIN, INPUT);
  pinMode(THROTTLE_IN_PIN, INPUT);
  pinMode(AUX_IN_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(STEERING_IN_PIN),
                  steering_pin_interrupt,
                  CHANGE);
  attachInterrupt(digitalPinToInterrupt(THROTTLE_IN_PIN),
                  throttle_pin_interrupt,
                  CHANGE);
  attachInterrupt(digitalPinToInterrupt(AUX_IN_PIN),
                  aux_pin_interrupt,
                  CHANGE);
}

void input_pwm_do()
{
  if (steering_pulse_available)
  {
    steering_in_value = steering_pulse_end - steering_pulse_begin;
    steering_pulse_available = false;
  }
  if (throttle_pulse_available)
  {
    throttle_in_value = throttle_pulse_end - throttle_pulse_begin;
    throttle_pulse_available = false;
  }
  if (aux_pulse_available)
  {
    aux_in_value = aux_pulse_end - aux_pulse_begin;
    aux_pulse_available = false;
  }
  if ((millis() - PULSEIN_TIMEOUT) > (steering_pulse_end / 1000))
    steering_in_value = 0;
  if ((millis() - PULSEIN_TIMEOUT) > (throttle_pulse_end / 1000))
    throttle_in_value = 0;
  if ((millis() - PULSEIN_TIMEOUT) > (aux_pulse_end / 1000))
    aux_in_value = 0;
  if (steering_in_value == 0 || throttle_in_value == 0 || aux_in_value == 0)
    failsafe = true;
  else
    failsafe = false;

  steering_out_value = steering_in_value;
  throttle_out_value = throttle_in_value;
  aux_out_value = aux_in_value;
  rotational_rate = mpu_get_rate();
  pass_values_to_control(rotational_rate, steering_out_value, throttle_out_value, aux_out_value, failsafe);
}

void steering_pin_interrupt()
{
  if (digitalRead(STEERING_IN_PIN))
  {
    steering_pulse_begin = micros();
  }
  else
  {
    steering_pulse_end = micros();
    steering_pulse_available = true;
  }
}

void throttle_pin_interrupt()
{
  if (digitalRead(THROTTLE_IN_PIN))
  {
    throttle_pulse_begin = micros();
  }
  else
  {
    throttle_pulse_end = micros();
    throttle_pulse_available = true;
  }
}

void aux_pin_interrupt()
{
  if (digitalRead(AUX_IN_PIN))
  {
    aux_pulse_begin = micros();
  }
  else
  {
    aux_pulse_end = micros();
    aux_pulse_available = true;
  }
}