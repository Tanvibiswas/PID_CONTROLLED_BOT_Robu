# 🤖 Robotron — PD Controlled Maze Solving Bot

An autonomous maze-solving robot built on Arduino Uno that uses a PD (Proportional-Derivative) controller to navigate through a maze by following walls and avoiding obstacles using 3 ultrasonic sensors.

---

## 📸 Circuit Diagram
![Circuit Diagram](ckt_diagram.png)

---

## ⚙️ How It Works

The robot reads distances from 3 HC-SR04 ultrasonic sensors (Front, Left, Right) and makes decisions every 30ms:

- **Normal movement** → PD controller keeps the robot centered between left and right walls
- **Front blocked** → turns toward the side with more open space
- **Dead end** → all 3 sides blocked, robot stops completely

---

## 🧠 PD Controller

| Parameter | Value | Effect |
|-----------|-------|--------|
| `baseSpeed` | 120 | Base forward speed of both motors |
| `Kp` | 2.5 | Corrects based on how far off-center the robot is |
| `Kd` | 3.5 | Dampens oscillation, smoothens correction |

The error is calculated as `dL - dR` (left distance minus right distance).
A zero error means the robot is perfectly centered.

---

## 🛠️ Components

| Component | Quantity |
|-----------|----------|
| Arduino Uno | 1 |
| HC-SR04 Ultrasonic Sensor | 3 |
| L298N Motor Driver | 1 |
| DC Motors | 2 |
| 9V / 12V Battery | 1 |
| Jumper Wires | as needed |
| Chassis | 1 |

---

## 🔌 Pin Mapping

| Component | Pin |
|-----------|-----|
| Front Sensor TRIG | 10 |
| Front Sensor ECHO | 11 |
| Left Sensor TRIG | 6 |
| Left Sensor ECHO | 7 |
| Right Sensor TRIG | 8 |
| Right Sensor ECHO | 12 |
| IN1 (Left Motor) | 2 |
| IN2 (Left Motor) | 3 |
| IN3 (Right Motor) | 4 |
| IN4 (Right Motor) | 13 |
| ENA (Left PWM) | 5 |
| ENB (Right PWM) | 9 |

---

## 📁 Repository Structure
PID_CONTROLLED_BOT_Robu/
│
├── robu_final.ino       # Arduino source code
├── ckt_diagram.png      # Circuit diagram
└── README.md            # Project documentation

---

## 🚀 How to Upload

1. Open `robu_final.ino` in **Arduino IDE**
2. Connect Arduino Uno via USB
3. Select **Board** → Arduino Uno
4. Select correct **Port**
5. Click **Upload**

---

## 📌 Known Limitations

- Turning is open-loop (fixed 150ms delay, no sensor feedback during turn)
- Dead end detection is a terminal state, no recovery
- No memory of visited paths, purely reactive navigation

---

## 👤 Author
**Tanvi Biswas**
