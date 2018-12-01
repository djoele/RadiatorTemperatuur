const char* ssid = "VGV7519B5207C";
const char* password = "YmgH334fLyNB";
const char* mqtt_username = "djoele";
const char* mqtt_password = "@jP6Aq03";
const IPAddress ip(192,168,2,10);
const int port = 8884;

//AANVOER_RADIATOR_KEUKEN = "46";
//RETOUR_RADIATOR_KEUKEN = "47";
//AANVOER_RADIATOR_BANK = "48";
//RETOUR_RADIATOR_BANK = "49";
//AANVOER_RADIATOR_TV = "50";
//RETOUR_RADIATOR_TV = "51";

const String devices[18]={"60:01:94:8E:B3:EA","46","AANVOER_RADIATOR_KEUKEN",
                          "60:01:94:8E:B9:2B","47","RETOUR_RADIATOR_KEUKEN",
                          "60:01:94:8E:BC:E1","48","AANVOER_RADIATOR_BANK",
                          "60:01:94:8E:B5:06","49","RETOUR_RADIATOR_BANK",
                          "60:01:94:8E:B0:B4","50","AANVOER_RADIATOR_TV",
                          "60:01:94:8E:B6:19","51","RETOUR_RADIATOR_TV"};

const char* MQTT_TOPIC_PUB = "domoticz/in";
