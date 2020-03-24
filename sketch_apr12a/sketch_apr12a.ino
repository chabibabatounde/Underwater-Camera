#include <Servo.h>
#include <Wire.h>
#include "TSYS01.h"
TSYS01 sensor;

byte relaisPin = 12;
byte ctrLed = 13;
byte servoPin = 9;
byte raspberryPin = 10;
byte luminositePin = A0;
int new_front = 0;
Servo servo;
int delai_eclairage = 2000;
int delai_mesure_luminosite = 10000;
unsigned long time_now = 0;
int luminosite = 0;
//Reset Function
//void(* resetFunc) (void) = 0;

    void setup() {
      Serial.begin(115200);
      servo.attach(servoPin);
      Wire.begin();
      sensor.init();
      // Bus I2C arduino UNO --> ‎A4 (SDA-->CABLE BLANC), A5 (SCL-->CABLE VERT)
      // 1100 --> OFF, 1900 --> Plein feux, 1700--> 75% 1500 50% d'éclairage soit 750 lumen, 1300 --> 25%
      servo.writeMicroseconds(1100); // envoie un signal "off" (éteint) au Lumen (lumière du tube)
      pinMode(relaisPin, OUTPUT);
      pinMode(ctrLed, OUTPUT);
      pinMode(raspberryPin, INPUT);
      digitalWrite(relaisPin, LOW);
      digitalWrite(ctrLed, LOW);
      pinMode(luminositePin, INPUT);
    }

    void loop()
    {
      int signal = 1100;
      new_front = digitalRead(raspberryPin);
      unsigned long currentMillis = millis();
        if (new_front==HIGH) 
        {
          sensor.read();
          String serie="";
          luminosite=analogRead(luminositePin);
          if(luminosite>300){
            signal = 1100;
            digitalWrite(ctrLed, HIGH);
            while ((unsigned long)millis() - time_now < 2000){
              //On allume la led verte pendant 2 secondes}
            }
            digitalWrite(ctrLed, LOW);
          }else{
            signal = map(luminosite, 10, 1024, 1700, 1100);
            puissance_eclairage(signal,delai_eclairage);
          }
          serie += String("{\"temperature\":"+String(sensor.temperature())+", \"luminosite\":"+String(luminosite)+", \"signal\":" + String(signal) + "}");
          Serial.println(serie);
          time_now = millis();
          Serial.println(serie);
        }
        servo.writeMicroseconds(1100); // Envoie le signal au lumen (fil jaune connecté
        digitalWrite(relaisPin, LOW);
        digitalWrite(ctrLed, LOW);
        //Reset Function
//        resetFunc();
    }

    
    
void puissance_eclairage(int signal,int delai_eclairage) {
while ((unsigned long)millis() - time_now < delai_eclairage)
      {
         servo.writeMicroseconds(signal); // Envoie le signal au lumen (fil jaune connecté)
         digitalWrite(relaisPin, HIGH);
         digitalWrite(ctrLed, HIGH);
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
