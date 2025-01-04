// charlieplexing code based on a snippet by StuffAndyMakes
// https://gist.github.com/StuffAndyMakes/ffb6c61871289b0d7b73

#pragma once

#include <Arduino.h>

template<uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E>
struct Charlieplexor5
{
    static constexpr int PIN_COUNT = 5;
    static constexpr int LED_COUNT = 20;

    static constexpr int PIN_CONFIG = 0;
    static constexpr int PIN_STATE = 1;

    /*
     * If you want to save bytes in SRAM, this could be a collection of bytes for each DDRx and PORTx
     * on the AVR chip (up to 8 bits or lines) (8 lines would allow for 56 LEDs)
     */
    static constexpr int matrix[LED_COUNT][2][PIN_COUNT] = {
        /*
         *  Each row in this matrix respresents the pin modes and pin states for a single LED
         */
        //              PIN_CONFIG                          PIN_STATE
        //    A       B       C      D      E         A     B    C    D    E
        { { OUTPUT, OUTPUT, INPUT, INPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BA 1
        { { OUTPUT, OUTPUT, INPUT, INPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } },  // AB 0

        { { INPUT, OUTPUT, OUTPUT, INPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } },  // CB 3
        { { INPUT, OUTPUT, OUTPUT, INPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BC 2

        { { INPUT, INPUT, OUTPUT, OUTPUT, INPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DC 4
        { { INPUT, INPUT, OUTPUT, OUTPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } },  // CD 5

        { { INPUT, INPUT, INPUT, OUTPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // ED 7
        { { INPUT, INPUT, INPUT, OUTPUT, OUTPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DE 6

        { { OUTPUT, INPUT, OUTPUT, INPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } },  // CA 9
        { { OUTPUT, INPUT, OUTPUT, INPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } },  // AC 8

        { { INPUT, OUTPUT, INPUT, OUTPUT, INPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DB 13
        { { INPUT, OUTPUT, INPUT, OUTPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BD 12

        { { INPUT, INPUT, OUTPUT, INPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // EC 11
        { { INPUT, INPUT, OUTPUT, INPUT, OUTPUT }, { LOW, LOW, HIGH, LOW, LOW } },  // CE 10

        { { OUTPUT, INPUT, INPUT, OUTPUT, INPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DA 15
        { { OUTPUT, INPUT, INPUT, OUTPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } },  // AD 14

        { { INPUT, OUTPUT, INPUT, INPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // EB 17
        { { INPUT, OUTPUT, INPUT, INPUT, OUTPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BE 16

        { { OUTPUT, INPUT, INPUT, INPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // EA 19
        { { OUTPUT, INPUT, INPUT, INPUT, OUTPUT }, { HIGH, LOW, LOW, LOW, LOW } }   // AE 18
    };

    static void switchOn(int led)
    {
        // avoid leaks across branches
        switchOff();

        // set all the pin states
        digitalWrite(A, (PinStatus)matrix[led][PIN_STATE][0]);
        digitalWrite(B, (PinStatus)matrix[led][PIN_STATE][1]);
        digitalWrite(C, (PinStatus)matrix[led][PIN_STATE][2]);
        digitalWrite(D, (PinStatus)matrix[led][PIN_STATE][3]);
        digitalWrite(E, (PinStatus)matrix[led][PIN_STATE][4]);

        // set all the pin modes
        pinMode(A, (PinMode)matrix[led][PIN_CONFIG][0]);
        pinMode(B, (PinMode)matrix[led][PIN_CONFIG][1]);
        pinMode(C, (PinMode)matrix[led][PIN_CONFIG][2]);
        pinMode(D, (PinMode)matrix[led][PIN_CONFIG][3]);
        pinMode(E, (PinMode)matrix[led][PIN_CONFIG][4]);
    }

    static void switchOff()
    {
#ifdef FAST_SWITCHOFF
        // this doesn't respect pin parameters, but reduces parasitic lights
        GPIOD->OUTDR = 0;
        GPIOC->OUTDR = 0;
        GPIOD->CFGLR = 0x44444444; // all floating inputs
        GPIOC->CFGLR = 0x44444444; // all floating inputs
#else
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);

        pinMode(A, INPUT);
        pinMode(B, INPUT);
        pinMode(C, INPUT);
        pinMode(D, INPUT);
        pinMode(E, INPUT);
#endif
    }
};
