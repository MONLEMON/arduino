#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MQTT.h>

#define LED 17
#define Relay 5
#define ldr 34

const char ssid[] = "Raysir";
const char password[] = "1234567890";

// const char ssid[] = "Bannaifun";
// const char password[] = "pondkub1412";
const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="group21/command";
const char mqtt_client_id[]="arduino_group_21"; // must change this string to a unique value
int MQTT_PORT=1883;

WiFiClient net;
MQTTClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int tree = 1;
int Fire =1;
int  ldr_var =0;

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

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(payload == "Water"){
    digitalWrite(Relay,HIGH);
    delay(3000);
    digitalWrite(Relay,LOW);
    client.publish(mqtt_topic,"STOP!!");
  }
  if(payload == "Tree1"){
    tree = 1;
    Fire = 1;
    client.publish(mqtt_topic,"Preset บันไดเศรษฐี");
  }
  if(payload == "Tree2"){
    tree = 2;
    Fire =1;
    client.publish(mqtt_topic,"Preset เบบี้ซันโรส");

  }if(payload == "Light On"){
    Fire = 2;
    digitalWrite(LED,HIGH);
    client.publish(mqtt_topic,"LED ON");        
  }if(payload == "Light Off"){
    Fire = 2;
    digitalWrite(LED,LOW);
    client.publish(mqtt_topic,"LED Off");        
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ldr,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(Relay,OUTPUT);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);
  connect();

  timeClient.begin();
}

void loop() {
  client.loop();
  timeClient.update();
  timeup();
  water();
  if(Fire == 1 ){
  Light();
  }
  
}
void water(){
  unsigned long currentTime = timeClient.getEpochTime();
currentTime += 25200;
  // get the current time in hours, minutes, and seconds
int currentHour = (currentTime % 86400L) / 3600;
int currentMinute = (currentTime % 3600) / 60;
int currentSecond = currentTime % 60;

if(tree == 1){
  if (currentHour == 7 && currentMinute == 0 && currentSecond == 0) {
    digitalWrite(Relay,HIGH);
    delay(3000);
    digitalWrite(Relay,LOW);
  }if (currentHour == 18 && currentMinute == 0 && currentSecond == 30) {
    digitalWrite(Relay,HIGH);
    delay(3000);
    digitalWrite(Relay,LOW);
  } 
}
if(tree == 2){
  if (currentHour == 12 && currentMinute == 0 && currentSecond == 0) {
    digitalWrite(Relay,HIGH);
    delay(5000);
    digitalWrite(Relay,LOW);
  } 
}    
   
}
void timeup(){
  unsigned long currentTime = timeClient.getEpochTime();
currentTime += 25200;
  // get the current time in hours, minutes, and seconds
int currentHour = (currentTime % 86400L) / 3600;
int currentMinute = (currentTime % 3600) / 60;
int currentSecond = currentTime % 60;
  // check if the current time is equal to a specific time
  if (currentHour == 14 && currentMinute == 26 && currentSecond == 0) {
    Serial.println("The current time is 12:30:00 PM!");
  }
  Serial.print(currentHour);
  Serial.print(":");
  Serial.print(currentMinute);
  Serial.print(":");
  Serial.println(currentSecond);  
  delay(1000);
}
void Light(){
  ldr_var = analogRead(ldr);
  Serial.println(ldr_var);
    if(tree == 1){
      if(ldr_var < 200){
      digitalWrite(LED,HIGH);
      }else if(ldr_var > 750){
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