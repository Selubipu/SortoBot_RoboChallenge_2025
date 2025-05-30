# InfoEducatie2025_SortoBot

SortoBot este un prototip de robot autonom proiectat pentru a detecta și sorta deșeurile menajere (plastic, sticlă, carton) în mod automat, pe baza culorii coșurilor de gunoi.

---

## Funcționalități

- Detectează automat coșurile de gunoi folosind ESP32-CAM și OpenCV
- Trimite imaginile către un laptop prin WebSocket
- Procesează culoarea dominantă în imagine și sortează gunoiul în compartimentul potrivit
- Urmărește linia de pe jos cu senzori IR pentru a simula traseul pe o stradă
- Opțional: Oprește în fața unui coș de gunoi detectat (cu senzor ultrasonic)
- Feedback vizual prin LED-uri colorate (roșu, verde, galben, albastru)
- Descărcare automată în tomberoane printr-o platformă înclinabilă

---

## Componente

- **ESP32-WROOM** – control general al robotului
- **ESP32-CAM** – captură imagine și transmitere prin WebSocket
- **PCA9685** – control 6× servomotoare (braț robotic, mecanisme)
- **L298N** – driver motor pentru deplasare
- **Senzori IR + ultrasonic** – urmărire linie & oprire la coș
- **3× baterii RCR123A + 2× convertoare buck** – alimentare stabilă

---

## Software

- Procesare imagine: Python + OpenCV
- Comunicare: WebSocket între ESP32-CAM și server Node.js
- Comenzi sortare/LED: HTTP către ESP32-WROOM
- Algoritm rutare: Dijkstra (simulat în interfață HTML+JS)
