#include <WiFi.h>
#include "time.h"
#include <LCD_I2C.h>
#include <MQTT.h>


#define LED 23
#define relay 18
#define ldr 34
// const char ssid[] = "Bannaifun";
// const char password[] = "pondkub1412";

const char ssid[] = "Raysir";
const char password[] = "1234567890";

const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="group21/command";
const char mqtt_client_id[]="arduino_group_21"; // must change this string to a unique value
int MQTT_PORT=1883;

const char ntpServer[] = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;
const int ON_HOUR = 12;
const int OFF_HOUR = 14;
int  ldr_var =0;
int tree = 0;

WiFiClient net;
MQTTClient client;
LCD_I2C lcd(0x27, 16, 2);\
struct tm timeinfo;


void connect(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("\nconnecting...");

  while (!client.connect(mqtt_client_id)) {  
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  client.subscribe(mqtt_topic);
}

void setup(){
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(ldr,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(relay,OUTPUT);
  
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);
  connect();
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(payload == "Water"){
    lcd.print("water the plants");
    digitalWrite(relay,HIGH);
    lcd.clear();
    lcd.print("Success");
    client.publish(mqtt_topic,"STOP!!");
  }
  if(payload == "Tree1"){
    lcd.print("Tree 1");
    tree = 1;
    client.publish(mqtt_topic,"Tree 1");
  }
  if(payload == "Tree2"){
    lcd.print("Tree 2");
    tree = 2;
    client.publish(mqtt_topic,"Tree 2");

  }
}

void loop(){
  client.loop();
  if (!client.connected()) {
    connect();
  }
  lcd.clear();
  printLocalTime();
  Light();
  
}

void printLocalTime(){
  
  
  if(!getLocalTime(&timeinfo)){
    lcd.println("Failed to obtain time");
    return;
  }
  lcd.print("Time:");
  lcd.print(&timeinfo,"%H:%M:%S");
  lcd.setCursor(0, 1);
  
  Serial.print("Time:");
  Serial.print(&timeinfo,"%H:%M:%S");
  Serial.println(" ");
  delay(1000);
}

void Light(){
  ldr_var = analogRead(ldr);
  Serial.println(ldr_var);
  if(time(nullptr) < 1617460172){
    if(tree == 1){
      if(ldr_var < 200){
      digitalWrite(LED,HIGH);
      }else if(ldr_var > 600){
      digitalWrite(LED,LOW);
      }
    }else if(tree == 2){
      if(ldr_var < 400){
      digitalWrite(LED,HIGH);
      }else if(ldr_var > 750){
      digitalWrite(LED,LOW);
      }
    }
  }
}