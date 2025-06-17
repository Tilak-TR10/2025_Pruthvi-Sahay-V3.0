Prithvi_Sahay_Joystick_Robot V3 Made By Tilak Raval

# 🌾 # 2025_Pruthvi-Sahay-V3.0

**An AI-Driven Smart Agricultural Assistant**

Pruthvi Sahay is a two-part robotic system designed to monitor, automate, and optimize farming operations. Built using ESP32, Arduino, sensors, actuators, and wireless modules, this system brings intelligence to traditional farming.

---

## 🚜 System Overview

- **Part 1 – Farm Monitoring System (Fixed Node)**
- **Part 2 – Autonomous Farming Robot (Mobile Unit)**

---

## 📦 Part 1: Smart Farm Monitoring & Irrigation Control

### 🔍 Features

- 🚶 Motion Detection  
- 🌧️ Rain Detection  
- 💧 Water Level Monitoring with LED Indicators  
- 🕒 RTC-based Sprinkler Scheduling  
- 🚿 Automated Sprinkler Control  
- 🔔 Emergency Buzzer Alerts  
- 📲 SMS Notifications via GSM

### 🧰 Hardware

- Arduino Pro Mini  
- ESP32  
- PIR Sensor  
- Rain Sensor  
- DS3231 RTC Module  
- 5-Level Water Sensor (LEDs)  
- Relay Module  
- Buzzer  
- SIM800L GSM Module  

### 🔁 Workflow

1. Detects motion/rain/water level.
2. Buzzer or SMS alerts sent on anomalies.
3. RTC schedules automatic irrigation unless rain is detected.
4. Sprinkler turns ON via relay for 5 seconds.

---

## 🤖 Part 2: Autonomous Farming Robot (Mobile Unit)

### 🌱 Features

- 🌡️ **Soil Moisture Monitoring**  
- 🌾 **Seed Dropping Mechanism**  
- 💦 **Sprinkler Control with Water Tank Monitoring**  
- 🎥 **Onboard Camera Surveillance**  
- 🧭 **Directional Movement (Forward, Backward, Left, Right)**  
- 🔋 Power-efficient & Modular Design  

### 🔧 Hardware Components

| Component              | Description                             |
|------------------------|-----------------------------------------|
| ESP32                  | Main Controller with Wi-Fi/BLE          |
| Soil Moisture Sensor   | Detects dry/wet soil                    |
| Servo Motor (Seed Drop)| Drops seeds in intervals                |
| DC Pump + Relay        | Sprays water from onboard tank          |
| Ultrasonic/Float Sensor| Monitors water level in tank            |
| Camera Module (ESP32-CAM or USB)| Monitors environment       |
| Motor Driver (TB6612FNG / L298N)| Controls robot wheels        |
| Wheels + Chassis       | 2WD or 4WD setup for mobility           |
| Power Bank / Li-Ion    | Portable power supply                   |

### 🔁 Workflow

1. **Soil Moisture Sensor** detects dryness.
2. **Servo** activates to **drop seeds** when soil is ready.
3. **Water Pump** activates if soil is dry *and* water level is sufficient.
4. **Camera** monitors field visually (live feed or capture).
5. Robot moves across the field via **manual control / automated paths**.
6. **Obstacle avoidance** and path logic (optional – future scope).

---

## 📷 Robot Camera Functionality

- Live stream via ESP32-CAM or similar module.
- Monitors plant health, presence of animals, or intrusions.
- Optional: Capture images on movement or time intervals.

---

## 🧪 Sensor Data Flow

| Sensor/Module        | Action                                       |
|----------------------|----------------------------------------------|
| Soil Moisture Sensor | Checks condition for seed drop & watering    |
| Float Sensor         | Confirms water availability in tank          |
| Camera Module        | Monitors surroundings                        |
| RTC (optional)       | Can be used to trigger periodic operations   |

---

## 🛠️ Setup Instructions

Step 1. **Clone the repository:**
   ``bash
   git clone https://github.com/your-username/pruthvi-sahay.git
   cd pruthvi-sahay

Step 2. **Install Required Libraries:**

   Servo.h, RTClib, Wire, SoftwareSerial, WiFi, ESP32Camera (if needed)

Step 3. **Upload the Code:**

   esp32_main_robot.ino → ESP32 Robot Unit

   arduino_monitoring.ino → Arduino Monitoring Node


📈 Future Enhancements

🤖 Autonomous navigation with GPS or IR grid tracking

🧠 AI-based plant health analysis via image classification

📡 MQTT/Blynk dashboard integration for remote monitoring

🌦️ Weather API integration for intelligent irrigation decisions

🤝 Contributors
Tilak Raval – Embedded Systems | AI & Robotics Engineer
Creating smarter farms, one robot at a time

📜 License
This project is licensed under the MIT License – see the LICENSE file for details.
  
  © 2025 Tilak Raval. All rights reserved.
  This project and all associated designs, code, and documentation are the intellectual property of Tilak Raval.
  You are free to use, modify, and distribute this project under the MIT License, provided proper attribution is maintained.
  Commercial usage, plagiarism, or reselling without written permission is strictly prohibited.
