/*

  HOTAS

  The circuit:
  - LED attached from pin 13 to ground, with 220 ohm resistor
  - pushbutton attached from pin to +5V
  - 22 kilohm resistor attached from pin to ground
  - potentiometer(s) have
    - 1 connected to +5V
    - 2    ''        GND
    - 3 analog input pin

  Created 16.06.2024

  Used resources:
  http://www.arduino.cc/en/Tutorial/Debounce
  https://docs.arduino.cc/built-in-examples/analog/AnalogInput/
*/



//// START OF CONFIGURATION ////

// Edit this portion to match your hardware

// The number of inputs of each type:
// Remember to update the input arrays with correct input pin IDs.
const int btns = 7;                 // Number of buttons
const int alogs = 1;                // Number of analog inputs

// I/O pin ID numbers:
const int btnPin[btns] = { 2, 3, 4, 5, 6, 7, 8 };     // Array of button pins: Define the digital input pins here
const int analogPin[alogs] = { A0 };    // Array of analog pins: Define the analog input pins here
const int ledPin = 13;              // the number of the LED pin (13 is the internal LED)

// Debounse
unsigned long debounceDelay = 10;   // the debounce time; increase if the output flickers

// Deadzone
const int analogDeadzone = 2;

//// END OF CONFIGURATION ////



// LED state information
int ledState = HIGH;              // the current state of the output pins
int ledFlash = 20;                // length of LED flash

// Button state information
int btnValue[btns] = { };         // the current reading from the input pins
int btnPrevious[btns] = { };      // the previous reading from the input pins
int btnTempValue[btns] = { };     // temp buffer for values

// Analog state information
int analogValue[alogs] = { };     // the current reading from the analog pins
int analogPrevious[alogs] = { };  // the previous reading from the input pins

// milliseconds, will quickly become a bigger number than can be stored in an int
unsigned long lastDebounceTime[btns] = { };  // the last time the output pin was toggled
unsigned long ledTime;  // the last time the output pin was toggled


// Sets up all buttons pins as inputs
void setupButtons() {
  for ( int i = 0; i < btns ; i++ ) {
    pinMode(btnPin[i], INPUT);
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
      sendValue("analog", i, analogValue[i]);
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
        sendValue("btn", i, btnValue[i]);
        flashLED(HIGH);
      }
    }
    btnPrevious[i] = btnTempValue[i];
  }
}


void setup() {

  setupButtons();
  setupLEDs();
  Serial.begin(9600);   // initialize serial comm at 9600 bits per second
}

void loop() {
  readAnalogs();
  readBtn();
  flashLED();
}
