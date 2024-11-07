# Pétomane ringard


A "simple" 3-voice "sound chip" with wavetables.

![preview](preview0.jpg)

3 channels are voiced using a 256-byte long 8-bit samples stored in SRAM. The mixing is done by time-multiplexing. The upper 8 bits select waveform. So you can have 256 different waveforms, or several amplitudes, or pre-filtered versions or even complete drump samples.
