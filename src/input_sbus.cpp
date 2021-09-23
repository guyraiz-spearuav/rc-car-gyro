#include <Arduino.h>
#include "input_sbus.h"
#include "SBUS.h"

const int TUNE_DOWN = 0;
const int TUNE_NEUTRAL = 1;
const int TUNE_UP = 2;
const int KP_SELECTION = 0;
const int KI_SELECTION = 1;
const int KD_SELECTION = 2;
const int SBUS_MIN_US = 170;
const int SBUS_MAX_US = 1700;
const int STORE_CH = 0;
const int SELECTION_CH = 1;
const int TUNING_CH = 2;
const int PID_ENABLE_CH = 3;

bool presence;
bool store_ch_value;
int selection_ch_value;
int tuning_ch_value;
bool enable_ch_value;

SBUS rxsr(Serial1);

uint16_t channels[16];
bool failSafe;
bool lostFrame;

void input_sbus_setup()
{
  rxsr.begin();
}

void input_sbus_do()
{
  if (rxsr.read(&channels[0], &failSafe, &lostFrame))
  {
    presence = !failSafe && channels[STORE_CH] > SBUS_MIN_US;
    store_ch_value = map(channels[STORE_CH], SBUS_MIN_US, SBUS_MAX_US, 0, 1);
    selection_ch_value = map(channels[SELECTION_CH], SBUS_MIN_US, SBUS_MAX_US, 0, 2);
    tuning_ch_value = map(channels[TUNING_CH], SBUS_MIN_US, SBUS_MAX_US, -1, 1);
    enable_ch_value = map(channels[PID_ENABLE_CH], SBUS_MIN_US, SBUS_MAX_US, 0, 1);
  }
  if (presence)
  {
    pass_values_to_pid_tune(store_ch_value, selection_ch_value, tuning_ch_value);
    pass_sbus_values_to_control(enable_ch_value);
  }
}