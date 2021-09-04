#ifndef _INPUT_SBUS_H
#define _INPUT_SBUS_H

void input_sbus_setup();
void input_sbus_do();
void pass_values_to_pid_tune(bool store_ch_value, int selection_ch_value, int tuning_ch_value);

#endif