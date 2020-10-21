#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "./secrets.h"

//report no more than once every 60 minutes
const int time_between_notifications = 3600000;

//the last time we notified
unsigned long last_notification_time = 0;

//has there been motion since last time we notified?
bool motion_since_last_time = false;

// the pin that the sensor is attached to
int sensor = D2; 

void setup() {
  Serial.begin(9600); // initialize serial
  send_message_to_slack("Mo has started up!");
  pinMode(sensor, INPUT); // initialize sensor as an input
  pinMode(LED_BUILTIN, OUTPUT);
}

void send_message_to_slack(String message) {
  if (WiFi.status() != WL_CONNECTED) {
    connect_to_wifi();
  }

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClientSecure client;
    client.setInsecure();

    if (!client.connect("hooks.slack.com", 443)) {
      Serial.println("connection failed");
      return;
    }
    
    //Slack webhook example.
    String payload = String("POST ") + slack_endpoint + " HTTP/1.1\r\n" +
                 "Host: " + "hooks.slack.com" + "\r\n" +
                 "User-Agent: Motion/1.0\r\n" +
                 "Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + String(message.length() + 11) + "\r\n\r\n" +
                 "{\"text\":\"" + message + "\"}";

     Serial.println(payload);

     client.print(payload);
  }
  else {
    Serial.println("Wifi not connected?");
  }
}

void connect_to_wifi() {
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {

  int val = digitalRead(sensor);
  Serial.println(val);
  if (val == HIGH) { // check if the sensor is HIGH
    digitalWrite(LED_BUILTIN, LOW);   // Arduino: turn the LED on
    motion_since_last_time = true;
  }
  else {
     digitalWrite(LED_BUILTIN, HIGH);   // Arduino: turn the LED off
  }

  unsigned long cur_time = millis();

  if ((cur_time < last_notification_time) || (cur_time > (last_notification_time + time_between_notifications))) {
    Serial.println("Sending the message...");
    if (motion_since_last_time) {
      send_message_to_slack("Motion detected recently");
    }
    else {
      send_message_to_slack("No motion detected recently");
    }
    motion_since_last_time = false;
    last_notification_time = millis();
  }

  delay(1000);
  
}
