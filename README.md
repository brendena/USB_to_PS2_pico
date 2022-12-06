# PS2_Device

## Good references
* https://github.com/Harvie/ps2dev/blob/master/src/ps2dev.cpp

### build steps
* mkdir build
* cd build
* cmake ..
* openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program RaspberryShifter.elf verify reset exit"