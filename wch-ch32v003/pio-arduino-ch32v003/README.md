# Audio level meter based on CH32V003J4M6

This is a simple audio level meter based on a very cheap and compact RISC V based microcontroller. There exist dedicated chips like LM3915, but they are old designs, probably not in production anymore and they are getting harder to find. They also tend to take a lot of board space.

This is a software-defined audio level meter. The circuit is almost as simple as LM3915, but provides more flexibility in terms of functionality as well as PCB layout. The bargraph display proper consists of 20 LEDs. The MCU chip only has 6 pins available for I/O, so the LEDs are charlieplexed.

Charlieplexing is a method of driving a high number of LEDs using few legs. The downside of this method is that multiple LEDs can't be turned on at the same time. In order to display a bar graph, the multiplexing factor is high. In this case 1:20.

![video](/doc/levelmeter.gif)

The LEDs can be grayscaled using PWM. In this application this effect is not practical so the LEDs are on/off only.

The meter is not calibrated to display any specific range of levels, the only thing I aimed for is for it to be entertaining and nice to watch. There are parameters that can be adjusted in the code, such as input gain, integration speed, decay speed and peak holding time. Only integer maths is used.

## Approximate specifications

The numbers, especially refresh rate and sampling rate are approximate just to give the general ballpark figure.

| parameter | value | remark |
| -- | -- | -- |
| supply voltage | 5V or 3.3V |
| number of LEDs | 20 |
| refresh rate | approx. 2.2kHz | 241 cycles / conversion | 
| sampling frequency | 43kHz (estimated) | 241 cycles / conversion |
| refresh rate | approx. 5.1kHz | 73 cycles / conversion |
| sampling frequency | 102kHz (estimated) | 73 cycles / conversion |
| input amplitude | 1V<sub>p-p</sub> |

The ADC is used in continuous sampling mode and the sampling rate is subject to jitter. 

ADC conversion time can be changed in platformio.ini:
```
	-DADC_CYCLES_SEL=7 ; 7: 241 cycles, 6: 73 cycles
```
There is also an artificial delay in the main loop. It has slight effect on perceived brightness and update speeds and can be adjusted to taste. 

## Schematics and layout

This is the complete schematics. There are 3 test pads used for connecting the programmer.
![schematics](/doc/levelmeter-sch.png)

Example layout on my PCB. Relevant parts are highlighted. The LEDs are 0402.
![layout](/doc/levelmeter-pcb.png)

## Building

The project builds using PlatformIO. See [CH32V003 notes](/CH32V003-notes.md) about setting up the environment.

## Things to improve

 * ADC work is crude.

