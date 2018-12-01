#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MqttClient.h>
#include "Constants.h"
#include "Mqtt.h"
#include "Functions.h"

String deviceID = "-1";
String mqttID = "";

void setup() {
  Serial.begin(115200); 
  ConnectWifi();
  setupMqtt();
  
  int id = DetermineEntryInArray();
  if (id > -1){
    deviceID = DetermineDeviceIDForDomoticz(id);
    Serial.println("deviceID: " + deviceID);
    mqttID = DetermineMQTTIDForDomoticz(id);
    Serial.println("mqttID: " + mqttID);
  } 

  
}

float temperature;

void loop() {
  temperature = MeetTemperatuur();
  PublishTemperatureToMqtt(deviceID, mqttID, temperature);
  delay(5000);
}
