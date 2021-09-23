#include <Arduino.h>
#include "buzzer.h"

const int BUZZER_PIN = 9;
int BUZZER_TIME_SHORT = 50;
const int OFF = 0;
const int ON = 1;
const int SHORT = 0;
const int LONG = 1;

int my_buzz_times;
int buzzer_long_or_short;
long unsigned int buzzer_timer;

void buzzer_setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
}
void buzz(bool long_or_short, int times)
{
    buzzer_long_or_short = map(long_or_short, 0, 1, 50, 250);
    my_buzz_times = times;
    buzzer_timer = millis();
}

void buzzer_do()
{
    if (millis() < buzzer_timer + buzzer_long_or_short && my_buzz_times > 0)
        tone(BUZZER_PIN, 1000);
    else if (millis() < buzzer_timer + buzzer_long_or_short * 2 && my_buzz_times > 0)
        noTone(BUZZER_PIN);
    else if (millis() > buzzer_timer + buzzer_long_or_short * 2 && my_buzz_times > 0)
    {
        my_buzz_times -= 1;
        buzzer_timer = millis();
    }
}