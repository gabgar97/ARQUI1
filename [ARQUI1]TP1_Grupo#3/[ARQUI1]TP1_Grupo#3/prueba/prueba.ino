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
 MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, maximo);
const int buttonPin1 = 6; 
const int buttonPin2 = 5; 
const int buttonPin3 = 4; 

#define PAUSE_TIME  1000

#define  DEBUG  0

#if  DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

typedef struct
{
  uint8_t spacing;  // character spacing
  const char *msg;  // message to display
} msgDef_t;

msgDef_t  M[] =
{
  { 1, "*" },
  { 0, "T" },
  { 1, "P" },
  { 0, "1" },
  { 1, "-" },
  { 0, "G" },
  { 1, "R" },
  { 0, "U" },
  { 1, "P" },
  { 0, "O" },
  { 1, "#" },
  { 0, "3" },
  { 1, "-" },
  { 0, "S" },
  { 1, "E" },
  { 0, "C" },
  { 1, "C" },
  { 0, "I" },
  { 1, "O" },
  { 0, "N" },
  { 1, "-" },
  { 0, "B" },
  { 1, "*" }
};
#define MAX_STRINGS  (sizeof(M)/sizeof(M[0]))

static uint8_t n = -1;

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  // Intialize the object:
  Serial.begin(57600);
  myDisplay.begin();
  P.begin();
  P.setCharSpacing(M[0].spacing);
  P.displayText(M[0].msg, PA_CENTER, myDisplay.getSpeed(), PAUSE_TIME, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(5);
  // Clear the display:
  myDisplay.displayClear();
  myDisplay.displayText("*TP1 - GRUPO 3 - SECCION B*", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop(void) {
  if(digitalRead(buttonPin3) == LOW){
    P.displayReset();
    n=1;
    delay(100);
    letrero();
    }
    else{
      myDisplay.displayReset();
      delay(100);
      cambio();
      }
}

void letrero(void){
  if (digitalRead(buttonPin1) == HIGH) {
    myDisplay.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    
    if(digitalRead(buttonPin2)== HIGH){
    int velocidad = 400;
    int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
    myDisplay.setSpeed(slide_scroll_speed);
    
    }else{
    int velocidad = 200;
    int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
    myDisplay.setSpeed(slide_scroll_speed);
      }
  
  }else{
    myDisplay.setTextEffect(PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
    if(digitalRead(buttonPin2)== HIGH){
    int velocidad = 400;
    int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
    myDisplay.setSpeed(slide_scroll_speed);
    
    }else{
    int velocidad = 200;
    int slide_scroll_speed = map(velocidad, 1023, 0, 400, 15);
    myDisplay.setSpeed(slide_scroll_speed);
      }
  }

  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
  }

void cambio(void){
   //static uint8_t n = 1;

  if (P.displayAnimate())
  {
    P.setTextBuffer(M[n].msg);
    P.setCharSpacing(M[n].spacing);
    P.displayReset();
    n = (n + 1) % MAX_STRINGS;
  }
  }
