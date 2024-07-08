# DIYDUINO HOTAS

A project to create a DIY HOTAS throttle controller from any Arduino or Arduino compatible board. The program can be used to create almost any kind of Arduino based controller.

If you have an Arduino Leonardo or Pro Micro, you don't need this project. These boards can act as input device, without the need for custon drivers.

## Description

This project consists of two parts: [Controller](arduino/README.md) and the [driver](driver/README.md).
The controller is the software flashed to the Arduino, while the driver is used to communicate with a virtual controller driver [vJoy](https://github.com/shauleiz/vJoy).

## Requirements

- Microsoft Windows 7, 8 or 10 (Linux support is planned, but requires a different driver)
- Any Arduino board
