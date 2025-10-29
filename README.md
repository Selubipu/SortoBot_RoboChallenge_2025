# RoboChallenge2025_SortoBot

SortoBot is an autonomous prototype robot designed to detect and sort already sorted waste bins (plastic, glass, paper) automatically, based on the bin color. It picks up the bins and sorts their contents into the corresponding compartments inside the robot, minimizing human error.

![sortobot-front](https://github.com/user-attachments/assets/0be4961a-94c6-4fc7-93eb-86d985d4c41f)
![sortobot-back](https://github.com/user-attachments/assets/2305cf88-7383-4299-aec0-4dba8b875e03)

---

## 3D Onshape Model

[View the 3D model](https://cad.onshape.com/documents/6dcd17111db380910306cb9f/w/09c7ec735f4629dd65699941/e/5b817ed5bd58267ef9394a37?renderMode=0&uiState=68889b5304ee9a1da0ab395c)

---

## Features

- Automatically detects waste bins using ESP32-CAM and OpenCV
- Sends images to a laptop via WebSocket
- Processes dominant color in the image and sorts the bin into the correct compartment
- Uses YOLO object detection to not only identify the bin but also to center the robot precisely in front of it
- Follows a line on the floor using IR sensors to simulate navigation along streets
- Stops in front of a detected bin using an ultrasonic sensor
- Visual feedback through colored LEDs (red, green, yellow, blue)
- Automatic dumping into the correct compartment through a tilting platform

---

## Components

- **ESP32-WROOM** – main controller
- **ESP32-CAM** – image capture and WebSocket communication
- **PCA9685** – servo driver for 6 servos (robotic arm, mechanisms)
- **L298N** – motor driver for propulsion
- **IR sensors + ultrasonic sensor** – line following & bin detection
- **Magnetic encoder (KY-035)** – monitors motor rotations
- **3× RCR123A batteries + 2× buck converters** – stable power supply

---

## Software

- Image processing: [YOLOv8 + OpenCV](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/computer%20vision/bin_detect.py) (~500 ms/image)
- Communication: WebSocket between ESP32-CAM and [Node.js server](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/server/server.js)
- Sorting and LED commands: HTTP to ESP32-WROOM
- Path planning: [Dijkstra algorithm](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/relevant%20files/harta_oras.html) simulated in HTML+JS interface

---

## Portfolio

[SortoBot_Documentation_Robochallenge2025.pdf](https://github.com/user-attachments/files/23219410/SortoBot_Documentation_Robochallenge2025.pdf)

---

## Videos and Images

- Video & image gallery:  
[Video/Image 1](https://github.com/user-attachments/assets/23158b6e-8c34-4196-94d6-50561dbbfcba)  
![Image 2](https://github.com/user-attachments/assets/a9da2578-b35f-405a-b837-394831c31751)  
[Video/Image 3](https://github.com/user-attachments/assets/360cc1d4-3342-45b5-8489-b0b60394ee9c)  
![Image 4](https://github.com/user-attachments/assets/f626d822-4479-4860-8af2-110f2207b43f)  
[Video/Image 5](https://github.com/user-attachments/assets/5af3a4a9-be06-4f97-8eef-67062ac7edec)

---

SortoBot demonstrates how a small, autonomous robot can combine mechanical design, electronics, and AI-based image processing to sort waste efficiently while minimizing human error. YOLO object detection ensures accurate recognition and alignment with each bin, allowing the robot to operate reliably in educational and competitive scenarios.
