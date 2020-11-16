# DualSense4Windows
DualSense Support for any game based on emulation provided by [ViGEm](https://github.com/ViGEm/ViGEmClient).

## Features
* Creates a virtual DualShock4(Xbox Support WIP) from any connected DualSense.
* WIP:
  * Support for touchpad, lightbar, rumble and player LEDs
  * Adaptive trigger rumble based on sound 

## Installation
Just grab the latest release under github releases and run it.
You'll need to have [ViGEm Bus](https://github.com/ViGEm/ViGEmBus) installed.

## Building
### Prerequisites
CMake(tested with 3.14)
Visual Studio 2019 (Any Edition)
[ViGEm Bus](https://github.com/ViGEm/ViGEmBus) installed.
[ViGEmCLient](https://github.com/ViGEm/ViGEmClient)
[hidapi](https://github.com/libusb/hidapi)

### Build Steps
First, create a dynamic library build for hidapi and ViGEmClient.
Place `hidapi.lib` and `ViGEmClient.lib` inside the lib folder of the project.
Inside the root folder of the project run `CMake -G "Visual Studio 16 2019"`
Open the Visual Studio Solution created and create a new build
In order to run your new build, you need to provide `hidapi.dll` and `ViGEmClient.dll` at the location of the `DualSense4Windows.exe`.
