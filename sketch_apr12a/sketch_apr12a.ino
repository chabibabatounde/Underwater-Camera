#include <Servo.h>
#include <Wire.h>
#include "TSYS01.h"
TSYS01 sensor;

byte relaisPin = 12;
byte ctrLed = 13;
byte servoPin = 9;
byte raspberryPin = 10;
byte luminositePin = A0;
Servo servo;
int delai_eclairage = 2000;
int luminosite = 0;
int compteur = 0;
//Reset Function
//void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  Wire.begin();
  sensor.init();
  
  // Bus I2C arduino UNO --> ‎A4 (SDA-->CABLE BLANC), A5 (SCL-->CABLE VERT)
  // 1100 --> OFF, 1900 --> Plein feux, 1700--> 75% 1500 50% d'éclairage soit 750 lumen, 1300 --> 25%
  
  servo.writeMicroseconds(1100);
  
  // envoie un signal "off" (éteint) au Lumen (lumière du tube)
  pinMode(relaisPin, OUTPUT);
  pinMode(ctrLed, OUTPUT);
  pinMode(raspberryPin, INPUT);
  
  digitalWrite(relaisPin, LOW);
  digitalWrite(ctrLed, LOW);
  pinMode(luminositePin, INPUT);

}

void loop()
{
  compteur ++;
  if (digitalRead(raspberryPin)==HIGH){
    sensor.read();
    luminosite=analogRead(luminositePin);
    int signal = lumiere(delai_eclairage, luminosite);
    String serie = String("{\"temperature\":"+String(sensor.temperature())+", \"luminosite\":"+String(luminosite)+", \"signal\":" + String(signal) + "}");
    Serial.println(serie);
  }
  /*if(compteur>= 10){
    delay(500);
    resetFunc();
  }*/
}
  
int lumiere(int delai_eclairage, int luminosite) {
  int signal = map(luminosite, 10, 1024, 1700, 1100);
  digitalWrite(ctrLed, HIGH);
  if(luminosite<300){
    servo.writeMicroseconds(signal);
    digitalWrite(relaisPin, HIGH);   
  }
  delay(delai_eclairage);
  digitalWrite(relaisPin, LOW);
  digitalWrite(ctrLed, LOW);
  servo.writeMicroseconds(1100); 
  return signal;
}
