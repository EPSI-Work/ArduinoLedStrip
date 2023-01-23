/*
* Christmas led strip 
* authors: HUON Nicolas, SORIN Aimeric, LECAT Baptiste, OLLIVIER Dimitri
* last-modification: 06/01/2023
*/

#include "FastLED.h"
#define NUM_LEDS 144
#define LED_PIN A3
#define BUTTON_ON_OFF_PIN A1
#define BUTTON_PROG_PIN A2
#define POTENTIOMETER_BRIGTHNESS_PIN A4
#define POTENTIOMETER_RATE_PIN A5

// define LEDs
CRGB leds[NUM_LEDS];

int buttonOnOffState = 1;
bool isLedsActive = false;
// current program running
int progNum = 0;
// number of programs
int progCount = 4;
// rate of the LED in ms
int ledRate = 1500;
// brightness of the LED
int ledBrightness = 20;

void setup() {
  // put your setup code here, to run once:

  // init button on/off
  pinMode(BUTTON_ON_OFF_PIN, INPUT);
  digitalWrite(BUTTON_ON_OFF_PIN, HIGH);

  // init button prog
  pinMode(BUTTON_PROG_PIN, INPUT);
  digitalWrite(BUTTON_PROG_PIN, HIGH);

  // init LEDs
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  LEDS.showColor(CRGB(0, 0, 0));
  LEDS.setBrightness(ledBrightness);
  Serial.begin(9600);
  Serial.println("Start here!");
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(BUTTON_ON_OFF_PIN) == LOW) {
    isLedsActive = !isLedsActive;
    Serial.println(isLedsActive);
    delay(100);
  }  

  if(digitalRead(BUTTON_PROG_PIN) == LOW) {
    progNum++;
    
    if(progNum == progCount + 1) {
      progNum = 1;
    }

    Serial.print("Program number: ");
    Serial.println(progNum);
    delay(100);
  }  

  if(isLedsActive) {
    switch (progNum) {
  case 1:
      firstProg();
    break;
  case 2:
      secondProg();
    break;
  case 3:
    thirdProg();
    break;
  case 4:
    fourthProg();
    break;
  default:
    // statements
    break;
}
    } else {
      reset();
    }


LEDS.setBrightness(analogRead(POTENTIOMETER_BRIGTHNESS_PIN) / 10);
ledRate = (analogRead(POTENTIOMETER_RATE_PIN) * 10); 
}

void reset() {
  LEDS.clearData();
  LEDS.showColor(CRGB(0, 0, 0));
}

void firstProg() {
  showLeds(1, 2, CRGB(188, 71, 73));
  delay(ledRate);
  showLeds(2, 2, CRGB(188, 71, 73));
  delay(ledRate);
}

void secondProg() {
  showLeds(1, 1, CRGB(188, 0, 73));
  delay(ledRate);
  showLeds(1, 1, CRGB(0, 0, 73));
  delay(ledRate);
}

void thirdProg() {
  showLeds(1, 1, CRGB(0, 0, 52));
  delay(ledRate);
  showLeds(1, 1, CRGB(0, 14, 73));
  delay(ledRate);
}

void fourthProg() {
  showLeds(1, 1, CRGB(247, 255, 88));
  delay(ledRate);
}

void showLeds(int startAt, int count, CRGB color) {
  reset();
  
  for(int n = startAt; n <+ NUM_LEDS; n= n + count) {
    leds[n] = color;
  }

  LEDS.show();
}
