/* ----------------- General config -------------------------------- */
/* WiFi */
const char* ssid                  = "IoTFi";     // Your WiFi SSID
const char* password              = "";     // Your WiFi password

/* MQTT */
const char* mqtt_broker           = "10.2.3.15"; // IP address of your MQTT broker
const char* mqtt_username         = "";              // Your MQTT username
const char* mqtt_password         = "";              // Your MQTT password
#define     REPORT_MQTT_SEPARATE  true               // Report each value to its own topic
#define     REPORT_MQTT_JSON      true               // Report all values in a JSON message
const char* status_topic          = "events";        // MQTT topic to report startup

/* Serial */
#define     SERIAL_BAUD_RATE    115200               // Speed for USB serial console


