#include <SoftwareSerial.h>
#include <Servo.h>

#define WATER_SENSOR_PIN A0  // Water level sensor input


// LED Indicators
#define RED_LED 11    // 20% Water Level
#define YELLOW_LED 10 // 40% Water Level
#define BLUE_LED A1   // 60% Water Level
#define GREEN1_LED A2 // 80% Water Level
#define GREEN2_LED A3 // 100% Water Level

int percentage = 0;

SoftwareSerial espSerial(2, 3); // RX, TX (ESP32 TX -> D2, ESP32 RX <- D3)
Servo soilservo;  // create servo object to control a servo

void setup() {
    Serial.begin(115200);
    espSerial.begin(9600); //ESP Serial
    
    soilservo.attach(5);
    soilservo.write(0);

    pinMode(WATER_SENSOR_PIN, INPUT);
    
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN1_LED, OUTPUT);
    pinMode(GREEN2_LED, OUTPUT);
}

void loop() {
   checkWaterLevel();
   if (espSerial.available()) {
      String command = espSerial.readString();
      int cmd = command.toInt();

      if (cmd == 1) {
          soilservo.write(140);
      } else if (cmd == 2) {
          soilservo.write(0);
      }
   }  
}

void checkWaterLevel() {
    int waterLevel = analogRead(WATER_SENSOR_PIN);
    percentage = map(waterLevel, 0, 1023, 0, 140);
    digitalWrite(RED_LED, percentage >= 0 ? HIGH : LOW);
    digitalWrite(YELLOW_LED, percentage >= 20 ? HIGH : LOW);
    analogWrite(BLUE_LED, percentage >= 40 ? 180 : 0);
    digitalWrite(GREEN1_LED, percentage >= 60 ? HIGH : LOW);
    digitalWrite(GREEN2_LED, percentage >= 80 ? HIGH : LOW);
}
