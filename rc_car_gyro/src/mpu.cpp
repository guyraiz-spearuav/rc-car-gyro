#include <Arduino.h>
#include <Wire.h>
#include "MPU6050_tockn.h"
#include "mpu.h"

MPU6050 mpu6050(Wire);


void mpu_setup() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void mpu_do(){
mpu6050.getGyroZ();
}