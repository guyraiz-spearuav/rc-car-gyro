#include <Arduino.h>
#include <PID_v1.h>
#include "pid.h"

double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void pid_setup()
{
  Setpoint = 0;
  myPID.SetMode(AUTOMATIC);
}

void pid_do()
{
 // Input = analogRead(0);
  myPID.Compute();
 // analogWrite(1, Output);
}