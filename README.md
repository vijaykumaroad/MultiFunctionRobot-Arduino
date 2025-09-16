# 🤖 RoboKarhao – The Multifunction Arduino Robot  

### 📌 Project Overview  
RoboKarhao is a multifunctional robot built using **Arduino Mega 2560**.  
It can:  
- 🚗 Follow a line using IR sensors  
- 🚧 Avoid obstacles using ultrasonic sensors  
- 🎨 Detect color (blue) using a color sensor  

This project was developed as part of **ENGG-100: Measurement & Design Lab (Fall 2024)** at the  
**Department of Electrical Engineering, Syed Babar Ali School of Science and Engineering (LUMS, Pakistan)**.  

---

## 📂 Repository Structure
MultiFunctionRobot-Arduino/
│── code/
│ └── robot.ino
│── schematics/
│ └── robot_schematic.png
│── demo/
│ └── (Video link provided below)
│── README.md


---

## 🎥 Demo
Watch the demo video here:  
👉 [Project Demo on Google Drive](https://drive.google.com/file/d/1Va1h697s9i50Sf2IolDkB2cjjUmhl0VR/view?usp=sharing)

---

## 🛠️ Hardware Used
- Arduino Mega 2560  
- Ultrasonic Sensor (HC-SR04)  
- IR Sensors  
- Color Sensor (TCS3200)  
- Motor Driver Module (L298N)  
- 4WD Robot Chassis with DC Motors  
- Breadboard + Jumper wires  

---

## ⚙️ Features Implemented
- **Line Following** – IR sensors calibrated to detect black/white track  
- **Obstacle Avoidance** – Ultrasonic sensor triggers left/right turns  
- **Color Detection** – Robot detects blue and stops  

---

## 📊 Results
- Line following accuracy: ~90%  
- Obstacle avoidance success: ~95% at ≤ 17 cm  
- Color detection (blue): ~92% accuracy  

---

## 👨‍💻 Team Contributions
**Vijay Kumar (2020-10-0445)**  
- Robot chassis assembly & motor configuration  
- Obstacle avoidance logic  
- Sensor calibration (IR & Color)  
- Color detection algorithm  
- Debugging & optimization  
- Project report & presentation  

**Obaidullah (2028-10-0483)**  
- Robot chassis assembly & motor configuration  
- Obstacle avoidance logic  
- Sensor calibration (IR & Color)  
- Color detection algorithm  
- Debugging & optimization  
- Circuit schematic  

---

## 📌 Future Improvements
- Increase sensor range  
- Optimize Arduino code for faster response  
- Add more colors & complex path detection  
- Wireless control or IoT integration  

---

## 📜 License
This project is shared for educational purposes. Feel free to use and improve it. ✨
