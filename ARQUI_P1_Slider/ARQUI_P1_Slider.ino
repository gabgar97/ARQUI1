//Importaciones
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "LedControl.h"

//Codigo de control de matriz
//Configuracion de las matrices
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
int MAX_DEVICES =2;
int CLK_PIN   =13;
int DATA_PIN  =11;
int CS_PIN    =10;
int direc_pin =8;
int vel_pin=9;
int msm_pin=7;
int indice=0;
int s_juego=6;

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//Parametros Scrolling

uint8_t scrollSpeed = 30;    // velocidad
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;

uint16_t scrollPause = 1000; // en milisegundos

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "*TP1-GRUPO 3-SECCION B*" };
bool newMessageAvailable = true;
int  direccionAnterior=1;
int modoAnterior=1;
boolean modoJuego=false;
int tiempoJuego=300;
int  contador=0;



LedControl lc=LedControl(4,5,3,MAX_DEVICES);


void readSerial(void)
{
  
  static char *cp = newMessage;

  while (Serial.available()){
    *cp = (char)Serial.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2)){
      *cp = '\0'; // end the string
      
      cp = newMessage;
      newMessageAvailable = true;
    }
    else  // move char pointer to next position
      cp++;
  }
}
void cambioVelocidad(){
    if (digitalRead(vel_pin)==1){
    Serial.println("Cambio de Velocidad!!");
    if(scrollSpeed==30){
      scrollSpeed=100;
    }else{
      scrollSpeed=30;
    }
     P.setSpeed(scrollSpeed) ;
    delay(500);
  }

}

void cambioDireccion(){
  if (digitalRead(direc_pin)==1&&direccionAnterior==0){
    Serial.println("Cambio de direccion!!");
    direccionAnterior=1;
    scrollEffect = PA_SCROLL_RIGHT; 
    scrollAlign = PA_RIGHT;
    P.displayClear();// limpia las matrices y se reinicia la animación
    P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
    delay(200);
  } 
  if (digitalRead(direc_pin)==0&&direccionAnterior==1){
    Serial.println("Cambio de direccion!!");
    direccionAnterior=0;
    scrollEffect = PA_SCROLL_LEFT; 
    scrollAlign = PA_LEFT;
    P.displayClear();// limpia las matrices y se reinicia la animación
    P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
    delay(200);
  } 
  
}

void setup() {
    lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  Serial.begin(9600);
  Serial.println("Iniciando aplicacion con normalidad...\n");
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  pinMode(direc_pin,INPUT);
  pinMode(vel_pin,INPUT);
  pinMode(msm_pin,INPUT);
  pinMode(s_juego,INPUT);/**/

  


}

void loop() {


  if(digitalRead(s_juego)==1){
    contador++;
   // Serial.println(contador);
    if (contador>tiempoJuego){
        modoJuego=!modoJuego;
        if (modoJuego){
          Serial.println("Modo de juego activado");
          P.displayClear();
        }
    }
  }else{
    contador=0;  
  }
  if(modoJuego){
   lc.setLed(0,1,1,true);
    delay(500);
    lc.setLed(0,1,1,false); 

  }else{
   
    modoDisplay();
  }
}
void modoDisplay(){
  cambioVelocidad();
    if(digitalRead(msm_pin)==0&&modoAnterior==1){
        Serial.println("Modo scrolling");
        modoAnterior=0;
        P.displayClear();
        P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
    }
    if(digitalRead(msm_pin)==0&&modoAnterior==0){
        
        cambioDireccion();
        if (P.displayAnimate())
        {
          if (newMessageAvailable)
          {
            strcpy(curMessage, newMessage);
            newMessageAvailable = false;
          }
            P.displayReset();
        }
        readSerial();  
    }
    if(digitalRead(msm_pin)==1&&modoAnterior==0){
      Serial.println("Modo letra por letra");
      modoAnterior=1;
      P.displayClear();
    }
    if(digitalRead(msm_pin)==1&&modoAnterior==1){
      P.displayClear();
      P.setTextEffect(PA_NO_EFFECT ,PA_NO_EFFECT);
      P.setTextAlignment(PA_CENTER);
      char letra[5]={' ',' ',newMessage[indice],' ',' '};
      P.setTextBuffer(letra);
      P.displayAnimate();
      indice++;
      delay(scrollSpeed*5);
      P.displayReset();
      if (indice>28){
        indice=0;
      }
    }
}
