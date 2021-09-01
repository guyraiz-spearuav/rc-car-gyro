#include <Arduino.h>
#include "mpu.h"
#include "input.h"
#include "control.h"
#include "output.h"


void setup() {
  Serial.begin(115200);
  mpu_setup();
  input_setup();
  control_setup();
  output_setup();
}

void loop() {
  input_do();
}