# DIYDUINO HOTAS

A project to create a DIY HOTAS throttle controller.

## Description

This project consists of two parts: [Controller](#controller) and the [driver](#driver).

## Controller

The controller with analog thrust levers and buttons. Designed to function with any Arduino board

## Driver

To make the device work with any Arduino board, a driver is required. The driver is just a simple program that needs to be running to translate the contoller inputs to controller axis and buttons.
The key mappings can be configured wiht a yaml-file. Behavior can also be adjusted: (switch/toggle).

