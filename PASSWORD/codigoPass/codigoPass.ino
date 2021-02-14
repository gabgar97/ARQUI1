#include<SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
int posServo = 0;    // variable to store the servo position

bool aceptoPass;

bool ingresandoClave = false; // bandera que va a cambiar cuando el ultrasonico detecte para ingresar clave

const byte ROWS = 4; // 4 filas
const byte COLS = 3; // 3 columnas

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

bool abrePuerta = true; 

byte rowPins[ROWS] = {2, 3, 4,5}; //connect to the row pinouts of the keypad
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

Room rooms[NUM_ROOM]={
  {1,false},
  {2,false},
  {3,false},
  {4,false},
};

/*LIGHTS PIN*/


SoftwareSerial console(28,27);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  console.begin(9600);
  pinMode(10, OUTPUT); // pin para los leds de alarma
  myservo.attach(9); // en el pin 9 esta el servo 
  lcd.begin(16, 2);

}

void loop() {
 
 /*LIGHTS CHANGE METHOD*/
 lights_state();

 
  /*while(ingresandoClave){  // ahorita no entra a este while estaba probando la clave primero pero si debe primero entrar aqui
    // aqui va el mensaje de inicio, ahorita empieza en pedir clave pero es de cambiar la bandera para que entre a este while
    // y con el ultrasonico cambiar la bandera para salir de este while y entrar a pedirClave()
    mensajeInicial(); // llamando a funcion con el mensaje inicial 
    // en la funcion mensajeInicial se podria ya se ahi mismo o llamar a otra funcion donde se trabaje con el ultrasonico para cambiar la bandera ingresandoClave
  }
  // llamar a metodo para pedir clave
  // lcd.clear();
  pedirClave();*/
  
}

void pedirClave(){
  
  char key = keypad.getKey();
  // console.println("tamos en clave");
  // lcd.print(acumuladorClave);
  if (key != NO_KEY){
    lcd.setCursor(0, 0);
    lcd.print("Ingrese clave: ");
    // lcd.setCursor(0, 1);
    //lcd.print(key);
    if(key == '*'){
      // lcd.clear();
            
      console.println("La clave va: " + acumuladorClave);
      if(acumuladorClave == claveCorrecta){
        console.println("CLAVE CORRECTA");
        lcd.print("Bienvenido a ");
        lcd.setCursor(0, 1);
        lcd.print("Casa ^_^");
        
        // aqui mover servo 
        myservo.write(180);
        // aqui la clave es correcta proceder a demas funciones aqui donde la clave es correcta 
        
        
      }else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Clave INCORRECTA!!!"); 
            intentos++;
            
            console.println("CLAVE INCORRECTA");
            console.print("intentos malos = ");
            console.println(intentos);
            
            if(intentos == 3){
              activarAlarma();
            }
        }
      acumuladorClave = ""; // aqui limpiamos donde acumulamos la clave
    }else{
      console.println(key);
      acumuladorClave += key;
      delay(10);
    }
    
  }  
}
void activarAlarma(){
  lcd.clear();
  lcd.print("ALARMA ACTIVADA!!");
  // encender leds
  int conteoAlarma = 0;

  for(int i = 0; i<20; i++){
    digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
    delay(500);              // wait for a second
  }
  
  intentos = 0;
}

void mensajeInicial(){
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
void lights_state(){
  
  
  for( int i=0; i < NUM_ROOM; i++){
    change_state(i);
    lcd.setCursor(0,0);
    lcd.print("Room: "+String(rooms[i].num_room));
    lcd.setCursor(0,1);
    lcd.print(rooms[i].state);
    delay(1000);
  }
  
  
  
  
}

void change_state(int index){
  int ldr= analogRead(A6);
  if(ldr > 10){
    rooms[1].state = true;
  }else {
    rooms[1].state = false;
  }
}
