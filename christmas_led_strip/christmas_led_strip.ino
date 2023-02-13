/*
* Christmas led strip 
* authors: HUON Nicolas, SORIN Aimeric, LECAT Baptiste, OLLIVIER Dimitri
* last-modification: 23/01/2023
*/

#include "FastLED.h"
#include "ArduinoBLE.h"
#define NUM_LEDS 144
#define LED_PIN A2
#define BUTTON_ON_OFF_PIN A5
#define BUTTON_PROG_PIN A6
#define POTENTIOMETER_BRIGTHNESS_PIN A3
#define POTENTIOMETER_RATE_PIN A4
#define BLUETOOTH_DEVICE_NAME "ViveLaBiere♥️"

BLEService colorsService("180a");  // creating the service
//BLEUnsignedCharCharacteristic randomReading("2A58", BLERead | BLENotify); // creating the readble only characteristic
BLEByteCharacteristic redChar("2a57", BLERead | BLEWrite);    // creating the red writable characteristic
BLEByteCharacteristic greenChar("2a58", BLERead | BLEWrite);  // creating the green writable characteristic
BLEByteCharacteristic blueChar("2a59", BLERead | BLEWrite);   // creating the blue writable characteristic

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

  // init bluetooth
  initBluetooth();

  Serial.begin(9600);
}

void initBluetooth() {
  //init ArduinoBLE library
  if (!BLE.begin()) {
    // catch if init fail
    Serial.println("starting Bluetooth® Low Energy failed!");
    while (1);
  }

  // setting a name that will appear when scanning for Bluetooth® devices
  BLE.setLocalName(BLUETOOTH_DEVICE_NAME);
  BLE.setAdvertisedService(colorsService);

  // add characteristics to a service
  colorsService.addCharacteristic(redChar);
  colorsService.addCharacteristic(greenChar);
  colorsService.addCharacteristic(blueChar);

  // add the service
  BLE.addService(colorsService);

  // set initial value for characteristics
  redChar.writeValue(255);
  greenChar.writeValue(255);
  blueChar.writeValue(255);

  // start advertising the service
  BLE.advertise();
  Serial.println(" Bluetooth® device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(BUTTON_ON_OFF_PIN) == LOW) {
    isLedsActive = !isLedsActive;
    Serial.println(isLedsActive);
    delay(100);
  }

  if (digitalRead(BUTTON_PROG_PIN) == LOW) {
    progNum++;

    if (progNum == progCount + 1) {
      progNum = 1;
    }

    Serial.print("Program number: ");
    Serial.println(progNum);
    delay(100);
  }

  if (isLedsActive && millis() >= 1000) {
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

  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();

  if (central) {  // if a central is connected to the peripheral
    Serial.print("Connected to central: ");
    Serial.println(central.address());
      
    while (central.connected()) {;
      if (redChar.written()) {
        Serial.println("red");
        Serial.print(redChar.value());
      }

      if (greenChar.written()) {
        Serial.println("green");
        Serial.print(greenChar.value());
      }

      if (blueChar.written()) {
        Serial.println("blue");
        Serial.print(blueChar.value());
      }
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

  //le delay a des effets de bord sur le BLE
  //delay(2000);
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

  for (int n = startAt; n < +NUM_LEDS; n = n + count) {
    leds[n] = color;
  }

  LEDS.show();
}
