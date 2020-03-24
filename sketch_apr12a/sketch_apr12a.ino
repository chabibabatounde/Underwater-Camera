#include <Servo.h>
#include <Wire.h>
#include "TSYS01.h"

byte relaisPin = 12;
byte servoPin = 9;
byte raspberryPin = 10;
byte luminositePin = A0;
int new_front = 0, old_front = 0;
Servo servo;
int boucle=-1;
long temps; // variable qui stocke la mesure du temps
int delai_eclairage = 2000;
int delai_mesure_luminosite = 10000;
unsigned long time_now = 0;
unsigned long time_now2 = 0;
int luminosite = 0;

    void setup() {
      Serial.begin(115200);
      servo.attach(servoPin);
      Wire.begin();
      // Bus I2C arduino UNO --> ‎A4 (SDA-->CABLE BLANC), A5 (SCL-->CABLE VERT)
      // 1100 --> OFF, 1900 --> Plein feux, 1700--> 75% 1500 50% d'éclairage soit 750 lumen, 1300 --> 25%
      servo.writeMicroseconds(1100); // envoie un signal "off" (éteint) au Lumen (lumière du tube)
      pinMode(relaisPin, OUTPUT);
      pinMode(raspberryPin, INPUT);
      digitalWrite(relaisPin, LOW);
      pinMode(luminositePin, INPUT);
    }

    void loop()
    {
      int signal = 1100;
      new_front = digitalRead(raspberryPin);
      unsigned long currentMillis = millis();
        if ((new_front==HIGH)) 
        {
          String serie="";
          luminosite=analogRead(luminositePin);
          signal = map(luminosite, 10, 1023, 1850, 1100);
          serie += String("{\"temperature\":0, \"luminosite\":"+String(luminosite)+", \"signal\":" + String(signal) + "}");
          Serial.println(serie);
          time_now = millis();
          Serial.println(serie);
          puissance_eclairage(signal,delai_eclairage);
        }
        servo.writeMicroseconds(1100); // Envoie le signal au lumen (fil jaune connecté
        digitalWrite(relaisPin, LOW);
    }
    
void puissance_eclairage(int signal,int delai_eclairage) {
while ((unsigned long)millis() - time_now < delai_eclairage)
      {
         servo.writeMicroseconds(signal); // Envoie le signal au lumen (fil jaune connecté)
         digitalWrite(relaisPin, HIGH);
     }
}

void eteint_eclairage(int signal,int delai_eclairage) {
while ((unsigned long)millis() - time_now < delai_eclairage) //(unsigned long)millis() < time_now + delai_eclairage
      {
         servo.writeMicroseconds(1100); // Envoie le signal au lumen (fil jaune connecté)
      }
}
void print_time(unsigned long time_millis){
    Serial.print("Time: ");
    Serial.print(time_millis/1000);
    Serial.print("s - ");
}
