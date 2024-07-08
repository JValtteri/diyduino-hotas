# Controller

A project to create a DIY HOTAS throttle controller from any Arduino or Arduino compatible board. The program can be used to create almost any kind of Arduino based controller.

If you have an Arduino Leonardo or Pro Micro, you don't need this project. These boards can act as input device, without the need for custon drivers.

## Description

The controller is the software flashed to the Arduino. It supports analog and digital inputs. It is designed to work with any Arduino board.

This program sends the inputs as serial messages over USB. A separate [driver](https://github.com/JValtteri/diyduino-hotas/blob/main/driver/README.md) is required on the PC to read the inputs and translate them in to generic HMI device inputs.

## Customize to your needs

The Arduino code has constants that you can modify to your needs

### Edit this portion to match your hardware

// The number of inputs of each type:
// Remember to update the input arrays with correct input pin IDs.

| Name | default | Description |
| -- | -- | -- |
| const int btns | 7 | Number of buttons |
| const int alogs | 2 | Number of analog inputs |
| const int btnPin[btns] | { 2, 3, 4, ... } | Array of button pins: Define the digital input pins here |
| const int analogPin[alogs] | { A0, A1 } | Array of analog pins: Define the analog input pins here |
| const int ledPin | 13 | the number of the LED pin (13 is the internal LED) |
| unsigned long debounceDelay | 10 | the debounce time; increase if the output flickers |
| const int analogDeadzone | 2 |

---

## Planned Extra features
- Add support for ilumination LEDs
  - Brightness control by PWM
