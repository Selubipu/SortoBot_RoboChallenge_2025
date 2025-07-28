#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>

// Servo pe PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN  100
#define SERVOMAX  550
void setServoAngle(uint8_t servo, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servo, 0, pulse);
}

// LED-uri pentru feedback culoare

#define BLUE_LED    2
#define YELLOW_LED  4
#define GREEN_LED   16
#define RED_LED     17

#define TRIG_PIN 21
#define ECHO_PIN 19
#define LED_PIN 12
#define CameraPin 15

const int sensorLeft = 5;
const int sensorRight = 18;
const int ENB = 27;
const int IN3 = 25;
const int IN4 = 26;
const int range = 15;
const int idle = 80;
const int ENCODER_PIN = 34;

int diffyL = 6, diffyR = 10, wrist = 7, claw = 8, depo = 9, steering = 15;
int nd = 48;
const int scoring = 60;
const int wristin = 130;
bool done = false;
bool gotbin=false;

// WiFi credentials
const char* ssid = "RoboNetwork";
const char* password = "1234567890";
WebServer server(80);

void lightColorLED(String color) {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  if (color == "red") digitalWrite(RED_LED, HIGH);
  else if (color == "green") digitalWrite(GREEN_LED, HIGH);
  else if (color == "blue") digitalWrite(BLUE_LED, HIGH);
  else if (color == "yellow") digitalWrite(YELLOW_LED, HIGH);
}


//Encoder
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;

float rpm = 0;
int finalPWM = 0;               // PWM stabil care va duce motorul la 1200 RPM
const int targetRPM = 1000;     // RPM dorit
const int pulsesPerRevolution = 1;  // depinde de encoderul tău

void IRAM_ATTR countPulse() {
  unsigned long currentTime = micros();
  if (currentTime - lastPulseTime > 1000) { // debounce 1 ms
    pulseInterval = currentTime - lastPulseTime;
    lastPulseTime = currentTime;
  }
}

//Motor Functions

void driveForward(int aaa) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, aaa); }
void stopMotor() { digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0); }
void driveBackwards() { digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, 90); }

void chillguy(){
  for(int i=0; i<=15; i++)
  if(i!=steering)setServoAngle(i,90);
  else setServoAngle(steering, idle);
}

// ================= ARM + CLAW ====================
void down() {
  for (int i = 1; i <= nd; i++) {
    setServoAngle(diffyR, 90 + i);
    setServoAngle(diffyL, 90 - i);
    delay(20);
  }
}

void up() {
  for (int i = 1; i <= nd; i++) {
    setServoAngle(diffyR, 90 + nd - i);
    setServoAngle(diffyL, 90 - nd + i);
    delay(20);
  }
}

void close() { setServoAngle(claw, 100); }
void open()  { setServoAngle(claw, 40); }
void wrist_intake() { setServoAngle(wrist, 140); }

void wirst_grab() {
  for (int i = 140; i >= wristin; i--) {
    setServoAngle(wrist, i);
    delay(10);
  }
}

void intake() {
  open(); wrist_intake(); down(); wirst_grab(); delay(100); close(); up();
}

void sort(int n) {
  switch(n) {
  case 1:
  for (int i = 1; i <= 60; i++) {
    setServoAngle(diffyR, 90 - i);
    setServoAngle(diffyL, 90 - i);
    delay(7);
  }
    break;

  case 2:
  for (int i = 1; i <= 12; i++) {
    setServoAngle(diffyR, 90 - i);
    setServoAngle(diffyL, 90 - i);
    delay(7);
  }
    break;

  case 3:
  for (int i = 1; i <= 40; i++) {
    setServoAngle(diffyR, 90 + i);
    setServoAngle(diffyL, 90 + i);
    delay(7);
  }
    break;
  }
}

void score(int n) {
  setServoAngle(wrist, 90);
  sort(n);
  delay(1000);
  for(int i=90; i>=scoring; i--){
  setServoAngle(wrist, i);
  delay(10);}
}

void letbin() {
  setServoAngle(diffyL, 90); setServoAngle(diffyR, 90);
  wirst_grab(); down(); open(); up();
}

void outtake() {
  setServoAngle(depo, 140);
  delay(1000);
  setServoAngle(depo, 90);
  delay(1000);
}

void photo() {
  digitalWrite(CameraPin, HIGH);
  delay(500);
  digitalWrite(CameraPin, LOW);
}

// ================= SERVER ====================
void handleLED() {
  String color = server.arg("color");
  Serial.println("Culoare primită: " + color);
  lightColorLED(color);

  int code = (color == "blue") ? 1 :
             (color == "yellow") ? 2 :
             (color == "green") ? 3 :
             (color == "red") ? -1 : 0;

  if (code > 0) {
    intake(); delay(300);
    score(code); delay(300);
    letbin(); delay(300);
    chillguy();
    gotbin=true;
  }
    driveForward(finalPWM);
    delay(500);
    stopMotor();

  server.send(200, "text/plain", "OK");
}

// ================= SETUP & LOOP ====================
void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT); pinMode(CameraPin, OUTPUT);
  pinMode(sensorLeft, INPUT); pinMode(sensorRight, INPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  for (int pin : {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED}) pinMode(pin, OUTPUT);
  digitalWrite(CameraPin, LOW);
  Wire.begin(23, 22);
  pwm.begin(); pwm.setPWMFreq(50);

  //pwm finder
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), countPulse, FALLING);

  digitalWrite(IN3, HIGH);  // direcția înainte
  digitalWrite(IN4, LOW);

  int pwm = 90;  // pornim de la 90 ca valoare minimă
  analogWrite(ENB, pwm);

  Serial.println("Calibrare pentru 1200 RPM...");

  while (true) {
    delay(300);  // un delay mai lung ca să se stabilizeze turația

    if (pulseInterval > 1000) {
      rpm = (60.0 * 1000000.0) / (pulseInterval * pulsesPerRevolution);
    } else {
      rpm = 0;
    }

    Serial.print("PWM: ");
    Serial.print(pwm);
    Serial.print(" -> RPM: ");
    Serial.println(rpm);

    if (rpm < targetRPM - 20) {
      pwm++;
      if (pwm > 255) break;
      analogWrite(ENB, pwm);
    } else if (rpm > targetRPM + 20) {
      pwm--;
      if (pwm < 90) pwm = 90;
      analogWrite(ENB, pwm);
    } else {
      finalPWM = pwm;
      Serial.print("Final PWM pentru ");
      Serial.print(targetRPM);
      Serial.print(" RPM: ");
      Serial.println(finalPWM);
      break;
    }
  }
  stopMotor();

  for (int pin : {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED}) digitalWrite(pin, HIGH);
  delay(1000);
  for (int pin : {RED_LED, GREEN_LED, BLUE_LED, YELLOW_LED}) digitalWrite(pin, LOW);

  WiFi.setHostname("SBot-CENTRAL");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("WiFi conectat: " + WiFi.localIP().toString());

  server.on("/led", HTTP_GET, handleLED);
  server.begin();
  Serial.println("Server Web activ!");
  chillguy();
}


int a=false;

void loop() {
  server.handleClient();

  int left = digitalRead(sensorLeft);
  int right = digitalRead(sensorRight);

  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = duration * 0.034 / 2;

  if (distance_cm <= 15.0) {
    stopMotor();//driveBackwards(); delay(200); stopMotor();
    photo(); // după care Python trimite /led?color=...
    return;
    delay(1000);
  }

  if (left == LOW && right == LOW) {
    driveForward(finalPWM); setServoAngle(steering, idle);
  } else if (left == LOW && right == HIGH) {
    driveForward(finalPWM); setServoAngle(steering, idle - range);
  } else if (left == HIGH && right == LOW) {
    driveForward(finalPWM); setServoAngle(steering, idle + range);
  } else {
    stopMotor();
    if (!done  && gotbin==true) { outtake(); done = true; }
  }

  delay(50);
}
