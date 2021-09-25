#include <ARDUINO.h>
#include <EEPROM.h>
#include "pid_tune.h"
#include "input_sbus.h"
#include "control.h"
#include "buzzer.h"

int get_from_eeprom(int address);
void constrain_value_to_byte();
void tune(int my_selection, int my_tuning);
void store_values_to_eeprom();

const int BYTE_VALUE_MIN = 0;
const int BYTE_VALUE_MAX = 255;
const int PID_ADDRESS[3] = {0, 1, 2};
const int GYRO_ADDRESS = 3;
const int PID_SELECT[3] = {0, 1, 2};
const int TUNE_DOWN = 1;
const int TUNE_NEUTRAL = 0;
const int TUNE_UP = -1;
const int TUNE_TIMEOUT = 250;
const int SHORT = 0;
const int LONG = 1;

double my_pid[3];
bool my_store;
bool store_previous;
int my_selection;
int my_tuning;
int my_gyro_ch;
int my_gyro_rate;
long unsigned int tune_timer = 0;
long unsigned int gyro_timer = 0;

void pass_values_to_pid_tune(bool store_ch_value, int selection_ch_value, int tuning_ch_value, int gyro_ch_value)
{
    my_store = store_ch_value;
    my_selection = selection_ch_value;
    my_tuning = tuning_ch_value;
    my_gyro_ch = gyro_ch_value;
    pid_tune_do();
}

void pid_tune_setup()
{
    for (int i = 0; i <= 2; i++)
    {
        my_pid[i] = get_from_eeprom(PID_ADDRESS[i]);
    }
    my_gyro_rate = get_from_eeprom(GYRO_ADDRESS);
    pass_pid_values(my_pid[0], my_pid[1], my_pid[2], my_gyro_rate);
}

void pid_tune_do()
{
    if (my_gyro_ch == 0)
        gyro_timer = millis();
    else if (millis() - gyro_timer > TUNE_TIMEOUT)
    {
        my_gyro_rate += my_gyro_ch;
        constrain_value_to_byte();
        pass_pid_values(my_pid[0], my_pid[1], my_pid[2], my_gyro_rate);
        gyro_timer = millis();
        buzz(SHORT, 1);
    }
    if (my_tuning == TUNE_NEUTRAL)
        tune_timer = millis();
    else if (millis() - tune_timer > TUNE_TIMEOUT)
    {
        tune(my_selection, my_tuning);
        pass_pid_values(my_pid[0], my_pid[1], my_pid[2], my_gyro_rate);
        tune_timer = millis();
        buzz(SHORT, 1);
    }

    if (my_store && my_store != store_previous)
    {
        store_values_to_eeprom();
        buzz(LONG, 3);
    }
    store_previous = my_store;
    buzzer_do();
}

void tune(int pid, int up_or_down)
{
    my_pid[pid] += up_or_down;
    constrain_value_to_byte();
}

int get_from_eeprom(int address)
{
    return (EEPROM.read(address));
}

void constrain_value_to_byte()
{
    for (int i = 0; i <= 2; i++)
    {
        my_pid[i] = constrain(my_pid[i], BYTE_VALUE_MIN, BYTE_VALUE_MAX);
    }
    my_gyro_rate = constrain(my_gyro_rate, BYTE_VALUE_MIN, BYTE_VALUE_MAX);
}

void store_values_to_eeprom()
{
    for (int i = 0; i <= 2; i++)
    {
        EEPROM.write(PID_ADDRESS[i], my_pid[i]);
    }
    EEPROM.write(GYRO_ADDRESS, my_gyro_rate);
}