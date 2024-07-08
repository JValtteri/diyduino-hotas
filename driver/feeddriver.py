#!/usr/bin/python
#
# Feeder Driver
# Module to feed a virtual joystick with serial data from USB connected game controller
# JValtteri 8.6.2024

#import os, sys
import serial
import time

##### Configuration #####

#port = '/dev/ttyUSB0'
port = 'COM4'
rate = 9600

#########################



while True:
    try:
        ser = serial.Serial(port, rate, timeout = 5)
        print(ser.name)
        break
    except serial.SerialException as e:
        time.sleep(1)

# listen for the input, exit if nothing received in timeout period
print("Listening...")
try:
    while True:
        line = ser.readline()
        if len(line) != 0:
            print(line)
except KeyboardInterrupt as e:
    ser.close()
    print("Connection closed")
except serial.SerialException as e:
    ser.close()
    print("Connection closed by device")
