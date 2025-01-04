# Audio level meter based on CH32V003J4M6

This is a simple audio level meter based on a very cheap and compact RISC V based microcontroller. There exist dedicated chips like LM3915, but they are old designs, probably not in production anymore and they are getting harder to find. They also tend to take a lot of board space.

This meter is a relatively flexible software-defined replacement. The circuit is almost as simple as the original. The meter has 20 LEDs. The chip only has 6 pins available for I/O, so the LEDs are charlieplexed.

Charlieplexing is a method of driving very high number of LEDs using few legs. The downside is that multiple LEDs can't be turned on at the same time and if you need to display a bar graph, like in this case, the multiplex factor is pretty high.

![video](/doc/levelmeter.gif)