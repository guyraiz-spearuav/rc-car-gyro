#include <Arduino.h>
#include "input.h"
#include "mpu.h"
#include "control.h"

int map_to_regulated_values(int value, int min, int max);
void steering_pin_interrupt();
void throttle_pin_interrupt();
void aux_pin_interrupt();

const int STEERING_IN_PIN = 4;
const int THROTTLE_IN_PIN = 5;
const int AUX_IN_PIN = 6;
const int PULSEIN_TIMEOUT = 30000;

volatile long unsigned int steering_pulse_begin;
volatile long unsigned int steering_pulse_end;
volatile bool steering_pulse_available;
volatile long unsigned int throttle_pulse_begin;
volatile long unsigned int throttle_pulse_end;
volatile bool throttle_pulse_available;
volatile long unsigned int aux_pulse_begin;
volatile long unsigned int aux_pulse_end;
volatile bool aux_pulse_available;

int min_sig_from_rc_steering = 1200;
int max_sig_from_rc_steering = 1800;
int min_sig_from_rc_throttle = 1200;
int max_sig_from_rc_throttle = 1800;
int min_sig_from_rc_aux = 1200;
int max_sig_from_rc_aux = 1800;

const int MID_SIG_TO_CONTROL = 1500;
const int MIN_SIG_TO_CONTROL = 800;
const int MAX_SIG_TO_CONTROL = 2200;

int steering_in_value;
int throttle_in_value;
int rotational_rate;
int aux_in_value;
bool failsafe;

void input_setup()
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

void input_do()
{
  if (steering_pulse_available)
  {
    steering_in_value = steering_pulse_begin - steering_pulse_end;
    steering_pulse_available = false;
  }
  if (throttle_pulse_available)
  {
    throttle_in_value = throttle_pulse_begin - throttle_pulse_end;
    throttle_pulse_available = false;
  }
  if (aux_pulse_available)
  {
    aux_in_value = aux_pulse_begin - aux_pulse_end;
    aux_pulse_available = false;
  }
  if (millis() - PULSEIN_TIMEOUT > steering_pulse_begin / 1000)
    steering_in_value = 0;
  if (millis() - PULSEIN_TIMEOUT > throttle_pulse_begin / 1000)
    throttle_in_value = 0;
  if (millis() - PULSEIN_TIMEOUT > aux_pulse_begin / 1000)
    aux_in_value = 0;
  if (steering_in_value == 0 || throttle_in_value == 0 || aux_in_value == 0)
    failsafe = true;
  else
    failsafe = false;
  if (steering_in_value > max_sig_from_rc_steering && !failsafe)
    max_sig_from_rc_steering = steering_in_value;
  if (steering_in_value < min_sig_from_rc_steering && !failsafe)
    min_sig_from_rc_steering = steering_in_value;
  if (throttle_in_value > max_sig_from_rc_throttle && !failsafe)
    max_sig_from_rc_throttle = throttle_in_value;
  if (throttle_in_value < min_sig_from_rc_throttle && !failsafe)
    min_sig_from_rc_throttle = throttle_in_value;
  if (aux_in_value > max_sig_from_rc_aux && !failsafe)
    max_sig_from_rc_aux = aux_in_value;
  if (aux_in_value < min_sig_from_rc_aux && !failsafe)
    min_sig_from_rc_aux = aux_in_value;

  steering_in_value = map_to_regulated_values(steering_in_value, min_sig_from_rc_steering, max_sig_from_rc_steering);
  throttle_in_value = map_to_regulated_values(throttle_in_value, min_sig_from_rc_throttle, max_sig_from_rc_throttle);
  aux_in_value = map_to_regulated_values(aux_in_value, min_sig_from_rc_aux, max_sig_from_rc_aux);
  rotational_rate = mpu_get_rate();
  pass_values_to_control(rotational_rate, steering_in_value, throttle_in_value, aux_in_value, failsafe);
}

int map_to_regulated_values(int value, int min, int max)
{
  return (map(value, min, max, MIN_SIG_TO_CONTROL, MAX_SIG_TO_CONTROL));
}

void steering_pin_interrupt()
{
  if (STEERING_IN_PIN)
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
  if (THROTTLE_IN_PIN)
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
  if (AUX_IN_PIN)
  {
    aux_pulse_begin = micros();
  }
  else
  {
    aux_pulse_end = micros();
    aux_pulse_available = true;
  }
}