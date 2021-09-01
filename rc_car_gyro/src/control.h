#ifndef _CONTROL_H
#define _CONTROL_H


void control_setup();
void pass_values_to_control(double rotational_rate, int steering_in_value, int throttle_in_value, bool aux_in_value, bool failsafe);
#endif
