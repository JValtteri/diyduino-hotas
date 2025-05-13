/*
  HOTAS

  The circuit:
  - LED attached from pin 13 to GND, with 220 ohm resistor
  - pushbutton attached from pin to GND
  - 22 kilohm resistor attached from pin to GND
  - potentiometer(s) have
    - 1 connected to +5V
    - 2 analog input pin
    - 3 connected to GND

  - MCP23017
    - SDL (pin 12) connected to A5 on Arduino
    - SDA (pin 13) connected to A4 on Arduino
    - VDD (pin 9) & RESET (pin 18) connected to +5V 
    - VSS (pin 10) & A0-A2 (pins 15-17)

  Created 16.06.2024
  Updated 13.05.2025

  Used resources:
  http://www.arduino.cc/en/Tutorial/Debounce
  https://docs.arduino.cc/built-in-examples/analog/AnalogInput/
*/

// ADDS MCP 23017 SUPPORT //
// Coumment out to remove

#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

//// START OF CONFIGURATION ////

// Edit this portion to match your hardware

const bool i2c = true;              // enable I2C if you have an MCP23X17 connected

// The number of inputs of each type:
// Remember to update the input arrays with correct input pin IDs.
const int btns = 11;                // Number of buttons
const int alogs = 1;                // Number of analog inputs
const int mcps = 16;

// I/O pin ID numbers:
const int btnPin[btns] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };     // Array of button pins: Define the digital input pins here
const int analogPin[alogs] = { A0 };    // Array of analog pins: Define the analog input pins here
const int ledPin = 13;              // the number of the LED pin (13 is the internal LED)
const int mcpBtnPin[mcps] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// Debounse
unsigned long debounceDelay = 10;   // the debounce time; increase if the output flickers

// Deadzone
const int analogDeadzone = 2;

//// END OF CONFIGURATION ////


// LED state information
int ledState = HIGH;             // the current state of the LED pin
int ledFlash = 2;                // length of LED flash

// Button state information
int btnValue[btns] = { };         // the current reading from the input pins
int btnPrevious[btns] = { };      // the previous reading from the input pins
int btnTempValue[btns] = { };     // temp buffer for values

// MCP Button state information
int mcpBtnValue[mcps] = { };         // the current reading from the input pins
int mcpBtnPrevious[mcps] = { };      // the previous reading from the input pins
int mcpBtnTempValue[mcps] = { };     // temp buffer for values

// Analog state information
int analogValue[alogs] = { };     // the current reading from the analog pins
int analogPrevious[alogs] = { };  // the previous reading from the input pins

// milliseconds, will quickly become a bigger number than can be stored in an int
unsigned long lastDebounceTime[btns] = { };  // the last time the output pin was toggled
unsigned long mcpLastDebounceTime[mcps] = { };  // the last time the output pin was toggled
unsigned long ledTime;  // the last time the output pin was toggled


// Sets up all buttons pins as inputs
void setupButtons() {
  for ( int i = 0; i < btns ; i++ ) {
    pinMode(btnPin[i], INPUT_PULLUP);  // INPUT or INPUT_PULLUP
  }
}


void setupMcpButtons() {
  for ( int i = 0; i < mcps ; i++ ) {
    mcp.pinMode(mcpBtnPin[i], INPUT_PULLUP);
  }
}


// Formats and sends a serial message
void sendValue(String name, int pin, int value) {
  // Format message
  String msg = name + String(pin) + ":" + value;
  Serial.println(msg);
}


// Sets up all buttons pins as inputs
void setupLEDs() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}


// Read analog inputs and update the array
void readAnalogs() {
  for ( int i = 0; i < alogs ; i++ ) {
    analogValue[i] = analogRead(i);
    if (abs(analogValue[i] - analogPrevious[i]) > analogDeadzone) {
      sendValue("A", i, analogValue[i]);
      analogPrevious[i] = analogValue[i];
    }
  }
}


// Sets the state of ilumination LEDs
void setLEDs(int state) {
  // Do PWM magic
  // set the LED:
  digitalWrite(ledPin, state);
}


// Does a signal LED flash
// true = turn on
// false = turn off if timeout
void flashLED(int state = LOW) {
  if (state == HIGH) {
    digitalWrite(ledPin, state);
    ledTime = millis();
  }
  else if ((millis() - ledTime) > ledFlash) {
    digitalWrite(ledPin, LOW);
  }
}


// Read the digital inputs and update the arrays
// uses debounse
void readBtn() {
  for ( int i = 0; i < btns ; i++ ) {
    btnTempValue[i] = digitalRead(btnPin[i]);

    // If a change is seen on input
    if (btnTempValue[i] != btnPrevious[i]) {
      // reset the debouncing timer
      lastDebounceTime[i] = millis();
    }

    // If the debounse timeout has passed
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (btnTempValue[i] != btnValue[i]) {
        // Set button value
        btnValue[i] = btnTempValue[i];

        // Send button event on serial
        sendValue("D", i, btnValue[i]);
        flashLED(HIGH);
      }
    }
    btnPrevious[i] = btnTempValue[i];
  }
}


// Reads digital inputs from MCP2017 and updates the arrays
// uses debounce
void readMCP() {
  for ( int i = 0; i < mcps; i++ ) {
    mcpBtnTempValue[i] = mcp.digitalRead(mcpBtnPin[i]);

    // If a change is seen on input
    if (mcpBtnTempValue[i] != mcpBtnPrevious[i]) {
      // reset the debouncing timer
      lastDebounceTime[i] = millis();
    }

    // If the debounse timeout has passed
    if ((millis() - mcpLastDebounceTime[i]) > debounceDelay) {

      if (mcpBtnTempValue[i] != mcpBtnValue[i]) {
        // Set button value
        mcpBtnValue[i] = mcpBtnTempValue[i];

        // Send button event on serial
        sendValue("MCP", i, mcpBtnValue[i]);
        flashLED(HIGH);
      }
    }
    mcpBtnPrevious[i] = mcpBtnTempValue[i];
  }
}


void setup() {
  setupButtons();
  setupLEDs();
  Serial.begin(9600);   // initialize serial comm at 9600 bits per second

  if (i2c) {
    if (!mcp.begin_I2C()) {
      Serial.println("Error.");
      while (1);
    }
    setupMcpButtons();
  }
}

void loop() {
  readAnalogs();
  readBtn();
  if (i2c) {
    readMCP();
  }
  flashLED();
}
