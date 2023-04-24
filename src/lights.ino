
/*--------------------------- Configuration ------------------------------*/
// Configuration should be done in the included file:
#include "config.h"
//#include "mqtt.ino"
#include "effects.h"
#include "hardware.h"

/*--------------------------- Libraries ----------------------------------*/
#include <ESP8266WiFi.h>              // ESP8266 WiFi driver
#include <PubSubClient.h>             // Required for MQTT

/*--------------------------- Global Variables ---------------------------*/
// Wifi
#define WIFI_CONNECT_INTERVAL           500  // Wait 500ms intervals for wifi connection
#define WIFI_CONNECT_MAX_ATTEMPTS        10  // Number of attempts/intervals to wait

// General
uint32_t g_device_id;                    // Unique ID from ESP chip ID
uint32_t g_device_id2;                    // Unique ID from ESP chip ID
uint32_t chipId = ESP.getChipId();
String deviceID = String(chipId);

// MQTT
char g_mqtt_message_buffer[255];      // General purpose buffer for MQTT messages
char g_power_topic[50];             // MQTT topic for receiving commands
char g_effect_topic[50];             // MQTT topic for receiving commands
char g_status_topic[50];             // MQTT topic for receiving commands
char g_estatus_topic[50];             // MQTT topic for receiving commands

/// MQTT call back
byte* buffer;
boolean Rflag=false;
int r_len;
char* Topic;

// Global variable to store the current effect
int currentEffect = -1; // -1 indicates no effect is currently running
int lastEffect = 0; // This will be used if someone sends the ON command - on reboot it will be set to Combination


/*--------------------------- Function Signatures ------------------------*/

void mqttCallback(char* topic, byte* payload, uint8_t length);
void sendstatus();


/*--------------------------- Instantiate Global Objects -----------------*/
// MQTT
WiFiClient esp_client;
PubSubClient client(esp_client);

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.print("starting up, v");

  // We need a unique device ID for our MQTT client connection
  g_device_id = ESP.getChipId();  // Get the unique ID of the ESP8266 chip
  Serial.print("Device ID: ");
  Serial.println(g_device_id, HEX);


  // Connect to WiFi
  Serial.println("Connecting to WiFi");
  if (initWifi())
  {
    Serial.println("WiFi connected");
  } else {

    Serial.println("WiFi FAILED");
  }

  delay(100);

  /* Set up the MQTT client */
  client.setServer(mqtt_broker, 1883);
  client.setCallback(mqttCallback);
  client.setBufferSize(255);

  sprintf(g_power_topic, "cmnd/%x/power", ESP.getChipId());  // For receiving commands
  sprintf(g_effect_topic, "cmnd/%x/effect", ESP.getChipId());  // For receiving commands
  sprintf(g_estatus_topic, "tele/%x/effect", ESP.getChipId());  // For receiving commands
  sprintf(g_status_topic, "tele/%x/status", ESP.getChipId());  // For receiving commands


  pinMode(ENABLE_A, OUTPUT);
  pinMode(INPUT_1, OUTPUT);
  pinMode(INPUT_2, OUTPUT);

  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);
}

void loop()
{

      if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      reconnectMqtt();
    }
  }

  client.loop();

// Check if there is a current effect to run
  if (currentEffect != -1) {
    // Run the current effect
    switch (currentEffect) {
      case 0:
        combination(client);
        break;
      case 1:
        inWaves(client);
        break;
      case 2:
        sequential(client);
        break;
      case 3:
        slowGlo(client);
        break;
      case 4:
        chasingFlash(client);
        break;
      case 5:
        twinkleFlash(client);
        break;
      case 6:
        steadyOn(client);
        break;
      case 7:
        slowFadeAlternate(client);
        break;
    }
  } else {
    // No current effect, wait for a command
    delay(100);
  }

 
}


/**
  Connect to Wifi. Returns false if it can't connect.
*/
bool initWifi()
{
  // Clean up any old auto-connections
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
  }
  WiFi.setAutoConnect(false);

  // RETURN: No SSID, so no wifi!
  if (sizeof(ssid) == 1)
  {
    return false;
  }

  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait for connection set amount of intervals
  int num_attempts = 0;
  while (WiFi.status() != WL_CONNECTED && num_attempts <= WIFI_CONNECT_MAX_ATTEMPTS)
  {
    delay(WIFI_CONNECT_INTERVAL);
    num_attempts++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  } else {
    return true;
  }
}

/**
  Reconnect to MQTT broker, and publish a notification to the status topic
*/
void reconnectMqtt() {
  char mqtt_client_id[20];
  sprintf(mqtt_client_id, "esp8266-%x", ESP.getChipId());

  // Loop until we're reconnected
  while (!client.connected())
  {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password))
    {
      //Serial.println("connected");
      // Once connected, publish an announcement
      sprintf(g_mqtt_message_buffer, "Device %s starting up", mqtt_client_id);
      client.publish(status_topic, g_mqtt_message_buffer);
      // Resubscribe
      client.subscribe(g_power_topic);
      client.subscribe(g_effect_topic);

    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void startLastEffect(){}
void getStatus(String deviceID) {
  // function code here
}


String getDeviceID(String topic) {
  // Extract the device ID from the topic
  int startIndex = topic.indexOf("cmnd/") + 5;
  int endIndex = topic.indexOf("/", startIndex);
  return topic.substring(startIndex, endIndex);
}

void sendstatus(String payload){
  
  lastEffect = currentEffect;
  sprintf(g_mqtt_message_buffer, "Powered On");
  client.publish(g_status_topic, g_mqtt_message_buffer);

  sprintf(g_mqtt_message_buffer, "Current effect is: %s", payload.c_str());
  client.publish(g_estatus_topic, g_mqtt_message_buffer);

}

void handleEffectCommand(String deviceID, String payload) {
  // Handle the effect command
    if (payload == "Combination") {
        currentEffect = 0;
    } else if (payload == "In Waves") {
        currentEffect = 1;
    } else if (payload == "Sequential") {
        currentEffect = 2;
    } else if (payload == "Slo Glo") {
        currentEffect = 3;
    } else if (payload == "Chasing /Flash") {
        currentEffect = 4;
    } else if (payload == "Twinkle") {
        currentEffect = 5;
    } else if (payload == "SteadyOn") {
        currentEffect = 6;
    } else if (payload == "Slow Fade Alternate") {
        currentEffect = 7;
    }
    if(currentEffect != -1){
      sendstatus(payload);
    }
}

void handlePowerCommand(String deviceID, String payload) {
  // Handle the power command
  if (payload == "ON") {
    // Power on will start the last effect or default
    currentEffect = lastEffect;
    sprintf(g_mqtt_message_buffer, "Powered On");
    client.publish(g_status_topic, g_mqtt_message_buffer);
  } else if (payload == "OFF") {
    currentEffect = -1;
    powerOff(client);
    sprintf(g_mqtt_message_buffer, "Powered Off");
    client.publish(g_status_topic, g_mqtt_message_buffer);
  }
}

void publishStatus(String deviceID) {
  // Publish the device status
  //String status = getStatus(deviceID);
  String status = "test";

  //client.publish(String("stat/") + deviceID + "/status", status);

  //sprintf(g_mqtt_message_buffer, " %deviceID");
 // client.publish(status_topic, g_mqtt_message_buffer);
}

void mqttCallback(char* topic, byte* payload, uint8_t length) {
  
  
  String topicStr = String(topic);
  //String payloadStr = String((char*)payload);
  // Create a char array of the same size as the payload
  char payloadBuf[length + 1];
  // Copy the payload into the char array
  memcpy(payloadBuf, payload, length);
  // Null terminate the char array
  payloadBuf[length] = '\0';
  // Create a String object from the char array
  String payloadStr(payloadBuf);
  
  Serial.print("Message arrived [");
  Serial.print(topicStr);
  Serial.print(" - ");
  Serial.print(payloadStr);
  Serial.print("] ");
  Serial.println();

  // Extract the device ID from the topic
  //String deviceID = getDeviceID(topicStr);

  // Handle different topics and payloads
  if (topicStr.endsWith("/effect")) {
    handleEffectCommand(deviceID, payloadStr);
  } else if (topicStr.endsWith("/power")) {
    handlePowerCommand(deviceID, payloadStr);
  } else if (topicStr.endsWith("/status")) {
    publishStatus(deviceID);
  }
}
