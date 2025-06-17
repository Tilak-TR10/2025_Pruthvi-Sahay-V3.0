#include <SoftwareSerial.h>

#define WATER_SENSOR_PIN A7  // Water level sensor input
#define RAIN_SENSOR_PIN 10   // Rain sensor input
#define PIR_SENSOR_PIN 11    // PIR sensor input

// LED Indicators
#define Buzzer 12        // 20% Water Level
#define RED_LED 13       // 40% Water Level
#define YELLOW1_LED A0   // 60% Water Level
#define YELLOW2_LED A1   // 80% Water Level
#define GREEN1_LED A2    // 90% Water Level
#define GREEN2_LED A3    // 100% Water Level

// GSM Module RX pin to Arduino 3
// GSM Module TX pin to Arduino 2
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin, txPin);

// ESP8266 RX pin to Arduino TX (D1)
// ESP8266 TX pin to Arduino RX (D0)
// SoftwareSerial espSerial();

const String PHONE = "+918780468884";

int percentage = 0;
bool stopCall = false;

/*----------------------------------------------------------------------Setup-----------------------------------------------------------------------------------*/
void setup() {
    sim800.begin(9600);
    Serial.begin(9600);
    
    pinMode(WATER_SENSOR_PIN, INPUT);
    pinMode(RAIN_SENSOR_PIN, INPUT);
    pinMode(PIR_SENSOR_PIN, INPUT);
    
    pinMode(Buzzer, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW1_LED, OUTPUT);
    pinMode(YELLOW2_LED, OUTPUT);
    pinMode(GREEN1_LED, OUTPUT);
    pinMode(GREEN2_LED, OUTPUT);
    
    sim800.println("AT");
    delay(1000);
    sim800.println("AT+CMGF=1");
    delay(1000);
    sim800.println("AT+CNMI=1,2,0,0,0");
    delay(1000);
}
/*------------------------------------------------------------------------Loop --------------------------------------------------------------------------------*/
void loop() {
    checkWaterLevel();
    checkRainSensor();
    checkPIRSensor();
    checkIncomingSMS();
}
/*-------------------------------------------------------------------Waterlevel Check -------------------------------------------------------------------------*/
void checkWaterLevel() {              //100% Working
    int waterLevel = analogRead(WATER_SENSOR_PIN);
    percentage = map(waterLevel, 0, 800, 0, 120);
    if (percentage <= 20) {
        buzzerAlert(1);
    }
    digitalWrite(RED_LED, percentage <= 20 ? LOW : HIGH);
    analogWrite(YELLOW1_LED, percentage <= 40 ? 0 : 150);
    analogWrite(YELLOW2_LED, percentage <= 60 ? 0 : 150);
    analogWrite(GREEN1_LED, percentage <= 80 ? 0 : 150);
    analogWrite(GREEN2_LED, percentage <= 100 ? 0 : 150);
}
/*-------------------------------------------------------------------Rain Check---- ----------------------------------------------------------------------------*/
void checkRainSensor() {              //100% Working
    int rainStatus = digitalRead(RAIN_SENSOR_PIN);
    if (rainStatus == LOW) {
       buzzerAlert(2);
       //Serial.println("2"); //RAIN:HIGH
       sendSMS("It's raining");
       delay(4000);
    }
}
/*---------------------------------------------------------------------PIR Check -------------------------------------------------------------------------------*/
void checkPIRSensor() {               //100% Working
    int motionStatus = digitalRead(PIR_SENSOR_PIN);   
    if (motionStatus == HIGH) {
        buzzerAlert(3);
        Serial.println("1"); //ANIMAL:HIGH
        sendSMS("Jay, Animals detected on your farmland: nature's harmony at work!");
        waitForOKResponse();
        if (!stopCall) {
            makeCall();
        }
    }
}
/*-------------------------------------------------------------------Buzzer Control----------------------------------------------------------------------------*/
void buzzerAlert(int type) {          //100% Working
    if (type == 1) {                                            // Water Level Low
        for (int i = 0; i < 3; i++) {
            digitalWrite(Buzzer, HIGH);
            delay(200);
            digitalWrite(Buzzer, LOW);
            delay(200);
        }
    } else if (type == 2) {                                    // Rain Detected
        for (int i = 0; i < 2; i++) {
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            delay(100);
        }
    } else if (type == 3) {                                    // PIR Motion Detected
        digitalWrite(Buzzer, HIGH);
        delay(3000);
        digitalWrite(Buzzer, LOW);
    }
}

/*-------------------------------------------------------------------------GSM-----------------------------------------------------------------------------------*/

void sendSMS(String message) {                              //Send SMS
    sim800.println("AT+CMGS=\"" + PHONE + "\"");
    delay(1000);
    sim800.println(message);
    delay(1000);
    sim800.write(26);
    delay(5000);
}

void checkIncomingSMS() {                                   //Check Incoming SMS
    if (sim800.available()) {
        String response = sim800.readString();
        Serial.println("Received: " + response);
        if (response.indexOf("Ok") >= 0) {
            stopCall = true;
        }
    }
}

void waitForOKResponse() {                                  //Read SMS "OK" 
    unsigned long startTime = millis();
    while (millis() - startTime < 30000) {
        checkIncomingSMS();
        if (stopCall) {
            return;
        }
    }
}

void makeCall() {                                          //Make Call
    sim800.println("ATD" + PHONE + ";");
    delay(1000);
    unsigned long callStart = millis();
    while (millis() - callStart < 20000) {
        checkIncomingSMS();
        if (stopCall) {
            hangUpCall();
            return;
        }
    }
    hangUpCall();
}

void hangUpCall() {                                        //End Call
    sim800.println("ATH");
    delay(1000);
}
