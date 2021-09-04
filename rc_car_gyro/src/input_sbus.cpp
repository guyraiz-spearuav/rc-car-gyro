#include <Arduino.h>
#include "input_sbus.h"
#include "SBUS.h"

const int TUNE_DOWN = 0;
const int TUNE_NEUTRAL = 1;
const int TUNE_UP = 2;
const int KP_SELECTION = 0;
const int KI_SELECTION = 1;
const int KD_SELECTION = 2;
const int FIRST_RANGE_MIN_US = 1000;
const int FIRST_BARRIER_US = 1300;
const int SECOND_BARRIER_US = 1700;
const int THIRD_RANGE_MAX_US = 2000;
const int MID_VALUE_US = 1500;
const int STORE_CH = 0;
const int SELECTION_CH = 1;
const int TUNING_CH = 2;

bool store_ch_value;
int selection_ch_value;
int tuning_ch_value;

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
    store_ch_value = channels[STORE_CH] > MID_VALUE_US;
    if (channels[SELECTION_CH] > SECOND_BARRIER_US)
      selection_ch_value = KP_SELECTION;
    else if (channels[SELECTION_CH] > FIRST_BARRIER_US)
      selection_ch_value = KI_SELECTION;
    else
      selection_ch_value = KD_SELECTION;
    if (channels[TUNING_CH] > SECOND_BARRIER_US)
      tuning_ch_value = TUNE_DOWN;
    else if (channels[TUNING_CH] > FIRST_BARRIER_US)
      tuning_ch_value = TUNE_NEUTRAL;
    else
      tuning_ch_value = TUNE_UP;
  }
  pass_values_to_pid_tune(store_ch_value, selection_ch_value, tuning_ch_value);
}