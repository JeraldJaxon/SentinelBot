<p align="center">
  <img src="https://img.shields.io/badge/Platform-Raspberry_Pi_+_ESP32-red?style=for-the-badge&logo=raspberrypi" />
  <img src="https://img.shields.io/badge/Vision-OpenCV-blue?style=for-the-badge&logo=opencv" />
  <img src="https://img.shields.io/badge/Language-Python_|_Embedded_C-yellow?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Navigation-RFID_+_Line_Following-purple?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" />
</p>

<h1 align="center">🤖 SentinelBot</h1>
<p align="center"><b>Semi-Autonomous Indoor Surveillance & Delivery Robot</b></p>
<p align="center">
  A Raspberry Pi + ESP32 powered robot capable of real-time face recognition, RFID-based waypoint navigation, autonomous delivery, and continuous patrol surveillance — all without human supervision.
</p>

---

## 📌 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [Circuit Diagram](#-circuit-diagram)
- [Software Requirements](#-software-requirements)
- [Getting Started](#-getting-started)
- [Working Principle](#-working-principle)
- [Operating Modes](#-operating-modes)
- [Project Images](#-project-images)
- [Applications](#-applications)
- [Future Improvements](#-future-improvements)
- [Team](#-team)

---

## 🔍 Overview

**SentinelBot** is a semi-autonomous indoor robot built for surveillance and item delivery in controlled environments. The dual-processor architecture pairs a **Raspberry Pi** (high-level vision & decision making) with an **ESP32** (real-time sensor acquisition & motor control), creating a robust, low-latency system capable of operating with minimal human oversight.

The robot identifies authorized personnel via a local face recognition database, navigates indoors using RFID checkpoints and line-following, delivers items to designated rooms, and patrols defined routes — all while logging environmental data.

> 🎯 Built for smart offices, hospitals, warehouses, and research labs where automation of routine monitoring and delivery tasks reduces operational overhead.

---

## ✨ Features

| Feature | Description |
|---|---|
| 👤 Face Recognition | Real-time detection & identification of authorized users via Pi Camera + OpenCV |
| 📍 RFID Navigation | Room identification and waypoint-based autonomous routing |
| 📦 Delivery Mode | Autonomously transports items to target rooms and halts on arrival |
| 🚨 Patrol Mode | Continuously cycles through designated checkpoints for surveillance |
| 🌡️ Environmental Monitoring | Indoor air quality and condition sensing during operation |
| 🔗 Dual-Processor Design | Raspberry Pi handles vision; ESP32 handles sensors and motor control |
| 📶 Serial Communication | High-speed UART link between Raspberry Pi and ESP32 for coordinated control |

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     SENTINELBOT                         │
│                                                         │
│   ┌──────────────────┐       ┌──────────────────────┐   │
│   │   Raspberry Pi   │◄─────►│       ESP32          │   │
│   │                  │ UART  │                      │   │
│   │  • Face Recog.   │       │  • Motor Control     │   │
│   │  • Image Proc.   │       │  • Sensor Reading    │   │
│   │  • Navigation    │       │  • RFID Interface    │   │
│   │    Decisions     │       │  • Line Following    │   │
│   └────────┬─────────┘       └──────────┬───────────┘   │
│            │                            │               │
│   ┌────────▼─────────┐       ┌──────────▼───────────┐   │
│   │   Pi Camera      │       │  IR / RFID / Env.    │   │
│   │   (Vision Input) │       │  Sensors + Motors    │   │
│   └──────────────────┘       └──────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

**Data Flow:**
`Camera Input → Face Recognition → Navigation Decision → ESP32 Command → Motor/Sensor Action → Status Feedback`

---

## 🔧 Hardware Components

| # | Component | Role | Qty |
|---|-----------|------|-----|
| 1 | **Raspberry Pi 4 / 3B+** | Primary processor — vision, decision making | 1 |
| 2 | **ESP32 Dev Board** | Real-time sensor & motor controller | 1 |
| 3 | **Raspberry Pi Camera Module** | Face detection and recognition input | 1 |
| 4 | **RFID Reader (RC522)** | Room identification & waypoint detection | 1 |
| 5 | **RFID Tags / Cards** | Room markers and navigation checkpoints | As needed |
| 6 | **IR Sensors** | Line following and path tracking | 2–3 |
| 7 | **L298N / L293D Motor Driver** | DC motor speed and direction control | 1 |
| 8 | **DC Motors + Wheels** | Robot locomotion | 2–4 |
| 9 | **Environmental Sensor (DHT11/22, MQ series)** | Indoor condition monitoring | 1–2 |
| 10 | **Chassis / Frame** | Robot body structure | 1 |
| 11 | **Li-ion Battery Pack / Power Bank** | Portable power supply | 1 |
| 12 | **Jumper Wires, Breadboard / PCB** | Circuit connections | As needed |

---

## 🗺️ Circuit Diagram

**ESP32 Pin Reference:**

| ESP32 Pin | Connected To |
|-----------|-------------|
| GPIO 21 / 22 | I²C — RFID RC522 (SDA / SCL) |
| GPIO 18 / 19 | IR Sensor Left / Right |
| GPIO 26 / 27 | Motor Driver IN1 / IN2 (Left Motor) |
| GPIO 32 / 33 | Motor Driver IN3 / IN4 (Right Motor) |
| GPIO 4 | DHT Sensor (Data) |
| GPIO 16 / 17 | UART TX / RX → Raspberry Pi |
| 3.3V / GND | Sensor VCC / GND |
| VIN / GND | Motor Driver VCC / GND |

> 📌 Full circuit diagram image: *(add image path here)*

---

## 💻 Software Requirements

### Raspberry Pi

- Raspberry Pi OS (Bullseye or later)
- Python 3.8+
- OpenCV (`pip install opencv-python`)
- face_recognition (`pip install face_recognition`)
- NumPy (`pip install numpy`)
- PySerial (`pip install pyserial`)

### ESP32 (Arduino IDE)

- [Arduino IDE](https://www.arduino.cc/en/software) v1.8.x or v2.x
- ESP32 Board Package:
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- **MFRC522** — RFID library (install via Library Manager)
- **DHT Sensor Library** — by Adafruit
- **L298N / Motor Driver Library**

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/JeraldJaxon/SentinelBot.git
cd SentinelBot
```

### 2. Set Up Raspberry Pi

```bash
cd raspberry_pi
pip install -r requirements.txt
```

Add known faces to the `known_faces/` directory:
```
known_faces/
  ├── person_name_1.jpg
  └── person_name_2.jpg
```

### 3. Flash ESP32

Open `esp32/SentinelBot_ESP32.ino` in Arduino IDE, select `ESP32 Dev Module` and the correct COM port, then upload.

### 4. Configure Serial Communication

In `raspberry_pi/config.py`, set the correct serial port:

```python
SERIAL_PORT = "/dev/ttyUSB0"   # adjust per your setup
BAUD_RATE   = 115200
```

### 5. Run the Main Controller

```bash
python3 raspberry_pi/main.py
```

---

## ⚙️ Working Principle

```
1. 👁️  Camera streams frames → OpenCV detects faces in real time
2. 🔐  Face matched against local database → authorized / unauthorized flagged
3. 📍  RFID reader scans room tags → ESP32 identifies current location
4. 🧭  Raspberry Pi issues navigation command to ESP32 over UART
5. ➡️  ESP32 drives motors; IR sensors maintain line-following path
6. 📦  On reaching destination RFID tag → robot halts, delivery confirmed
7. 🚨  In patrol mode → robot loops through all registered checkpoints
8. 🌡️  Environmental sensors stream data continuously during operation
9. 🔄  Cycle repeats; Raspberry Pi logs all events with timestamps
```

---

## 🕹️ Operating Modes

### 🚨 Patrol Mode
The robot autonomously follows a predefined route between registered RFID checkpoints, performing surveillance. Unrecognized faces trigger an alert flag in the log.

### 📦 Delivery Mode
A target room is selected (via terminal or future mobile app). The robot navigates to the matching RFID tag using line-following, deposits the item, and returns to the home base.

### 🔐 Access Control Mode
Stationary operation at an entry point — grants or denies access based on face recognition results.

---

## 📸 Project Images

### Hardware Setup
*(Add image here)*

### Circuit Diagram
*(Add image here)*

### Robot in Action
*(Add demo GIF / video link here)*

---

## 🌾 Applications

- 🏥 **Hospital Delivery** — Medicine and document transport between wards
- 🏢 **Smart Office** — Autonomous mail delivery and access monitoring
- 🏭 **Warehouse Monitoring** — Inventory patrol and anomaly detection
- 🔬 **Research & Prototyping** — Robotics, CV, and IoT integration studies
- 🎓 **Educational Platform** — Embedded systems and AI learning tool
- 🔒 **Security Patrol** — Continuous indoor monitoring with face-based access control

---

## 🔮 Future Improvements

- [ ] **SLAM Navigation** — Autonomous map building without fixed RFID tags
- [ ] **Mobile App Control** — React Native / Flutter remote dashboard
- [ ] **AI Intrusion Detection** — Anomaly-based unauthorized access alerting
- [ ] **Voice Commands** — Natural language instruction interface
- [ ] **Cloud Logging** — Firebase / AWS IoT event storage and analytics
- [ ] **Multi-Robot Coordination** — Fleet management for larger environments
- [ ] **Obstacle Avoidance** — Ultrasonic / depth sensor collision prevention

---

## 🛠️ Technologies Used

![Raspberry Pi](https://img.shields.io/badge/Raspberry_Pi-A22846?style=flat-square&logo=raspberrypi)
![ESP32](https://img.shields.io/badge/ESP32-Espressif-red?style=flat-square)
![Python](https://img.shields.io/badge/Python-3.8+-blue?style=flat-square&logo=python)
![OpenCV](https://img.shields.io/badge/OpenCV-Vision-green?style=flat-square&logo=opencv)
![Embedded C](https://img.shields.io/badge/Language-Embedded_C-yellow?style=flat-square)
![RFID](https://img.shields.io/badge/Navigation-RFID-purple?style=flat-square)
![Arduino](https://img.shields.io/badge/IDE-Arduino-teal?style=flat-square&logo=arduino)

---

## 👨‍💻 Team

**Team SENTINELBOT** — SNMIMT Engineering College, Ernakulam

| Name | Role |
|------|------|
| **Jerald Jaxon** *(Lead)* | System architecture, Raspberry Pi programming, ESP32 integration, face recognition |
| **Ajoy Antony** | Hardware assembly, sensor interfacing, RFID integration |
| **Amritha V.S** | Project ideation, navigation logic development |
| **Sanjana Jojy** | Documentation, testing, repository management |

<p>
  <a href="https://github.com/JeraldJaxon">
    <img src="https://img.shields.io/badge/GitHub-JeraldJaxon-black?style=flat-square&logo=github" />
  </a>
  &nbsp;
  <a href="https://www.linkedin.com/in/jeraldjaxon">
    <img src="https://img.shields.io/badge/LinkedIn-Connect-blue?style=flat-square&logo=linkedin" />
  </a>
</p>

---

<p align="center">
  ⭐ If you found SentinelBot useful, please consider giving it a star!<br/>
  Made with ❤️ by Team SENTINELBOT
</p>
