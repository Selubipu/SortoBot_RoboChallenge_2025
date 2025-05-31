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

- Procesare imagine: Python + OpenCV(se pot folosi și modelele pytorch fine-tunate -> ViT, ResNet-18, însă necesită GPU deoarece procesarea este relativ lentă =~ 4sec/imagine)
- Comunicare: WebSocket între ESP32-CAM și server Node.js
- Comenzi sortare/LED: HTTP către ESP32-WROOM
- Algoritm rutare: Dijkstra (simulat în interfață HTML+JS)



##Schema logica:
![Image](https://github.com/user-attachments/assets/4635153e-174f-4f07-bb7c-84c2de5af619)

##Poze si filmari:

https://github.com/user-attachments/assets/b2ccb9a9-f443-49c3-909a-d3e5d988ea58

https://github.com/user-attachments/assets/23e91e9d-a596-46d2-8fa3-5b06764e7cb6

https://github.com/user-attachments/assets/e5d52afc-24f5-448b-b534-50202f314977

![Image](https://github.com/user-attachments/assets/25b689ca-6fb0-4974-b696-684fc74e2748)

https://github.com/user-attachments/assets/464c78f9-554b-40b7-92ba-7ac12fb501f8

https://github.com/user-attachments/assets/1126a804-20d5-4390-b573-944901a968cf

![Image](https://github.com/user-attachments/assets/8d889ea7-a891-4fbb-bb07-fadc6c8f0e00)
