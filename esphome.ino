#include <WiFi.h>
#include <PubSubClient.h>
#include "NimBLEDevice.h"

// WiFi credentials
const char* ssid = "HUAWEI Y9 2019";  // Replace with your WiFi SSID
const char* password = "123456789";   // Replace with your WiFi Password

// MQTT Broker
const char* mqtt_server = "192.168.43.84";  // Replace with your Raspberry Pi IP address

WiFiClient espClient;
PubSubClient client(espClient);

// BLE UUIDs
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

// BLE Client and Characteristic pointers
BLEClient* pClient1;
BLEClient* pClient2;
static BLERemoteCharacteristic* pRemoteCharacteristic1;
static BLERemoteCharacteristic* pRemoteCharacteristic2;

// BLE addresses
std::string sensorAddress1 = "A4:C1:38:3D:94:B6";
std::string sensorAddress2 = "a4:c1:38:1f:09:B3";

// Function declarations
void connectToWiFi();
void reconnect();
bool connectToServer(BLEAddress pAddress, BLEClient*& pClient, BLERemoteCharacteristic*& pRemoteCharacteristic, void(*notifyCallback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool));
static void notifyCallback1(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void notifyCallback2(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialize WiFi
  connectToWiFi();
  
  // Initialize MQTT client
  client.setServer(mqtt_server, 1883);

  // Initialize BLE
  BLEDevice::init("ESP32Client");

  // Connect to BLE sensors
  connectToServer(BLEAddress(sensorAddress1), pClient1, pRemoteCharacteristic1, notifyCallback1);
  connectToServer(BLEAddress(sensorAddress2), pClient2, pRemoteCharacteristic2, notifyCallback2);
}

void loop() {
  // Reconnect to WiFi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    connectToWiFi();
  }

  // Reconnect to MQTT if disconnected
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop(); // Maintain MQTT connection
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi");
  }
}

// Function to reconnect to MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" (try again in 5 seconds)");
      delay(5000);
    }
  }
}

// BLE callback for Sensor 1
static void notifyCallback1(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  if (length < 5) {
    Serial.println("Sensor 1: Received data length is too short");
    return;
  }

  uint16_t tempRaw = (pData[0] | (pData[1] << 8));
  float temperature = tempRaw * 0.01;
  uint16_t humidityRaw = pData[2];

  Serial.print("Sensor 1 - Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidityRaw);
  Serial.println(" %");

  // Publish data to MQTT
  String payload = "Temperature: " + String(temperature) + " C, Humidity: " + String(humidityRaw) + " %";
  client.publish("home/sensor1", payload.c_str());
}

// BLE callback for Sensor 2
static void notifyCallback2(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  if (length < 5) {
    Serial.println("Sensor 2: Received data length is too short");
    return;
  }

  uint16_t tempRaw = (pData[0] | (pData[1] << 8));
  float temperature = tempRaw * 0.01;
  uint16_t humidityRaw = pData[2];

  Serial.print("Sensor 2 - Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidityRaw);
  Serial.println(" %");

  // Publish data to MQTT
  String payload = "Temperature: " + String(temperature) + " C, Humidity: " + String(humidityRaw) + " %";
  client.publish("home/sensor2", payload.c_str());
}

// Function to connect to BLE server
bool connectToServer(BLEAddress pAddress, BLEClient*& pClient, BLERemoteCharacteristic*& pRemoteCharacteristic, void(*notifyCallback)(BLERemoteCharacteristic*, uint8_t*, size_t, bool)) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  if (!pClient->connect(pAddress)) {
    Serial.println("Failed to connect to server!");
    return false;
  }
  Serial.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");

  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");

  pRemoteCharacteristic->registerForNotify(notifyCallback);

  return true;
}
