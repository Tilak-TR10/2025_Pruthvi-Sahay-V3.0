#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Pruthvi_Sahay_Farm";      // Replace with your WiFi SSID
const char* password = "12345678";            // Replace with your WiFi password

ESP8266WebServer server(80);
RTC_DS3231 rtc;

#define LED D4
#define RELAY_1 D5
#define RELAY_2 D6
#define RELAY_3 D7
#define RELAY_4 D8

int scheduledHour = -1;
int scheduledMinute = -1;
bool relayTriggered = false;
unsigned long relayStartTime = 0;
const int relayDuration = 15000; // 15 seconds
const int waterlevelPin = A0;

/*-------------------------------------------------------------------Relay Control ------------------------------------------------------------------------------*/
void handleRelay() {                    //100% Working
  if (server.hasArg("State")) {
    String state = server.arg("State");
    //Serial.println(state);
    printCurrentTime();
    if (state == "Switch_1_ON") {
      digitalWrite(RELAY_1, LOW);
      server.send(200, "text/plain", "Relay 1 ON");
    } else if (state == "Switch_1_OFF") {
      digitalWrite(RELAY_1, HIGH);
      server.send(200, "text/plain", "Relay 1 OFF");
    } else if (state == "Switch_2_ON") {
      digitalWrite(RELAY_2, LOW);
      server.send(200, "text/plain", "Relay 2 ON");
    } else if (state == "Switch_2_OFF") {
      digitalWrite(RELAY_2, HIGH);
      server.send(200, "text/plain", "Relay 2 OFF");
    } else if (state == "Switch_3_ON") {
      digitalWrite(RELAY_3, LOW);
      server.send(200, "text/plain", "Relay 3 ON");
    } else if (state == "Switch_3_OFF") {
      digitalWrite(RELAY_3, HIGH);
      server.send(200, "text/plain", "Relay 3 OFF");
    } else if (state == "Switch_4_ON") {
      digitalWrite(RELAY_4, LOW);
      server.send(200, "text/plain", "Relay 4 ON");
    } else if (state == "Switch_4_OFF") {
      digitalWrite(RELAY_4, HIGH);
      server.send(200, "text/plain", "Relay 4 OFF");
    }
  }else {
   server.send(400, "text/plain", "Missing parameters");
  }
}
/*-------------------------------------------------------------------Set Timer For Relay ------------------------------------------------------------------------*/
void setTimeRTC() {                     //100% Working
  if (server.hasArg("Stime")) {
    String StimeData = server.arg("Stime");
    //Serial.println(StimeData);
    scheduledHour = StimeData.substring(0, 2).toInt();
    scheduledMinute = StimeData.substring(3, 5).toInt();
    //Serial.println(scheduledHour);
    //Serial.println(scheduledMinute); 
    relayTriggered = false;
    server.send(200, "text/plain", "Time Set Successfully");
  } else {
    server.send(400, "text/plain", "Missing parameters"); 
  }
}

/*-------------------------------------------------------------------Set Time to RTC ----------------------------------------------------------------------------*/

void setRTC() {                         //100% Working
  if (server.hasArg("time")) {
    String timeData = server.arg("time"); // Get time in "dd:MM:yyyy:hh:mm" format
    int day = timeData.substring(0, 2).toInt();
    int month = timeData.substring(3, 5).toInt();
    int year = timeData.substring(6, 10).toInt();
    int hour = timeData.substring(11, 13).toInt();
    int minute = timeData.substring(14, 16).toInt();
    rtc.adjust(DateTime(year, month, day, hour, minute, 0));
    server.send(200, "text/plain", "RTC Updated");
  } else {
    server.send(400, "text/plain", "Missing time parameter");
  }
}

/*-------------------------------------------------------------------Relay On/ff based on Time Set---------------------------------------------------------------*/
void checkScheduledTime() {             //100% Working
  DateTime now = rtc.now();
  if (now.hour() == scheduledHour && now.minute() == scheduledMinute && !relayTriggered) {
    digitalWrite(RELAY_1, LOW);
    digitalWrite(RELAY_2, LOW);
    //digitalWrite(RELAY_3, LOW);
    //digitalWrite(RELAY_4, LOW);
    relayTriggered = true;
    relayStartTime = millis();
    //Serial.println("Relay ON (Automatic)");
  }
  if (relayTriggered && millis() - relayStartTime >= relayDuration) {
    digitalWrite(RELAY_1, HIGH);
    digitalWrite(RELAY_2, HIGH);
    //digitalWrite(RELAY_3, HIGH);
    //digitalWrite(RELAY_4, HIGH);
    relayTriggered = false;
    //Serial.println("Relay OFF (Automatic)");
  }
}
/*---------------------------------------------------------------------Waterlevel print--------------------------------------------------------------------------*/

String getWaterLevel() {                //100% Working
    int waterLevel = analogRead(waterlevelPin);
    int percentage = map(waterLevel, 0, 1023, 0, 100);
    return String(percentage);
}
/*-------------------------------------------------------------------Print Current Time-------------------------------------------------------------------------*/
void printCurrentTime() {               //100% Working
  DateTime now = rtc.now();
  //Serial.printf("Current RTC Time: %02d:%02d:%04d %02d:%02d:%02d\n", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
}

void handleRoot() {                     //100% Working
    server.send(200, "text/html", "<h1>ESP8266 Relay Control</h1>"); 
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Checkarduino() { 
  if (Serial.available()) {
   String receivedData = Serial.readString(); // Read incoming data until newline
   int readarduino = receivedData.toInt();
   if (readarduino == 1) {
     digitalWrite(RELAY_1, LOW);   
     server.send(200, "text/plain", "ANIMAL: Alert" "%");
   } else if (readarduino == 2) {
     digitalWrite(RELAY_2, LOW);
     server.send(200, "text/plain", "RAIN: Alert" "%");
   } else {
    digitalWrite(RELAY_1, HIGH);
    digitalWrite(RELAY_2, HIGH);
   }
  } 
}
----------------------------------------------------------------Setup-----------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
/*
  Serial.println("AP Mode Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
*/
  pinMode(LED, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  
  Wire.begin(D2, D1);
  if (!rtc.begin()) {
    //Serial.println("Couldn't find RTC");
    while (1){
        digitalWrite(LED, HIGH);  // Turn LED on
        delay(1000);                      // Wait for 1 second
        digitalWrite(LED, LOW);   // Turn LED off
        delay(1000); 
        Serial.print(".");
      }
  }
  digitalWrite(LED, HIGH);  // Turn LED on

  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
  
  // Define web server routes
  server.on("/", handleRoot);         //100% Working
  server.on("/relay", handleRelay);   //100% Working
  server.on("/setTime", setTimeRTC);  //100% Working
  server.on("/setRTC", setRTC);       //100% Working 
  
  server.on("/getWater", []() {
     String waterLevel = getWaterLevel();
     server.send(200, "text/plain", "Water level: " + waterLevel + "%");
  });
  
  server.begin();
  //Serial.println("Server started");
}
/*---------------------------------------------------------------------Loop -----------------------------------------------------------------------------------*/

void loop() {
  server.handleClient();
  checkScheduledTime();
  //Checkarduino();
}
