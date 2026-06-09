#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
#define IR_Left 8
#define IR_Right 7
#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3
#define buz 2
MFRC522 mfrc522(SS_PIN, RST_PIN); 
String DEST_A = "6E 5C 73 06";
String DEST_B = "3C 7B 77 06";
String ID = "";
char currentMode = 'S'; // 'S'=Stop, 'A'=Go A, 'B'=Go B, 'P'=Patrol
bool patrolToggle = true; // true = go to A, false = go to B
void ReadRFID();
void LineIR(String);
void carStop();
void ffCar();
void revCar();
void leftCar();
void rightCar();
void buzzer(int);
void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  pinMode(IR_Left, INPUT);
  pinMode(IR_Right, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(buz, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  buzzer(1);  }
void loop() {
  if (currentMode == 'A') {
    LineIR(DEST_A);
    currentMode = 'S'; // Stop after reaching } 
  else if (currentMode == 'B') {
    LineIR(DEST_B);
    currentMode = 'S';  } 
  else if (currentMode == 'P') {
    if (patrolToggle) {
      LineIR(DEST_A);  } 
else {
      LineIR(DEST_B); }
    patrolToggle = !patrolToggle; // Switch destination for next loop  } 
  else {
    carStop();} }
void receiveEvent(int howMany) {
  if (Wire.available()) {
    currentMode = Wire.read();
    Serial.print("New Mode: "); Serial.println(currentMode);
    if(currentMode == 'P') patrolToggle = true; // Start patrol with Dest A}}
void LineIR(String dest) {
  int left, right;
  Serial.println("Moving to: " + dest);
  buzzer(1);
  while (true) {
    // Check if a new command came in while moving
    if (currentMode == 'S') { carStop(); return; } 
    ReadRFID();
    if (ID == dest) {
      carStop();
      Serial.println("Reached Target");
      buzzer(2);
      ID = ""; // Clear ID so it doesn't trigger immediately next time
      delay(2000); // Wait 2 seconds at destination
      return;   }
    left = digitalRead(IR_Left);
    right = digitalRead(IR_Right);
    if (left == 0 && right == 0) ffCar();
    else if (left == 1 && right == 0) rightCar();
    else if (left == 0 && right == 1) leftCar();
    delay(20); }}
void ReadRFID() {
  ID = "NON";
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); }
  content.toUpperCase();
  ID = content.substring(1);
  mfrc522.PICC_HaltA(); // Stop reading the same card }
void carStop() {
  Serial.println("Stop");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW); }
void ffCar() {
  Serial.println("Forward");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); }
void revCar() {
  Serial.println("Reverse");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); }
void leftCar() {
  Serial.println("Left");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW); }
void rightCar() {
  Serial.println("Right");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); }
void buzzer(int n) {
  int i;
  for(i=0;i<n;i++)  {
      digitalWrite(buz,HIGH);
      delay(80);
      digitalWrite(buz,LOW);
      delay(80);  }
 }
