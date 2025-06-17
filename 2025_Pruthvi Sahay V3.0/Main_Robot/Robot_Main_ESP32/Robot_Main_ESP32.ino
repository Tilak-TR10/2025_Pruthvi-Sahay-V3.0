#include <WiFi.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include "DFRobotDFPlayerMini.h"


// WiFi Credentials
const char *ssid = "Pruthvi_Sahay_Rover";
const char *password = "12345678";

// Motor Control Pins
#define RPWM1 16  
#define LPWM1 17  
#define REN1  4   
#define LEN1  18  
#define RPWM2 21  
#define LPWM2 22  
#define REN2  19  
#define LEN2  23  

// Servo Motors
Servo sliderServo;
Servo seedServo;
Servo soilServo;
#define RELAY_PIN 14               // Relay for sprinkler

const int sliderPin = 13;          //Checked
const int seedServoPin = 32;       //Checked
const int soilServoPin = 5;       //
const int soilMoisturePin = 36;    //Checked INPUT
const int waterlevelPin = 34;    //

// Serial Communication
#define ESP_TX 25
#define ESP_RX 33

// DFPlayer Mini Pins
#define DF_RX 27  // ESP32 RX1 <-- DFPlayer RX
#define DF_TX 26  // ESP32 TX1 --> DFPlayer TX

// Speed Control
int speedPWM = 150;

// Web Server
WebServer server(80);

// DFPlayer Mini
DFRobotDFPlayerMini player;

void moveForward() {   //100% Working
    analogWrite(RPWM1, speedPWM);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM2, speedPWM);
    analogWrite(LPWM2, 0);
}

void moveBackward() {   //100% Working
    analogWrite(RPWM1, 0);
    analogWrite(LPWM1, speedPWM);
    analogWrite(RPWM2, 0);
    analogWrite(LPWM2, speedPWM);
}

void turnLeft() {    //100% Working
    analogWrite(RPWM1, 0);
    analogWrite(LPWM1, speedPWM);
    analogWrite(RPWM2, speedPWM);
    analogWrite(LPWM2, 0);
}

void turnRight() {    //100% Working
    analogWrite(RPWM1, speedPWM);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM2, 0);
    analogWrite(LPWM2, speedPWM);
}

void stopMotors() {   //100% Working
    analogWrite(RPWM1, 0);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM2, 0);
    analogWrite(LPWM2, 0);
}

void handleMotorControl() {    //100% Working
    if (server.hasArg("State")) {
        String state = server.arg("State");
        if (state == "F") moveForward();
        else if (state == "B") moveBackward();
        else if (state == "L") turnLeft();
        else if (state == "R") turnRight();
        else stopMotors();
        Serial.println("Motor State: " + state);
        server.send(200, "text/plain", "Motor command received: " + state);
    }
}

void handleSpeedControl() {    //100% Working CAR Speed
    if (server.hasArg("State")) {
        int speedValue = server.arg("State").toInt();
        speedPWM = map(speedValue, 0, 10, 0, 255);
        Serial.println("Speed set to: " + String(speedPWM));
        server.send(200, "text/plain", "Speed set to: " + String(speedPWM));
    }
}

void handleServoControl() {    //100% Working (Camera Servo)
    if (server.hasArg("State")) {
        int angle = server.arg("State").toInt();
        sliderServo.write(angle);
        Serial.println("Servo moved to: " + String(angle) + "°");
        server.send(200, "text/plain", "Servo moved to: " + String(angle) + "°");
    }
}

void handleSeedServo() {       //100% Working
    if (server.hasArg("State")) {
        String action = server.arg("State");
        if(action == "SEED_MOVE"){
           player.play(9);   //DF player "Seed is Droped" 009.mp3    4                      //Soil'ss Moisture
           delay(1000);
           seedServo.write(140);
        } else if(action == "SEED_STOP"){
           seedServo.write(0);
        }
        //seedServo.write(action == "SEED_MOVE" ? 140 : 0);    //player.play(1);    
        server.send(200, "text/plain", "Seed servo command received: " + action);
    }
}

void handleSoilServo() {       //100% Working                                         
    if (server.hasArg("State")) {
        String action = server.arg("State");
        if(action == "SOIL_MOVE"){
           player.play(8);   //DF player "Soil Moisture is Being Fetched" 008.mp3    9    //Sprinkler OFF
           Serial2.println("1");
           delay(1000);
        } else if(action == "SOIL_STOP"){
           Serial2.println("2");
           delay(1000);
        }    
        server.send(200, "text/plain", "Soil servo command received: " + action);
    }
}

String getSoilMoisture() {     //100% Working                                            
    int value = analogRead(soilMoisturePin);
    return String(map(value, 0, 4095, 100, 0));
}

String getWaterLevel() {       //100% Working
    player.play(5);   //DF player "Checking Waterlevel" 005.mp3                   6       //Config Done Online
    delay(1000);
    int waterLevel = analogRead(waterlevelPin);
    int percentage = map(waterLevel, 0, 900, 0, 100);
    return String(percentage);
}

void handleSprinkler() { //100% Working
    if (server.hasArg("State")) {
        String state = server.arg("State");
        if (state == "ON") {
            player.play(6);   //DF player "Sprinkler ON"  006.mp3                   7      //Checking Water level
            delay(1000);
            digitalWrite(RELAY_PIN, HIGH);
        } else if (state == "OFF") {
            digitalWrite(RELAY_PIN, LOW);
            player.play(7);   //DF player "Sprinkler OFF" 007.mp3                    9       //Sprinkler ON
            delay(1000);
        }
        Serial.println("Sprinkler State: " + state);
        server.send(200, "text/plain", "Sprinkler command received: " + state);
    }
}

///////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
//////////////////////////////////// DFPlayer Mini/////////////////////////////////

    // Initialize Serial Communication for                
    Serial1.begin(9600, SERIAL_8N1, DF_RX, DF_TX);
    if (!player.begin(Serial1)) {
        Serial.println("DFPlayer Mini not detected! Check wiring.");
        while (true);
    }
    Serial.println("DFPlayer Mini Ready!");
    player.volume(30); // Set volume
    player.play(1);    // Play Robot has Started (1) = 1st File in SD Card  001.mp3
    delay(2000);
    
////////////////////////////////////Wi-Fi//////////////////////////////////////////
    
    Serial.println("Setting up WiFi...");
    WiFi.softAP(ssid, password);
    
    while (WiFi.softAPgetStationNum() == 0) {
        player.play(2);   //DF player "Connecting to Wi-fi" 002.mp3
        Serial.print(".");
        delay(2000);
    }
    Serial.println("\nWiFi AP started successfully.");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.softAPIP());
    player.play(3);   //DF player "Wi-fi Connected"  003.mp3
    delay(2000);
///////////////////////////////////////////////////////////////////////////////////
    
    pinMode(RPWM1, OUTPUT);
    pinMode(LPWM1, OUTPUT);
    pinMode(REN1, OUTPUT);
    pinMode(LEN1, OUTPUT);
    pinMode(RPWM2, OUTPUT);
    pinMode(LPWM2, OUTPUT);
    pinMode(REN2, OUTPUT);
    pinMode(LEN2, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    
    digitalWrite(REN1, HIGH);
    digitalWrite(LEN1, HIGH);
    digitalWrite(REN2, HIGH);
    digitalWrite(LEN2, HIGH);
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is OFF at start
    
    sliderServo.attach(sliderPin);
    sliderServo.write(90);
    seedServo.attach(seedServoPin);
    seedServo.write(0);
    soilServo.attach(soilServoPin);
    soilServo.write(0);
    
    Serial2.begin(9600, SERIAL_8N1, ESP_RX, ESP_TX);
    
    server.on("/control", handleMotorControl);  //100% Working
    server.on("/speed", handleSpeedControl);    //100% Working
    server.on("/servo", handleServoControl);    //100% Working Camera
    server.on("/seedservo", handleSeedServo);   //100% Working
    server.on("/sprinkler", handleSprinkler);   //100% Working
    server.on("/soilservo", handleSoilServo);
    
    server.on("/getsoil", []() {
        String soilMoisture = getSoilMoisture();
        server.send(200, "text/plain", "Soil moisture: " + soilMoisture);
    });
    
    server.on("/getwaterlevel", []() {
        String waterLevel = getWaterLevel();
        server.send(200, "text/plain", "Water level: " + waterLevel + "%");
    });
    
    server.begin();
    Serial.println("HTTP server started");

    player.play(4);   //DF player "Configuration Done Ready to use" 004.mp3          5          //Seed is dropped
    delay(2000);
}

void loop() {
    server.handleClient();
}
