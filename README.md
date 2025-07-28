# InfoEducatie2025_SortoBot

SortoBot este un prototip de robot autonom proiectat pentru a detecta și sorta deșeurile menajere (plastic, sticlă, carton) în mod automat, pe baza culorii coșurilor de gunoi.



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

