# DualSense4Windows
DualSense Support for any game based on emulation provided by [ViGEm](https://github.com/ViGEm/ViGEmClient).

## Features
Get the most out of your DualSense controller. 
Full support for any game using XInput.
Enhanced features like touchpad mouse controls.
Freely bind any action your controller is capable of to your desire.
* Creates a virtual DualShock4(Xbox Support WIP) from any connected DualSense.
* Lightbar Support
* Player LED Support
* Adaptive Trigger Support
* Rumble Support
* WIP:
  * Support for touchpad

## Installation
Just grab the latest release under [releases](https://github.com/broken-bytes/DualSense4Windows/releases) and run it.

You'll need to have [ViGEm Bus](https://github.com/ViGEm/ViGEmBus) installed.

## Building
### Prerequisites
You'll need to have the following components installed:
* CMake(tested with 3.14)
* Visual Studio 2019 (Any Edition)
* [ViGEm Bus](https://github.com/ViGEm/ViGEmBus)

You need to provide library files for the following components used
* [ViGEmClient](https://github.com/ViGEm/ViGEmClient)
* [hidapi](https://github.com/libusb/hidapi)
* [Ultralight](https://github.com/ultralight-ux/Ultralight)

### Build Steps
* First, create a dynamic library build for hidapi and ViGEmClient.
* Place `hidapi.lib`, `ViGEmClient.lib`, `AppCore.lib`, `UltraLight.lib`, `UltraLightCore.lib`, and `WebCore.lib` inside the `lib` folder of the project.
* Also place the corrosponding `.dll` files in the `bin` folder of your project root folder
* Execute `CreateVisualStudioSolution.bat`
* Open `DualSense4Windows.sln` created in your `build` folder
* Create a build
