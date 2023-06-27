/**
 * Sentient Lightform (a la Hal9000, K.I.T.T. etc.)
 * Copyright (c) 2023 Playful Technology 
 
 * Creates an animated glowing light whose brightness can pulse with a steady heartbeat of specified rate/intensity, 
 * or manually lit via a trigger, e.g. to accompany speech or to communicate "yes"/"no" responses
 */

// INCLUDES
// Use to calculate sine wave "heart beat" function. See FastLED.io
#include <FastLED.h>;

// CONSTANTS
// GPIO pin used for analog output
const byte outputPin = 26;
// GPIO pin used for digital input from, e.g. simple button/switch
const byte inputPin = 27;

// GLOBALS
// The intensity of the light output (0-255) 
uint8_t brightness = 0;
// Timestamp at which light intensity last updated
unsigned long previousMillis;
// delay (in ms) between updating the light
uint8_t updateInterval = 16;

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__ __DATE__);
  
  Serial.println(F("Configuring input"));
  pinMode(inputPin, INPUT_PULLUP);

  Serial.println(F("Configuring output"));
  pinMode(outputPin, OUTPUT);
  // Fadeup
  for(int val=0; val<255; val++){
    #ifdef ESP32
      dacWrite(outputPin, val);
    #else
      analogWrite(outputPin, val);
    #endif
    delay(10);
  }
  // Fadedown
  for(int val=255; val>0; val--){
    #ifdef ESP32
      dacWrite(outputPin, val);
    #else
      analogWrite(outputPin, val);
    #endif
    delay(10);
  }
}

void loop() {

  // Define a steady heartbeat pulse with specified (BPM, low intensity, high intensity)
  int targetBrightness = beatsin8(20, 195, 228);
  // Override brightness if button is being pressed
  if(!digitalRead(inputPin)){
    targetBrightness = 255;
  }

  // Grab the current timestamp
  unsigned long currentMillis = millis();

  // If sufficient time has elapsed since last update
  if(currentMillis - previousMillis >= updateInterval) {
    // Adjust the brightness to match the target brightness
    if(targetBrightness > brightness) { brightness++; }
    else if(targetBrightness < brightness) { brightness--; }
    // Send the updated value
    dacWrite(outputPin, brightness);
    previousMillis = currentMillis; 
  }
}