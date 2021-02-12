#include <LedControl.h>
//Matrix
const byte DIN = 3;
const byte CLK = 4;
const byte CS = 2;
LedControl lc = LedControl(DIN, CLK, CS, 2); // 1 = počet řadičů
 
//Botones
const byte btnRight = 15; // right 
const byte btnLeft = 14; // left 
const byte btnUp = 16;// up
const byte btnDown = 17; // down 
 
//Proměnné
int serpiente[8][16] = {0};
 
byte xserpiente = 4;
byte yserpiente = 5;
byte xcomida = 0;
byte ycomida = 0;
 
byte direccion = 1;
int paso = 1;
long cambio = 0;
int velocidad = 500;
byte punteo =1;
int matriz=0;
boolean snezeno = false;
boolean sonido = true;
boolean pos = false;
  
//Deklarování čísel
int Cislo[11][7] = {
  {B00000000,B00000000,B00000000,B00000000,B00000001,B01111111,B00100001}, //1
  {B00000000,B00000000,B00110001,B01001001,B01000101,B00100011},//2
  {B00000000,B00000000,B01000110,B01101001,B01010001,B01000001,B01000010},//3
  {B00000000,B00001000,B11111111,B01001000,B00101000,B00011000,B00001000},//4
  {B00000000,B00000000,B00111110,B01000101,B01001001,B01010001,B00111110}, //5
  {B00000000,B00000000,B00000110,B01001001,B01001001,B00101001,B00011110}, //6
  {B00000000,B00000000,B01110000,B01001000,B01000111,B01000000,B01100000}, //7
  {B00000000,B00000000,B00110110,B01001001,B01001001,B01001001,B00110110}, //8
  {B00000000,B00000000,B00111100,B01001010,B01001001,B01001001,B00110000}, //9
  {B00000000,B00000000,B00111110,B01010001,B01001001,B01000101,B00111110}, //0
  {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000}, //mezera
  };
 
void setup() {
  Serial.begin(9600);
  Serial.println("Comienza");
 
  randomSeed(analogRead(A0));
  GeneraComida();
 
  pinMode(btnLeft, INPUT);
  pinMode(btnRight, INPUT);
  pinMode(btnUp, INPUT);
  pinMode(btnDown, INPUT);
 
 
  //Cargar Pantalla
  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  lc.clearDisplay(0);
  
  lc.shutdown(1, false);
  lc.setIntensity(1, 4);
  lc.clearDisplay(1);
}
 
void loop() {
  if ((digitalRead(btnLeft) == true)){ direccion = 0;}
  if ((digitalRead(btnRight) == true) ) {direccion = 1;}
  if ((digitalRead(btnUp) == true) ){ direccion = 2;}
  if ((digitalRead(btnDown) == true) ) {direccion = 3;}
 
  if ((millis() - cambio) > velocidad) {
    cambio = millis();
 
    LimpiarSerpiente(serpiente);
    Punteo();
    Movimiento();
 
    if (xserpiente == xcomida && yserpiente == ycomida) snezeno = true;
    if (serpiente[xserpiente][yserpiente] != 0) GameOver();
 
    paso++;
    serpiente[xserpiente][yserpiente] = paso;
  }
  Dibujo(serpiente, xcomida, ycomida);
}
///////////////////////////////////////////////////////////////////
// Funkce
void Punteo() {
  if (snezeno == true) {
    snezeno = false;
    GeneraComida();
 
    punteo++;
    
    velocidad -= 20;
 
    Serial.print ("punteo je: ");
    Serial.println (punteo);
  }
}
 
void Dibujo(int matice[8][16], byte x, byte y) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      if ((matice[i][j] >= 1) || ((i == x) && (j == y) )) {
        if(j>=8){
          matriz=1;
          lc.setLed(matriz, i, j-8, true);
        }else{
          matriz=0;
          lc.setLed(matriz, i, j, true);  
        }
      }else{ 
        if(j>=8){
          matriz=1;
          lc.setLed(matriz, i, j-8, false);
        }else{
          matriz=0;
          lc.setLed(matriz, i, j, false);  
        }
      }
    }
  }
}
 
void LimpiarSerpiente(int matice[8][16]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      if (matice[i][j] == (paso - punteo)) matice[i][j] = 0;
    }
  }
}
 
void Movimiento() {
  switch (direccion) {
    case 0:
      xserpiente++;
      break;
    case 1:
      xserpiente--;
      break;
    case 2:
      yserpiente++;
      break;
    case 3:
      yserpiente--;
      break;
  }
  if (xserpiente == 8) {GameOver();}
  if (yserpiente == 16) {GameOver();}
  if (xserpiente == 255) {GameOver();}
  if (yserpiente == 255){GameOver();}
}
 
void GeneraComida() {
  lc.setLed(0, xcomida, ycomida, false);
  do {
    xcomida = random(0, 8);
    ycomida = random(0, 16);
  } while (serpiente[xcomida][ycomida] != 0 );
}
 
void DibujoCislo(byte cislo){
  if(cislo ==0) cislo = 10;
    for (int col=0; col<7; col++) {
      lc.setRow(0, col, Cislo[cislo-1][col]);    
  }
}
 
void GameOver() {
  Serial.println ("Game over");
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (sonido == true) {
        
        delay(1000);
      
      }
      lc.setLed(0,col,row, true);
      delay(25);
    }
    sonido = false;
  }
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 16; col++) {
      lc.setLed(0, col, row, false); 
      delay(25);
    }
  }
  do {
       DibujoCislo((punteo/10)%10);
  delay(200);
  DibujoCislo(punteo%10);
  delay(200);
  DibujoCislo(11);
  delay(1000);
  } while (true);
}
