# Audio level meter based on CH32V003J4M6

This is a simple audio level meter based on a very cheap and compact RISC V based microcontroller. There exist dedicated chips like LM3915, but they are old designs, probably not in production anymore and they are getting harder to find. They also tend to take a lot of board space.

This meter is a relatively flexible software-defined replacement. The circuit is almost as simple as the original. The meter has 20 LEDs. The chip only has 6 pins available for I/O, so the LEDs are charlieplexed.

Charlieplexing is a method of driving a high number of LEDs using few legs. The downside is that multiple LEDs can't be turned on at the same time and if you need to display a bar graph, like in this case, the multiplex factor is pretty high.

![video](/doc/levelmeter.gif)

Not a huge problem in this case though. The LEDs can even be driven using PWM for grayscale effect, but here grayscale is not very practical. So it's just on/off. There's a holding peak dot for more entertainment.

The meter is not calibrated to display any specific range of levels, the only thing I aimed for is for it to be entertaining and pretty to watch. There are parameters that can be adjusted in the code that change input gain, integration speed, decay speed and peak holding time. Only integer maths is used.

### Approximate specifications

| parameter | value | remark |
| -- | -- | -- |
| supply voltage | 5V or 3.3V |
| number of LEDs | 20 |
| refresh rate | approx. 2.2kHz | 241 cycles / conversion | 
| sampling frequency | 43kHz (estimated) | 241 cycles / conversion |
| refresh rate | approx. 5.1kHz | 73 cycles / conversion |
| sampling frequency | 102kHz (estimated) | 73 cycles / conversion |
| input amplitude | 1V<sub>p-p</sub> |

The numbers, especially refresh rate and sampling rate are approximate just to give the general ballpark figure.

The ADC is used in continuous sampling mode and the sampling rate is subject to jitter. There is room for improvement here, but it's some effort for very little return.
