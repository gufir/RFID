#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//MQTT Server
const char* mqtt_server = "IP Raspi";
const char* mqttt_username = "pi";
const char* mqtt_password = "raspberry"
const char* mqtt_topic1 = "RFID1";
const char* mqtt_topic2 = "RFID2";

//WiFI
const char* ssid = "ASDE";
const char* password= "1234566678";

WiFiClient espClient;
PubSubClient client(espClient);


//Setup WiFi
void wifisetup(){
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
}

//Connecting to MQTT Server
void BrokerConnect() {
  // Connect to MQTT broker
  client.setServer(mqtt_server,1883);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  wifisetup();
  BrokerConnect();
}

void loop() {
  //JSON Data
  DynamicJsonDocument doc(1024);
  doc["RFID"] = "RFID1";
  doc["Status"] = "";

  String jsonString;
  serializeJson(doc, jsonString);
  Serial.println(jsonString);

  //Publish Data
  client.publish(mqtt_topic1,jsonString.c_str(),1);
  
  
}
