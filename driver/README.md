# Driver

To make the device work with any Arduino board, a driver is required. vJoy is used as a virtual joystick driver, while this program is used to read the arduino serial communication and feed the virtual joystick with data.

## Setup

### Install vJoy
[Download from SourceForge](https://sourceforge.net/projects/vjoystick/) ([source](https://github.com/shauleiz/vJoy))

Install at least
- the configuration tool and
- the monitor (useful for troubleshooting)

### Create a virtual joystick

- Numberd tabs correspond to virtual device IDs.
- Put a check mark on each axis you need and the number of buttons you have.
*Arduino axis are assigned automatically in order. It is recommend you start with **X** and check in order as many axis as you need.*
- Disable force feedback
- Click apply

### Make sure you have an up to date Python version 3.6.7 or newer.

[Download Python](https://www.python.org/downloads/) (Windows)

### Install requirements
```pip install -r requirements.txt```

### Configure feeddriver

- Open feeddriver.py in an editor.
- `virtual_device_no` should correspond with the tab number in vJoyConf program.
- `port` should match the USB port you connect your arduino to. It's usually `COM3` or later.
- `rate` leave at default, unless you know what you're doing.
- `joy_multip` (default 32) is used to scale the input to the joystick.
  - Arduino by default uses 10 bit ADC, giving 1024 increments for axis. To scale this to vJoy, we multiply it by 32 to get 32768 (0x8000).
- `joy_select` for joysticks should be zero. Axis will be named starting from X. The number corresponds to how many axis names are ignored, starting from X. For two engine HOTAS we could use 5, to get axis named as SL0 and SL1

### Verify axis are working

- Connect your Arduino
- Start feeddriver.py
- Start vJoy Monitor
- Check each axis and button to see they work

## Run

- Connect your arduino
- Start feeddriver.py

## Acknowledgements/resources

[willwade/pyvjoy](https://github.com/willwade/pyvjoy) was a great resource for understanding how to use the original library[tidzo/pyvjoy](https://github.com/tidzo/pyvjoy)
[vJoy](https://sourceforge.net/projects/vjoystick/) is creates a virtual joystick that is recognized by Windows
