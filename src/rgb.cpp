#include <Arduino.h>
#include "TeensyThreads.h"
#include"rgb.h"

const int NUM_LEDS = 26;
const int DATA_PIN = 3;
const int SHORT_TIME = 100;
const int LONG = 250;

CRGB leds[NUM_LEDS];
int fr[6] = 0, 1, 2, 3, 4, 5;
int rr[6] = 6, 7, 8, 9, 10, 11;
int rl[6] = 12, 13, 14, 15, 16, 17;
int fl[6] = 18, 19, 20, 21, 22, 23;
volatile int blinkcode = 0;

void blinkthread()
{
    while (true)
    {

        threads.delay(150);
    }
    threads.yield();
}

void rgb_setup()
{
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);

    threads.addThread(blinkthread);
}

int count = 0;

void rgb_solid(CRGB colour)
{
    if (solid_colour)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = colour;
        }
        FastLED.show();
    }
}

void rgb_flash(CRGB colour1, CRGB colour2)
{
    while (flashing_colours)
    {
        rgb_solid(colour1);
        delay(SHORT_TIME);
        rgb_solid(colour2);
        delay(SHORT_TIME);
    }
}