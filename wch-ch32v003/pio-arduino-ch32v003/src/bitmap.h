#pragma once

#include <cstdint>

// A bitmap of grayscale lights with max possible 256 levels of brightness.
// Adjust phase_increment to a lower value to add more levels.
// Invoke loop() at regular intervals, as often as possible 
//
// If leds is Charlieplexor5, N is 20
template<typename leds, int N>
struct GrayscaleBitmap
{
    uint8_t values[N];  // brightness

    uint8_t phase;
    int index;
    int phase_increment;

    bool some_on;

    // gamma curve for smooth brightness control (gamma = 2.2)
    static constexpr uint8_t pwm_gamma22[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6,
        6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12,
        12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19,
        20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29,
        30, 30, 31, 32, 33, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41,
        42, 43, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
        73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 85, 87, 88, 89, 90,
        91, 93, 94, 95, 97, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 111,
        113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135,
        137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161,
        163, 165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186, 188, 190,
        192, 194, 196, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
        223, 225, 227, 229, 231, 234, 236, 238, 240, 242, 244, 246, 248, 251, 253, 255
    };
    

    GrayscaleBitmap()
        : values{}, phase(0), index(0), phase_increment(128), some_on(false)
    {
    }

    void loop()
    {
        if (phase < values[index]) {
            leds::switchOn(index);
            some_on = true;
        } else {
            if (some_on) {
                leds::switchOff();
                some_on = false;
            }
        }

        index += 1;
        if (index >= N) {
            index = 0;
            phase += phase_increment;
        }
    }

    void off() const
    {
        leds::switchOff();
    }

    static constexpr int led_count() { return N; }
};

