#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Servo.h>

Servo steering_servo;

MPU6050 mpu6050(Wire);
double kp = 1;
double ki = 0;
double kd = 0;

const int steering_servo_pin = 9;

long unsigned int currentTime, previousTime;
double elapsedTime;
double cumError;
double rateError;
double lastError;

double omega;

int steering_in;
int steering_out;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  steering_servo.attach(steering_servo_pin);

}

void loop() {
  mpu6050.update();
  omega = mpu6050.getGyroZ();
  steering_in = map(1500/*pulseIn(9, HIGH, 3000)*/, 1000, 2000, -500, 500);
  steering_out = 1500 + computePID(omega, steering_in);
  steering_out = constrain(steering_out, 1000, 2000);
  steering_servo.writeMicroseconds(steering_out);
//  Serial.print(mpu6050.getGyroZ());
//  Serial.print("   ");
  Serial.println(steering_out);
}


double computePID(double Input, double SetPoint) {
  currentTime = millis();                //get current time
  elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation

  double error = SetPoint - Input;                                // determine error
  cumError += error * elapsedTime;                // compute integral
  rateError = (error - lastError) / elapsedTime; // compute derivative

  double out = kp * error + ki * cumError + kd * rateError;          //PID output

  lastError = error;                                //remember current error
  previousTime = currentTime;                        //remember current time

  return out;                                        //have function return the PID output
}
