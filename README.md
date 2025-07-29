# InfoEducatie2025_SortoBot

SortoBot este un prototip de robot autonom proiectat pentru a detecta și sorta deșeurile menajere (plastic, sticlă, carton) în mod automat, pe baza culorii coșurilor de gunoi.


![sortobot-fata](https://github.com/user-attachments/assets/0be4961a-94c6-4fc7-93eb-86d985d4c41f)

![sortobot-spate](https://github.com/user-attachments/assets/2305cf88-7383-4299-aec0-4dba8b875e03)


---

## Funcționalități

- Detectează automat coșurile de gunoi folosind ESP32-CAM și OpenCV
- Trimite imaginile către un laptop prin WebSocket
- Procesează culoarea dominantă în imagine și sortează gunoiul în compartimentul potrivit
- Urmărește linia de pe jos cu senzori IR pentru a simula traseul pe o stradă
- Oprește în fața unui coș de gunoi detectat (cu senzor ultrasonic)
- Feedback vizual prin LED-uri colorate (roșu, verde, galben, albastru)
- Descărcare automată într-un loc printr-o platformă înclinabilă

---

## Componente

- **ESP32-WROOM** – control general al robotului
- **ESP32-CAM** – captură imagine și transmitere prin WebSocket
- **PCA9685** – control 6× servomotoare (braț robotic, mecanisme)
- **L298N** – driver motor pentru deplasare
- **Senzori IR + ultrasonic** – urmărire linie & oprire la coș
- **Senzor electromagnetic** - Senzor Hall Analog, magnetic, KY-035 pentru variațiile de tensiune 
- **3× baterii RCR123A + 2× convertoare buck** – alimentare stabilă

---

## Software

- Procesare imagine: [YOLOv8 + OpenCV](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/computer%20vision/bin_detect.py) (500 ms / imagine)
- Comunicare: WebSocket între ESP32-CAM și [server Node.js](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/server/server.js)
- Comenzi sortare/LED: HTTP către ESP32-WROOM
- Algoritm [harta](https://github.com/Selubipu/InfoEducatie2025_SortoBot/blob/main/relevant%20files/harta_oras.html): Dijkstra (simulat în interfață HTML+JS)


## Portofoliu

[SortoBot_Porotofoliu.pdf](https://github.com/user-attachments/files/20531227/SortoBot_Porotofoliu.pdf)


## Schema logica:
![Image](https://github.com/user-attachments/assets/4635153e-174f-4f07-bb7c-84c2de5af619)

## Videos and images
![449458835-25b689ca-6fb0-4974-b696-684fc74e2748](https://github.com/user-attachments/assets/a9da2578-b35f-405a-b837-394831c31751)

https://github.com/user-attachments/assets/360cc1d4-3342-45b5-8489-b0b60394ee9c

![449458833-8d889ea7-a891-4fbb-bb07-fadc6c8f0e00](https://github.com/user-attachments/assets/f626d822-4479-4860-8af2-110f2207b43f)


https://github.com/user-attachments/assets/5af3a4a9-be06-4f97-8eef-67062ac7edec

