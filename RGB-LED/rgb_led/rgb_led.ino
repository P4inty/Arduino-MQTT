#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//WiFi
const char* ssid = "";
const char* password = "";
//Mqtt
const char* mqtt_server = ""; //Define Broker Ip
const char* topic = "";
const char* client_id = "";
const char* client_usn = "";
const char* client_pwd = "";
//Pin layout
const byte redPin = D0; //Define Arduino Pins
const byte greenPin = D1;
const byte bluePin = D2;
  //Append if needed
int red;
int green;
int blue;

WiFiClient espClient;
PubSubClient client(espClient);

void setColor() {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char powerStatus = payload[0];
  if (powerStatus == '1') {
    red = ((payload[1] - '0') * 10 + (payload[2] - '0')) * 10 + (payload[3] - '0');
    green = ((payload[4] - '0') * 10 + (payload[5] - '0')) * 10 + (payload[6] - '0');
    blue = ((payload[7] - '0') * 10 + (payload[8] - '0')) * 10 + (payload[9] - '0');
  }
  if (powerStatus == '0') {
    red = 0;
    green = 0;
    blue = 0;
  }
  else {
    Serial.println("Powerstatus: " + (String)powerStatus + ", is not valid!");
  }
  setColor();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(client_id, client_usn, client_pwd)) {
      client.subscribe(topic);
    } 
    else {
      delay(5000);
    }
  }
}

void setup()
{
  client.setServer(mqtt_server, 1883); // Adjust port if needed
  client.setCallback(callback);
  //Set pins
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //Boot-sequence
  digitalWrite(redPin, HIGH);
  delay(5000);
  digitalWrite(redPin, LOW);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
