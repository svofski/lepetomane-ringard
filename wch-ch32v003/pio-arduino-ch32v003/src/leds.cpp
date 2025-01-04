// CH32V003J4M6 audio level meter
//
// Uses 20 charlieplexed LEDs on all available pins.
//
// ch32v003j4m6
// pin#           D1 D2 D3 D4 D5 D6 D7 D8 D9 10 11 12 13 14 15 16 17 18 19 20
//  1 ----[470]---*--*--------------------*--*--------------*--*--------*--+
//                -  v                    |  |              |  |        |  |
//                ^  -                    |  |              |  |        |  |
//  8 ----[470]---*--*--*--*--------------|--|--*--*--------|--|--*--+  |  |
//                      -  v              -  v  |  |        |  |  |  |  |  |     
//                      ^  -              ^  -  |  |        -  v  |  |  -  v    
//  5 ----[470]---------*--*--*--*--------*--*--|--|--*--.  ^  -  |  |  ^  -
//                            -  v              -  v  |  |  |  |  -  v  |  |    
//                            ^  -              ^  -  |  |  |  |  ^  -  |  |   
//  6 ----[470]---------------*--*--*--*--------*--*--|--|--*--+  |  |  |  |
//                                  -  v              -  v        |  |  |  | 
//                                  ^  -              ^  -        |  |  |  |
//  7 ----[470]---------------------*--*--------------*--*--------*--*--*--+
// 
//
//  3 ---- audio in (center DC around 0.54V)

#include <Arduino.h>

#define RANDOM_STRENGTH    2
#define CH32V003           1

#include "ch32v003fun.h"
#include "polling_adc.h"
#include "cplex.h"
#include "bitmap.h"
#include "levelmeter.h"

// pin defines
#define A D6
#define B D4
#define C C1
#define D C2
#define E C4

#define LED_COUNT 20

int nloops = 40000000;  // brick prevention safety, all-off after some time

LevelMeter<GrayscaleBitmap<Charlieplexor5<A,B,C,D,E>, LED_COUNT>> levelmeter;

void setup() {
    delay(5000);  // initial delay to ensure that the chip always can be programmed

    polling_adc::init();
}

// for libm, if it's used
volatile int __errno;

void loop()
{
    for (int i = 0; i < LED_COUNT; ++i) {
        levelmeter.display_tick();
        levelmeter.sample(polling_adc::get());
    }
    levelmeter.update_bitmap();

    if (--nloops == 0) {
        levelmeter.off();
        for(;;);
    }
}
