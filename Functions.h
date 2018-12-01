void ConnectWifi(){
  Serial.println("Connecting to Wifi..");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

float MeetTemperatuur(){
  float temperature = 0;
  //doe temperatuur meting
  Serial.print("AnalogRead(0): ");
  Serial.println(analogRead(0));
  Serial.print("Voltage in mV: ");
  Serial.println(analogRead(0)*(3300/1024.0));
  temperature = ((analogRead(0)*(3300/1024.0))-400) / (195.3/10);
  Serial.print("Gemeten temperatuur: ");
  Serial.println(float(temperature));
  return temperature;
}

int DetermineEntryInArray(){
  String deviceID = "-1";
  String mac = WiFi.macAddress();
  Serial.println("MAC-adres: " + mac);

  boolean check = false;
  for (int i = 0; i < 18; i++) {
      if (devices[i] == mac) {
          Serial.println("Device found on entry: " + i);
          //deviceID = devices[i+1];
          check = true;
          return i;
      }
 }
 if (check == false){
     Serial.println("Device not found...");
 }
}

String DetermineDeviceIDForDomoticz(int entry){
  return devices[entry+1];
}

String DetermineMQTTIDForDomoticz(int entry){
  return devices[entry+2];
}

void setupMqtt(){
  
  // Setup MqttClient
  MqttClient::System *mqttSystem = new System;
  MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
  MqttClient::Network * mqttNetwork = new MqttClient::NetworkClientImpl<WiFiClient>(network, *mqttSystem);
  //// Make 128 bytes send buffer
  MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
  //// Make 128 bytes receive buffer
  MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
  //// Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
  //// Configure client options
  MqttClient::Options mqttOptions;
  ////// Set command timeout to 60 seconds
  mqttOptions.commandTimeoutMs = 60000;
  //// Make client object
  mqtt = new MqttClient(
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers
  );
}

void PublishTemperatureToMqtt(String deviceID, String mqttID, float temperature){
  // Check connection status
  if (!mqtt->isConnected()) {
    // Close connection if exists
    network.stop();
    // Re-establish TCP connection with MQTT broker
    Serial.println("Connecting");
    network.connect(ip, port);
    if (!network.connected()) {
      Serial.println("Can't establish the TCP connection");
      //ESP.reset();
    }
    // Start new MQTT connection
    MqttClient::ConnectResult connectResult;
    // Connect
    {
      MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
      options.MQTTVersion = 4;
      options.clientID.cstring = const_cast<char*>(mqttID.c_str());;
      options.cleansession = false;
      options.keepAliveInterval = 120; // 120 seconds
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        Serial.printf("Connection error: %i", rc);
        return;
      }
      Serial.println("MqttClient is nu connected...");
    }
    }
    {
      //Temperatuur omzetten van float naar String
      float temp = temperature;
      char temperatuur[5];
      dtostrf(temp, 3, 2, temperatuur);
      const char* buf = temperatuur;

      //Message payload maken
      String mess = "{ \"idx\" : " + deviceID + ", \"nvalue\" : 0, \"svalue\" : \"" + buf + "\"}";
      Serial.println("Message: " + mess);
      
      MqttClient::Message message;
      message.qos = MqttClient::QOS0;
      message.retained = false;
      message.dup = false;
      message.payload = (void*) mess.c_str();
      message.payloadLen = strlen(mess.c_str());
      mqtt->publish(MQTT_TOPIC_PUB, message);
    }
    // Idle for 1 seconds
    mqtt->yield(1000L);
}
