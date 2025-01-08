# Raspberry Pi Pico Firmware

Nothing of substance here yet.

# Building

```
mkdir build && cd build && cmake ..
ninja
...
picotool load ringard.bin
```

`picotool reboot -u -f` will not work because reset interface is not implemented, however a 1200 baud hack works. 

On Windows invoke `mode com30 1200` to put pico back into bootloader mode. 

Sometimes USB device would enter some indeterminate state. Use `USBTreeView.exe` to locate the failed hub and invoke `Restart Device` on it.

# Breadcrumbs

### USB / MIDI etc

 * tinymidi example and infovore's [pico-example-midi](https://github.com/infovore/pico-example-midi)
 * Paul Fortescue's [Miscellaneous](https://github.com/PaulFortescue/Miscellaneous)
 * [MIDI as part of a Composite Device #2158](https://github.com/hathach/tinyusb/issues/2158)