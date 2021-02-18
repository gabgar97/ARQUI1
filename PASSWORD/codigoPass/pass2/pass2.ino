#include<SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
// Servo de salida
Servo servoSalida;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
int posServo = 0;    // variable to store the servo position

bool aceptoPass;

bool ingresandoClave = false; // bandera que va a cambiar cuando el ultrasonico detecte para ingresar clave

const byte ROWS = 4; // 4 filas
const byte COLS = 3; // 3 columnas

const int trigPin = 13;  //Trig Ultrasons -> pin 13
const int echoPin = 12;  //Echo Ultrasons -> pin 12

const int trigPinSalida = 14;
const int echoPinSalida = 15;
const int ledSalida = 17;

long duracion, distancia, duracionSalida, distanciaSalida;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

bool abrePuerta = true;

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String acumuladorClave = "";  // variable que sirve para ir concatenando lo de la clave
String claveCorrecta = "20213";
int intentos = 0; // variable para contar los intentos de clave incorrecta

/*
  para el software serial es: tx/rx del virtual
*/


/*ROOM'S STRUCT*/
typedef struct {
  int num_room;
  bool state;
} Room;

#define NUM_ROOM 4

Room rooms[NUM_ROOM] = {
  {1, false},
  {2, false},
  {3, false},
  {4, false},
};

/*LIGHTS PIN*/


SoftwareSerial console(28, 27);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  console.begin(9600);
  pinMode(10, OUTPUT); // pin para los leds de alarma
  myservo.attach(9); // en el pin 9 esta el servo
  servoSalida.attach(16); // en el pin 9 esta el servo
  lcd.begin(16, 2);

  pinMode(trigPin, OUTPUT);  //Trig
  pinMode(echoPin, INPUT);   //Echo
  myservo.write(0);
  servoSalida.write(0);
}

void loop() {

  UltraSonico();

}

void pedirClave() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ingrese Clave:");
  lcd.setCursor(0, 1);
  lcd.print(acumuladorClave);
  char key = keypad.getKey();
  // console.println("tamos en clave");
  // lcd.print(acumuladorClave);
  if (key != NO_KEY) {
    // lcd.setCursor(0, 1);
    //lcd.print(key);
    if (key == '*') {
      // lcd.clear();

      console.println("La clave va: " + acumuladorClave);
      if (acumuladorClave == claveCorrecta) {
        console.println("CLAVE CORRECTA");
        lcd.clear();
        lcd.print("Bienvenido a ");
        lcd.setCursor(0, 1);
        lcd.print("Casa ^_^");

        // aqui mover servo
        myservo.write(180);
        delay(3000);
        myservo.write(0);
        delay(100);
        lights_state();
        // aqui la clave es correcta proceder a demas funciones aqui donde la clave es correcta

        

        /*
          duracionSalida = pulseIn(echoPin, HIGH);

          duracionSalida = duracionSalida * 340 / (2 * 10000);

          if (distanciaSalida <= 150) {
          digitalWrite(ledSalida, HIGH);
          delay(30);
          servoSalida.write(0);
          delay(3000);
          servoSalida.write(-180);
          delay(100);
          }
          else {
          digitalWrite(ledSalida, LOW);

          }
        */

      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Clave INCORRECTA!!!");
        intentos++;

        console.println("CLAVE INCORRECTA");
        console.print("intentos malos = ");
        console.println(intentos);

        if (intentos == 3) {
          activarAlarma();
        }
      }
      acumuladorClave = ""; // aqui limpiamos donde acumulamos la clave
    } else {
      console.println(key);
      acumuladorClave += key;
      delay(10);
    }

  }
}
void activarAlarma() {
  lcd.clear();
  lcd.print("ALARMA ACTIVADA!!");
  // encender leds
  int conteoAlarma = 0;

  for (int i = 0; i < 20; i++) {
    digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
    delay(500);              // wait for a second
  }

  intentos = 0;
}

void mensajeInicial() {
  //console.println("CASA INTELIGENTE ACE1");
  lcd.begin(16, 2);
  lcd.print("CASA INTELIGENTE ACE1");
  lcd.setCursor(0, 1);
  lcd.print("---------------");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ACE1-B-G3-S1 ");
  lcd.setCursor(0, 1);
  lcd.print("---------------");
  delay(1000);
  lcd.clear();

}


/*VERIFIES IF LIGHT IT'S TURNED ON*/
void lights_state() {
  delay(500);
  for ( int i = 0; i < NUM_ROOM; i++) {
    change_state(i);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Room: " + String(rooms[i].num_room));
    lcd.setCursor(0, 1);
    /*if (rooms[i].state == 0) {
      lcd.print("light: OFF");
    } else {
      lcd.print("light: ON");
    }*/

  lcd.print(rooms[i].state);

    delay(1000);
  }

UltraSonicoSalida();


}

void change_state(int index) {
  int ldr = analogRead(A6); //controller from room1
  int ldr2 = analogRead(A7);//controller from room2
  int ldr3 = analogRead(A8);//controller from room3
  int ldr4 = analogRead(A9);//controller from room4
  if (ldr > 10) {
    rooms[1].state = true;
  } else if (ldr < 10) {
    rooms[1].state = false;
  } else if (ldr2 > 10) {
    rooms[2].state = true;
  } else if (ldr2 < 10) {
    rooms[2].state = false;
  } else if (ldr3 > 10) {
    rooms[3].state = true;
  } else if (ldr3 < 10) {
    rooms[3].state = false;
  } else if (ldr4 > 10) {
    rooms[4].state = true;
  } else {
    rooms[4].state = false;
  }
}

void UltraSonico() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calcul de l'écho
  duracion = pulseIn(echoPin, HIGH);

  distancia = duracion * 340 / (2 * 10000);

  if (distancia <= 150) {
    //digitalWrite(Cerca,HIGH);
    //digitalWrite(Lejos,LOW);
    lcd.clear();
    pedirClave();
    Serial.println(distancia);
    Serial.println(" cm ");
    ingresandoClave = true;
  }
  else {
    //digitalWrite(Cerca,LOW);
    //digitalWrite(Lejos,HIGH);
    mensajeInicial();
    Serial.println(distancia);
    Serial.println(" cm ");
    ingresandoClave = false;
  }

  delay(500);
}

void UltraSonicoSalida() {

  digitalWrite(trigPinSalida, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSalida, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSalida, LOW);

  // Calculo del echo
  duracionSalida = pulseIn(echoPin, HIGH);

  duracionSalida = duracionSalida * 340 / (2 * 10000);

  if (distanciaSalida <= 150) {
    digitalWrite(ledSalida, HIGH);
    delay(30);

  }
  else {
    digitalWrite(ledSalida, LOW);
  }

  servoSalida.write(0);
  delay(3000);
  servoSalida.write(180);
  delay(500);

}
