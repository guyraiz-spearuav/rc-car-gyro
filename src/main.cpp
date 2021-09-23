#include <Arduino.h>
#include "mpu.h"
#include "input_pwm.h"
#include "input_sbus.h"
#include "control.h"
#include "pid_tune.h"
#include "output.h"
#include "buzzer.h"


void setup() {
  Serial.begin(115200);
  mpu_setup();
  input_sbus_setup();
  input_pwm_setup();
  control_setup();
  output_setup();
  pid_tune_setup();
  input_sbus_setup();
  buzzer_setup();
}

void loop() {
  input_pwm_do();
  input_sbus_do();
}