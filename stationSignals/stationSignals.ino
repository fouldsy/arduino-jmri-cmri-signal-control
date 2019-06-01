// Add includes for CMRI (https://github.com/madleech/ArduinoCMRI)
// and AutoRS485 (https://github.com/madleech/Auto485)
// Make sure these are installed in your Arduino library
#include <CMRI.h>
#include <Auto485.h>

// Intialize communications for CMRI
// Defaults to a CMRI node at address 1 with 24 inputs and 48 outputs
#define CMRI_ADDR 1
#define DE_PIN 2
Auto485 bus(DE_PIN);
CMRI cmri(CMRI_ADDR, 24, 48, bus);

// Configure shift register communication
// Set these to the pins in use on your own Arduino
const int clockPin = 8; // yellow wire
const int latchPin = 9; // green wire
const int dataPin = 10; // blue wire

void setup() {
  Serial.begin(9600, SERIAL_8N2);

  // Set the shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Start a CMRI process
  cmri.process();
 
  // Set to latch to low before shifting out the data
  digitalWrite(latchPin, LOW);

  // Get the data from CMRI, then shift out the data to each register
  // The registers are shifted out in reverse - that is, the fourth
  // register (3) gets shifted out, then the third register (2), etc.
  //
  // This approach pushes the data out to each register as it goes
  // Update these lines for however many registers you actually have
  shiftOut(dataPin, clockPin, MSBFIRST, cmri.get_byte(3));
  shiftOut(dataPin, clockPin, MSBFIRST, cmri.get_byte(2));
  shiftOut(dataPin, clockPin, MSBFIRST, cmri.get_byte(1));
  shiftOut(dataPin, clockPin, MSBFIRST, cmri.get_byte(0));

  // Once all the data has been shifted out, set the latch to high to finalize
  digitalWrite(latchPin, HIGH);
}
