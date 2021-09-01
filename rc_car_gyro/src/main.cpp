#include <Arduino.h>
#include "run.h"
#include "mpu.h"
#include "pid.h"


void setup() {
  mpu_setup();
  pid_setup();
}

void loop() {
  run_do();
}