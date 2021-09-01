#include <PWMServo.h>
#include "output.h"

PWMServo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void output_setup() {
  myservo.attach(SERVO_PIN_A);  // attaches the servo on pin 9 to the servo object
  //myservo.attach(SERVO_PIN_A, 1000, 2000); // some motors need min/max setting
}


void output_do() {
    myservo.writeMicroseconds(1500);              // tell servo to go to position in variable 'pos'
}