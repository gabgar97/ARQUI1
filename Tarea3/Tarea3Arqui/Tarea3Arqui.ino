int val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly
  val = analogRead (A1);
  float mv = (val/1024.0)*5000;
  float temp = mv/10;
  Serial.println(temp);
  delay(5000);
  Serial.flush();
}
