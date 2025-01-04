# Notes about using CH32V003J4M6

## Essentials
  
 * Everywhere it's said that only WCH-LinkE will work with this micro. This could be the case in the past, but at the day of writing this (Jan 2025) this is not true. WCH-LinkW also works, even though minichlink.exe might report it as unknown programmer.
 * When the programming pin is also used as GPIO output, it may be impossible to reprogram the micro without removing it from the board.
 * To avoid finding yourself with a bricked MCU, always insert a significant delay in the initialisation code before making GPIO pin PD1/PD4/PD5/SWIO an OUTPUT.
 * It appears that the power provided by the WCH-LinkW dislikes bypass caps on my board. To unbrick a bricked MCU I have to remove it from the board and connect directly to the programmer, then it works.

## Frameworks

Before installing drivers compatible with the Arduino IDE, use the original `WCH-LinkUtility` to switch the programmer into RISCV mode. It ships in ARM mode by default. There are also other ways of switching it, they are documented elsewhere.

### Arduino IDE

The chip can be programmed from the vanilla Arduino IDE thanks to this package: https://github.com/AlexanderMandera/arduino-wch32v003 -- see installation instructions on that page.
minichlink.exe requires WinUSB drivers to be installed using Zadig. This will make WCH Utility not work anymore. You can reinstall the drivers back and forth if you need to use the official utility again.

### PlatformIO

The platform ch32v will not auto-install. It can be installed using this command. This also works on windows.
```
pio pkg install -g -p https://github.com/Community-PIO-CH32V/platform-ch32v.git
```
Check out the repo for examples. There is an Arduino framework, same as the one used in Arduino IDE as well as [CNLohr's ch32v003fun](https://github.com/cnlohr/ch32v003fun).
The main repo for ch32v003fun https://github.com/cnlohr/ch32v003fun

### Pinout
![ch32v003j4m6 pinout](https://raw.githubusercontent.com/Tengo10/pinout-overview/main/pinouts/CH32v003/ch32v003j4m6.svg)

### Floating point

It seems to be that it's enough to specify build_flags in `platformio.ini` like so:
```
build_flags = -lgcc -lm
```
However it results in unresolved reference to `__errno` from `sqrtf` or something else. There is probably a proper way of helping this, but I solved it by defining `int __errno` in the main source file.
