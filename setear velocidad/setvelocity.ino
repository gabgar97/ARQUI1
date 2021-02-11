
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define maximo 2
#define CS_PIN 10
#define DATA_PIN 11
#define CLK_PIN 13

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, maximo);
//MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, maximo);
const int buttonPin1 = 6;
const int buttonPin2 = 5;
const int buttonPin3 = 4;

int gameSpeed = 1;
char value = char(gameSpeed);
void setup() {

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  myDisplay.begin();
  myDisplay.setIntensity(5);
  myDisplay.displayClear();
  myDisplay.displayText("0 1", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
}

void loop() {

  if (digitalRead(buttonPin1) == HIGH) {
    increment();
    delay(350);
  }

  if (digitalRead(buttonPin2) == HIGH) {
    decrement();
    delay(350);
  }

  if (digitalRead(buttonPin3) == HIGH) {
    reset();
  }

  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}


void increment() {
  gameSpeed++;
  if (gameSpeed == 2) {
    myDisplay.displayText("0 2", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  }
  else if (gameSpeed == 3) {
    myDisplay.displayText("0 3", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  }
  else if (gameSpeed == 4) {
    myDisplay.displayText("0 4", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  } else {
    gameSpeed = 1;
  }
}

void decrement() {
  gameSpeed--;
  if (gameSpeed == 3) {
    myDisplay.displayText("0 3", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  } else if (gameSpeed == 2) {
    myDisplay.displayText("0 2", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  } else if (gameSpeed == 1) {
    myDisplay.displayText("0 1", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
  } else {
    gameSpeed = 1;
  }

}

void reset() {
  gameSpeed = 1;
  myDisplay.displayText("0 1", PA_CENTER, 100, 1, PA_PRINT, PA_PRINT);
}
