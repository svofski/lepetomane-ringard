#pragma once

// Not a VU-meter because I have no idea about the real watts or decibels.
// This is an entertainment device, so simply "LevelMeter".
template<typename bmp_t>
struct LevelMeter
{
    static constexpr int peak_hold = 750; // time to hold a peak value
    static constexpr int MIDDLE = 108;    // ADC value corresponding to zero DC
    static constexpr int GAIN = 400;      // input gain

    static constexpr int FIX1 = 1 << 14;                  // fixed point 1 = 16384
    static constexpr int k_discharge = 0.001 * FIX1;      // discharge rate, smaller is slower
    static constexpr int k_charge = 0.1 * FIX1;           // charge rate, larger is faster

    int peak = 0;         // current peak value
    int peak_decay = 0;   // peak hold counter
    int current = 0;      // current value

    bmp_t bmp;            // a LED bitmap



    // -60 dB scale
    // (trouble if LED_COUNT != 20)
    static constexpr int decibel60[bmp_t::led_count()] = {
        21,   //-57)
        30,   //-54)
        43,   //-51)
        61,   //-48)
        86,   //-45)
        122,  //-42)
        173,  //-39)
        245,  //-36)
        346,  //-33)
        489,  //-30)
        690,  //-27)
        975,  //-24)
        1378, //-21)
        1947, //-18)
        2750, //-15)
        3885, //-12)
        5488, //-9)
        7752, //-6)
        10950,//-3)
        15467 // 0)
    };
    
    // -40 dB scale, but the lower positions are "tuned"
    // (trouble if LED_COUNT != 20)
    static constexpr int decibel40[bmp_t::led_count()] = {
          194/16,//-38   ~ -60
          245/8,//-36    ~ -30
          308/4,//-34    ~ -45
          388/2,//-32    ~ -40
          489,//-30
          615,//-28
          775,//-26
          975,//-24
         1228,//-22
         1546,//-20
         1947,//-18
         2451,//-16
         3086,//-14
         3885,//-12
         4891,//-10
         6157,//-8
         7752,//-6
         9759,//-4
        12286,//-2
        15467 //0};
    };      


    // take in a new sample from ADC
    void sample(int adcval)
    {
        adcval = abs(adcval - MIDDLE) * GAIN; // offset and scale
        adcval = adcval * adcval / FIX1;      // square
        if (adcval > current) {               // charge (fast)
            current = current * (FIX1 - k_charge) + adcval * k_charge;
            current = current / FIX1;
        }
        else {                                // discharge (slow)
            current = current * (FIX1 - k_discharge);
            current = current / FIX1;
        }
    }

    // update bitmpa values
    void update_bitmap()
    {
        int vu = constrain(current, 0, FIX1);

        int newpeak = 0;
        for (int i = 0; i < bmp.led_count(); ++i) {
            bmp.values[i] = 0;
            if (vu >= decibel40[i]) {
                bmp.values[i] = 255;
                newpeak = i;
                if (i >= peak) {
                    peak = i;
                    peak_decay = peak_hold;
                }
            }
        }

        if (peak_decay) {
            --peak_decay;
            if (peak_decay == 0) {
                peak = newpeak;
                peak_decay = peak_hold;
            }
        }

        bmp.values[peak] = 128;
    }

    void display_tick()
    {
        bmp.loop();
    }

    void off()
    {
        for (int i = 0; i < bmp.led_count(); ++i) {
            bmp.values[i] = 0;
        }
        bmp.off();
    }
};

