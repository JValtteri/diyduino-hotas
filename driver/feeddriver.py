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
ports = ['COM3', 'COM4', 'COM5', 'COM6']
rate = 9600
joy_multip = 32    # from 1024 to 0x8000 value scale
joy_select = 5     # +5 ignores first 5 axis names
digital_pins = 11  # number of digital pins used on arduino
mcp_size = 16      # number of MCP IO pins per mcp chip
first_mcp_id = 32  # I2C address id of first MCP chip
double_switches = [5,12,13,14,15,16,17,11,10,36,37]
combo_switches = [
    {
        "key": 7,
        "modifier":8,
    }
] # key works normally, but modifier is triggered only when both are pressed

#########################

# build switch state memory
combo_memory = dict()
for combo in combo_switches:
    combo_memory[combo["key"]] = 0
    combo_memory[combo["modifier"]] = 0

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

# NOTE! Undefined bahavior, if the "key" is depressed, when "modifier" changes
def combo_key(pin: int, value: int) -> tuple[int, int]:
    combo_memory[pin] = value
    for combo in combo_switches:
        if pin in [combo["key"], combo["modifier"]]:
            if combo_memory[combo["key"]] == combo_memory[combo["modifier"]] and combo_memory[combo["key"]] == 1:
                return combo["modifier"], 1
            elif combo_memory[combo["key"]] == 1:
                return combo["key"], 1
            elif combo_memory[combo["modifier"]] == 1:
                return combo["modifier"], 0
            else:
                return combo["key"], 0
    return -1, -1

def decode_event(event: str) -> tuple[int, int]:
    sevent = event.decode()
    id = sevent[0]
    if id == "#":
        return -1, -1
    sevent = sevent[1:] # Drop the first character
    sevent = sevent.strip('\r\n')
    if sevent == '':  # Ignore empty input and comments
        return -1, -1   # Using this value will result in a crash in vJoy
    try:
        name, value = sevent.split(':')
    except Exception as e:
        print(sevent.split(':'))
        raise e
    if id == "M":
        chip, pin = name.split('.')
        name = (int(chip) - first_mcp_id) * mcp_size + digital_pins + int(pin)# Make the button id run continuously through native and expansion pins

    return int(name)+1, int(value)  # name is offset by one: button index starts at 1 not zero

def button_update(name: int, value: int):
    if int(name) in double_switches:     # Simulate a short click
        j.set_button(name, 1)
        time.sleep(0.1)                  # Give time for click to register
        j.set_button(name, 0)
    elif int(name) in combo_memory:
        name, value = combo_key(int(name), int(value))
        j.set_button(int(name), value)
    else:
        j.set_button(name, value)

def joy_update(name: int, value: int):
    this_axis = axis[name+joy_select]                        # Maps input axis to virtual axis name
    j.set_axis(this_axis, (value*joy_multip))

def main():
    searching = True
    while searching:
        for port in ports:
            print(f"Waiting for device '{port}' to connect...")
            try:
                ser = serial.Serial(port, rate, timeout = 5)
                print(ser.name)
                searching = False
                break
            except serial.SerialException as e:
                pass
                time.sleep(0.1)

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
                    joy_update(name, int(value))
                elif line[0] == 68: # ASCII 'D'
                    print(f"Digital:{name} Value:{value}")
                    button_update(name, value)
                elif line[0] == 77: # ASCII 'M'
                    print(f"Digital:{name} Value:{value}, (mcp)")
                    button_update(name, value)

    except KeyboardInterrupt as e:
        ser.close()
        print("Connection closed")
    except serial.SerialException as e:
        ser.close()
        print("Connection closed by device")

main()
