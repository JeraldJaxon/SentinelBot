#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "DHT.h"
#define WIFI_SSID "wifi_ssid"
#define WIFI_PASSWORD "enter wifi_password"
#define API_KEY "enter API_key"
#define DATABASE_URL " enter_url_id "
#define USER_EMAIL "gmailid@gmail.com"
#define USER_PASSWORD "enter gmai_password"
#define DHTPIN 18
#define RCWL 23
#define GAS 19
#define DHTTYPE DHT11
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
int t, h, rcwl, gas;
int pt,ot,dt=0;
String STAT="S",STAT_PRE="ss";
void Connect_WiFi();
void Firebase_Store_JSON(String PATH);
String Firebase_getString(String PATH);
void temp_read();
void SensorRead();
void timerDisplay();
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("AirQualityMon");
  lcd.setCursor(0, 1);
  lcd.print("   Robot");
  Connect_WiFi();
  lcd.clear();
  Wire.begin(21, 22); 
  lcd.print("Initializing");
  pinMode(RCWL, INPUT);
  pinMode(GAS, INPUT);
  dht.begin();
  pt=ot=millis()/100; }
void loop() {
  temp_read();
  SensorRead();
  // Send status to Arduino only if it changes to save I2C bandwidth
  if (STAT != STAT_PRE) {
    Wire.beginTransmission(8);
    if (STAT == "A") Wire.write('A');
    else if (STAT == "B") Wire.write('B');
    else if (STAT == "AB") Wire.write('P'); // 'P' for Patrol
    else Wire.write('S'); // 'S' for Stop/Wait
    Wire.endTransmission();
    STAT_PRE = STAT; }
  timerDisplay();
  delay(100); }
void Firebase_Store_JSON(String PATH) {
  lcd.clear();
  lcd.print("Uploading ...");
  FirebaseJson json;
  json.set("Alert", 222);
  json.set("MOV", (rcwl == 1) ? "Yes" : "No");
  json.set("gas", (gas == 0) ? "Alert" : "Safe"); 
  json.set("hum", String(h) + "%");
  json.set("temp", t);
  Serial.println("Updating Root Data...");
  if (Firebase.RTDB.updateNode(&fbdo, PATH, &json))  {
    Serial.println("Root Upload Success!");
    lcd.setCursor(0,1);
    lcd.print("Upload Success!"); } 
  else   {
    Serial.println("Error: " + fbdo.errorReason());
    lcd.setCursor(0,1);
    lcd.print("Error: " + fbdo.errorReason());   }
void Connect_WiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.clear();
  lcd.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); }
  Serial.println("\nConnected!");
  lcd.clear();
  lcd.print("WiFi READY");
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true); }
String Firebase_getString(String PATH) {
  if (Firebase.RTDB.getString(&fbdo, PATH)) {
    return fbdo.to<const char *>(); }
  return fbdo.errorReason().c_str(); }
void temp_read() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    h = 0; t = 0; } }
void SensorRead() {
  rcwl = digitalRead(RCWL);
  gas = digitalRead(GAS);
  String msgUpload = "RCWL:";
   if(rcwl == 1) {
      msgUpload += "Movement  Gas:";   }
   else {
      msgUpload += "noMovement  Gas:";  }
   if(gas == 1) {
      msgUpload += "Safe  Temp:"; }
   else   {
      msgUpload += "Alert  Temp:";  }
   msgUpload += String(t);
   msgUpload += "°C Hum:";
   msgUpload += String(h);
   msgUpload += "%";
   Serial.println(msgUpload); }
void timerDisplay()  {
   pt=millis()/100;
   if(pt-ot>10)   {
    ot=pt;
    dt++;
    lcd.clear();
    lcd.print(t);
    lcd.print("*c");
    lcd.setCursor(7,0);
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(14,0);
    lcd.print(dt);
    lcd.setCursor(14,1);
    lcd.print(STAT);
            if(rcwl == 1)      {
                lcd.setCursor(0,1);
                lcd.print("Mov");          }
            else         {
                lcd.setCursor(0,1);
                lcd.print("NoMov");  }
            if(gas == 1)  {
                lcd.setCursor(7,1);
                lcd.print("Safe");            }
            else            {
                lcd.setCursor(7,1);
                lcd.print("Alert");            }
      if(dt==10)      {
          dt=0;
          Firebase_Store_JSON("/");       }
      if(dt==5)      {
          lcd.clear();
          lcd.print("Getting CMD");
          String msg = Firebase_getString("/RoboticCAR/CMD");
          Serial.print("Command: ");
          Serial.println(msg); 
          if(msg == "\\\"CMD_A\\\"")         {
            STAT="A";
            lcd.setCursor(0,1);
            lcd.print("Dest-A");
            delay(1000);          }
          if(msg == "\\\"CMD_B\\\"")        {
            STAT="B";
            lcd.setCursor(0,1);
            lcd.print("Dest-B");
            delay(1000);        }
          if(msg == "\\\"CMD_AB\\\"")       {
            STAT="AB";
            lcd.setCursor(0,1);
            lcd.print("Dest-AB Patrol");
            delay(1000);          }      }   }
}

