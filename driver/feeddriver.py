#!/usr/bin/python
#
# Feeder Driver
# Module to feed a virtual joystick with serial data from USB connected game controller
# JValtteri 8.6.2024

#import os, sys
import serial
import time
import pyvjoy

##### Configuration #####

virtual_device_no = 1 # Corresponding vJoy device ID
#port = '/dev/ttyUSB0'
port = 'COM4'
rate = 9600
joy_multip = 32 # from 1024 to 0x8000 value scale
joy_select = 5 # +5 ignores first 5 axis names

#########################


axis = [
    pyvjoy.HID_USAGE_X,
    pyvjoy.HID_USAGE_Y,
    pyvjoy.HID_USAGE_Z,
    pyvjoy.HID_USAGE_RX,
    pyvjoy.HID_USAGE_RY,
    pyvjoy.HID_USAGE_RZ,
    pyvjoy.HID_USAGE_SL0,
    pyvjoy.HID_USAGE_SL1,
    pyvjoy.HID_USAGE_WHL,
    pyvjoy.HID_USAGE_POV
    ]


j = pyvjoy.VJoyDevice(virtual_device_no)

def decode_event(event):
    sevent = event.decode()
    sevent = sevent[1:] # Drop the first character
    sevent = sevent.strip('\r\n')
    if sevent == '':  # Ignore empty input
        return -1, -1   # Using this value will result in a crash in vJoy
    try:
        name, value = sevent.split(':')
    except Exception as e:
        print(sevent.split(':'))
        raise e
    return int(name)+1, int(value)  # name is offset by one: button index starts at 1 not zero

def button_update(name, value):
    j.set_button(name, value)

def joy_update(name, value):
    this_axis = axis[name+joy_select]                        # Maps input axis to virtual axis name
    j.set_axis(this_axis, (value*joy_multip))

def main():
    print(f"Waiting for device '{port}' to connect...")
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
                #print(line)
                name, value = decode_event(line)
                if line[0] == 65: # ASCII 'A'
                    print(f"Analog:{name} Value:{value}")
                    joy_update(name, value)
                elif line[0] == 68: # ASCII 'D'
                    print(f"Digital:{name} Value:{value}")
                    button_update(name, value)

    except KeyboardInterrupt as e:
        ser.close()
        print("Connection closed")
    except serial.SerialException as e:
        ser.close()
        print("Connection closed by device")

main()
