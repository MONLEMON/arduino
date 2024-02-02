#include <ESP32Servo.h>
#define LED 16
#define BTN 22
Servo myservo;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin = 18;
int pos = 0;
void IRAM_ATTR IO_INT_ISR()
{
  // Toggle The LED
  digitalWrite(LED, !digitalRead(LED));
}
void setup() {
  myservo.attach(servoPin);
  pinMode(LED,OUTPUT);
  pinMode(BTN,INPUT);
  attachInterrupt(BTN, IO_INT_ISR, FALLING);
}
void loop() {
  if(digitalRead(LED) == HIGH){
  moter();
  }
}

void moter(){
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
}