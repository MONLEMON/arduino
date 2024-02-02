#include <Time.h>

const int PUMP_PIN = 10; // Water pump pin
const int ON_HOUR = 8;   // Hour to turn on water pump (24-hour format)
const int OFF_HOUR = 18; // Hour to turn off water pump (24-hour format)

void setup() {
  pinMode(PUMP_PIN, OUTPUT);  // Set water pump pin as output
  Serial.begin(9600);         // Initialize serial communication
  setTime(10, 0, 0, 23, 2, 2023); // Set the initial time in the format (hour, minute, second, day, month, year)
}

void loop() {
  time_t now = now();  // Get the current time as a UNIX timestamp

  // Convert the UNIX timestamp to a tm struct
  tm *t = localtime(&now);

  // Check if the current hour is within the specified range to turn on or off the water pump
  if (t->tm_hour >= ON_HOUR && t->tm_hour < OFF_HOUR) {
    digitalWrite(PUMP_PIN, HIGH);  // Turn on water pump
  } else {
    digitalWrite(PUMP_PIN, LOW);   // Turn off water pump
  }

  Serial.print("Current Time: ");
  Serial.print(t->tm_year + 1900);
  Serial.print('/');
  Serial.print(t->tm_mon + 1);
  Serial.print('/');
  Serial.print(t->tm_mday);
  Serial.print(' ');
  Serial.print(t->tm_hour);
  Serial.print(':');
  Serial.print(t->tm_min);
  Serial.print(':');
  Serial.print(t->tm_sec);
  Serial.println();
  
  delay(1000);   // Wait for 1 second before taking another reading
}