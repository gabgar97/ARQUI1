/* Basic example code for MAX7219 LED dot matrix display with Arduino. More info: https://www.makerguides.com */
// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define maximo 2
#define CS_PIN 10
// Create a new instance of the MD_Parola class with hardware SPI connection:
//MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, maximo);
// Setup for software SPI:
#define DATA_PIN 11
#define CLK_PIN 13
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, maximo);
const int buttonPin1 = 6;
const int buttonPin2 = 5;

void setup() {
  pinMode(buttonPin1, INPUT);
  // Intialize the object:
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(5);
  // Clear the display:
  myDisplay.displayClear();
  myDisplay.displayText("*TP1 - GRUPO 3 - SECCION B*", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {

  if (digitalRead(buttonPin1) == HIGH) {
    myDisplay.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);

    if (digitalRead(buttonPin2) == HIGH) {
      int velocidad = 400;
      int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
      myDisplay.setSpeed(slide_scroll_speed);

    } else {
      int velocidad = 200;
      int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
      myDisplay.setSpeed(slide_scroll_speed);
    }

  } else {
    myDisplay.setTextEffect(PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
    if (digitalRead(buttonPin2) == HIGH) {
      int velocidad = 400;
      int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
      myDisplay.setSpeed(slide_scroll_speed);

    } else {
      int velocidad = 200;
      int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
      myDisplay.setSpeed(slide_scroll_speed);
    }
  }

  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}
