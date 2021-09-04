#include <ARDUINO.h>
#include <EEPROM.h>
#include "pid_tune.h"
#include "input_sbus.h"

int get_form_eeprom(int address);
int constrain_value_to_byte(int value);
void tune_up();
void tune_down();
void store_values_to_eeprom();

const int BYTE_VALUE_MIN = 0;
const int BYTE_VALUE_MAX = 255;
const int KP_ADDRESS = 0;
const int KI_ADDRESS = 1;
const int KD_ADDRESS = 2;
const int KP_SELECT = 0;
const int KI_SELECT = 1;
const int KD_SELECT = 2;
const int TUNE_DOWN = 0;
const int TUNE_NEUTRAL = 1;
const int TUNE_UP = 2;
const int TUNE_TIMEOUT = 500;

int my_Kp_value;
int my_Ki_value;
int my_Kd_value;
bool my_store;
bool store_previous;
bool my_selection;
bool my_tuning;
long unsigned int tune_timer = 0;

void pass_values_to_pid_tune(bool store_ch_value, int selection_ch_value, int tuning_ch_value)
{
    my_store = store_ch_value;
    my_selection = selection_ch_value;
    my_tuning = tuning_ch_value;
    pid_tune_do();
}

void pid_tune_setup()
{
    my_Kp_value = get_form_eeprom(KP_ADDRESS);
    my_Ki_value = get_form_eeprom(KI_ADDRESS);
    my_Kd_value = get_form_eeprom(KD_ADDRESS);
}

void pid_tune_do()
{
    if (my_tuning == TUNE_DOWN && millis() - tune_timer > TUNE_TIMEOUT)
    {
        tune_down();
        tune_timer = millis();
    }
    if (my_tuning == TUNE_NEUTRAL)
        tune_timer = millis();
    if (my_tuning == TUNE_UP && millis() - tune_timer > TUNE_TIMEOUT)
    {
        tune_up();
        tune_timer = millis();
    }

    my_Kp_value = constrain_value_to_byte(my_Kp_value);
    my_Ki_value = constrain_value_to_byte(my_Ki_value);
    my_Kd_value = constrain_value_to_byte(my_Kd_value);

    if (my_store && my_store != store_previous)
        store_values_to_eeprom();
    store_previous = my_store;
}

void tune_down()
{
    if (my_selection == KP_SELECT)
        my_Kp_value -= 1;
    if (my_selection == KI_SELECT)
        my_Ki_value -= 1;
    if (my_selection == KD_SELECT)
        my_Kd_value -= 1;
}
void tune_up()
{
    if (my_selection == KP_SELECT)
        my_Kp_value += 1;
    if (my_selection == KI_SELECT)
        my_Ki_value += 1;
    if (my_selection == KD_SELECT)
        my_Kd_value += 1;
}

int get_form_eeprom(int address)
{
    return (EEPROM.read(address));
}

int constrain_value_to_byte(int value)
{
    return (constrain(value, BYTE_VALUE_MIN, BYTE_VALUE_MAX));
}

void store_values_to_eeprom()
{
    EEPROM.write(KP_ADDRESS, my_Kp_value);
    EEPROM.write(KI_ADDRESS, my_Ki_value);
    EEPROM.write(KD_ADDRESS, my_Kd_value);
}