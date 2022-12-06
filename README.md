# USB to PS2 on pico

## Good references
* https://github.com/Harvie/ps2dev/blob/master/src/ps2dev.cpp

### build steps
* mkdir build
* cd build
* cmake ..
* openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program USB_TO_PS2_PICO.elf verify reset exit"