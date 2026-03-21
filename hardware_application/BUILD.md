# Build guide for WDS_proj/hardware_application

## Linux

1. Install [build prerequisites](#build-prerequisites) on your system
2. `git clone https://github.com/maxksiazka/WDS_proj.git`
3. `cd WDS_proj/hardware_application/`
4. `mkdir build; cd build`
5. `cmake ..`
6. `make`
7. Copy the `sumo.uf2` binary to your Pico 2W in BOOTSEL mode (hold the BOOTSEL button while plugging it in).

**Notes:**
 - This build process does not rely on the Pico VSCode extension.
 - The build process relies on environment variable PICO_SDK_PATH - if you're using the VSCode extension for Pico, said variable needs to be set manually.
 - It is heavily advised to have a somewhat recent version of arm-none-eabi-gcc.
 - It also needs the [picotool](https://github.com/raspberrypi/picotool) binary to be in PATH.
 - The TCP server necessary for connecting to the Pico is NOT provided.

## Windows

**Unsupported** - as in, I have no clue,
*I use arch, btw* - and everybody laughed,

## Build prerequisites

- arm-none-eabi-gcc
- [pico_sdk](https://github.com/raspberrypi/pico-sdk)
- [picotool](https://github.com/raspberrypi/picotool)
