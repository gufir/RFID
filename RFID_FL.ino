#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid     = "ASDE";
const char* password = "1234566678";

// Replace with your MQTT Broker server ip
const char* mqtt_server = "192.168.43.64";
const char* mqtt_user ="pi";
const char* mqtt_password="raspberry";
const char* mqtt_topic = "RFID1";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client",mqtt_user,mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  DynamicJsonDocument doc(1024);
  doc["RFID_1"] = "Status_RFID1";
  doc["RFID_2"] = "Status_RFID2";
  doc["LAMP_1"] = "Status_Lamp1";
  doc["LAMP_2"] = "Status_Lamp2";
  String jsonString;
  serializeJson(doc, jsonString);
  Serial.println(jsonString);
  delay(3000);

  client.publish(mqtt_topic,jsonString.c_str(),1);
  
}
