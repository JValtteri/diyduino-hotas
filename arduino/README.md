# Controller

A project to create a DIY HOTAS throttle controller from any Arduino or Arduino compatible board. The program can be used to create almost any kind of Arduino based controller.

If you have an Arduino Leonardo or Pro Micro, you don't need this project. These boards can act as input device, without the need for custon drivers.

## Description

The controller is the software flashed to the Arduino. It supports analog and digital inputs. It is designed to work with any Arduino board.

This program sends the inputs as serial messages over USB. A separate [driver](https://github.com/JValtteri/diyduino-hotas/blob/main/driver/README.md) is required on the PC to read the inputs and translate them in to generic HMI device inputs.

---

## Planned Extra features
- Add support for ilumination LEDs
  - Brightness control by PWM
