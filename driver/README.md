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

| Variable | Default | Description |
| :--: | :--: | -- |
| `virtual_device_no` | | correspond with the virtual device (tab number) in [**vJoyConf**](https://sourceforge.net/projects/vjoystick/) |
| `ports` |  | List of `COM` ports to try, when connecting to Arduino. Try adding ports, if you are unable to connect |
| `rate` | `9600` | leave at default, unless you know what you're doing. |
| `joy_multip` | `32`[^ADC] | is used to scale the input to the joystick. |
| `joy_select` |  | for joysticks should be zero. Axis will be named starting from X. The number corresponds to how many `axis` names are ignored, starting from X. For two engine HOTAS we could use `5`, to get axis named as SL0 and SL1 |
| `digital_pins` | `11` | The total number of digital IO pins used on Arduino itself. This is used to set virtual button numbers correctly. A too small number will cause issues. |
| `mcp_size` | `16` | Must match the number of IO pins on the used MCP chip. This is used to set the virtual button numbers correctly. For MCP23017, the number is 16 |
| `first_mcp_id` | `32` | I2C address if of the first MCP chip. Used to parse the Arduino input events |
| `double_switches` | | List of virtual button IDs numbers, whose switch should trigger an event on both rising and falling edge (flip switch) |
| `combo_switches` | | List of two button chords where the `key` button works normally, but if pressed while `modifier` is depressed, the evemt is `modifier` instead of `key` |


[^ADC]: Arduino by default uses 10 bit ADC, giving 1024 increments for axis. To scale this to vJoy, we multiply it by 32 to get 32768 (0x8000).

### Verify axis are working

- Connect your Arduino
- Start `feeddriver.py`
- Start `vJoy Monitor`
- Check each axis and button to see they work

###### Note:
You may need to run feeddriver explicitly with 
```python feeddriver.py```
or
```python3 feeddriver.py```
depending on what [Python version](#make-sure-you-have-an-up-to-date-python-version-367-or-newer) is the default on your system

## Run

- Connect your arduino
- Start `feeddriver.py`

## Acknowledgements/resources

[willwade/pyvjoy](https://github.com/willwade/pyvjoy) was a great resource for understanding how to use the original library[tidzo/pyvjoy](https://github.com/tidzo/pyvjoy)
[vJoy](https://sourceforge.net/projects/vjoystick/) is creates a virtual joystick that is recognized by Windows
