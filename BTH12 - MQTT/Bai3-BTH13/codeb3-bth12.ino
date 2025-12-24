#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* device_name = "ESP32";
const char* ssid = "TP-LINK_A0EA";
const char* password = "tumotden8";

const char* mqtt_server = "192.168.0.108";
const int mqtt_port = 1883;
const char* mqtt_user = "cong1";
const char* mqtt_pass = "1234";

const char* student_id = "10123046";

WiFiClient espClient;
PubSubClient client(espClient);

/* ===== TOPIC (tự động theo student_id) ===== */
String topic_cmd;
String topic_state;
String topic_note;
String topic_lwt;

const int LED_PIN = 2;
bool ledState = false;

/* ===== LED CONTROL ===== */
void applyLed(bool on) {
  ledState = on;
#if defined(ESP8266)
  digitalWrite(LED_PIN, on ? LOW : HIGH);
#else
  digitalWrite(LED_PIN, on ? HIGH : LOW);
#endif
}

void publishStateRetained() {
  client.publish(topic_state.c_str(), ledState ? "ON" : "OFF", true);
}

/* ===== WIFI ===== */
void setup_wifi() {
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

/* ===== MQTT CALLBACK ===== */
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  
  Serial.print("MQTT [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);
  
  if (String(topic) == topic_cmd) {
    if (msg == "ON") applyLed(true);
    else if (msg == "OFF") applyLed(false);
    else if (msg == "TOGGLE") applyLed(!ledState);
    publishStateRetained();
  }
}

/* ===== MQTT RECONNECT ===== */
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP-Lab3-";
    clientId += student_id;
    Serial.print("Connecting MQTT... ");
    
    if (client.connect(
      clientId.c_str(),
      mqtt_user,
      mqtt_pass,
      topic_lwt.c_str(),
      0,
      true,
      "OFFLINE"
    )) {
      Serial.println("connected");
      client.subscribe(topic_cmd.c_str());
      Serial.print("Subscribed ");
      Serial.println(topic_cmd);
      
      client.publish(topic_note.c_str(),
        "ESP BOOT / RECONNECT (note not retained)",
        false);
      client.publish(topic_lwt.c_str(), "ONLINE", true);
      publishStateRetained();
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(1500);
    }
  }
}

/* ===== SETUP ===== */
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  applyLed(false);
  
  /* Build topics */
  topic_cmd = "iot/lab3/" + String(student_id) + "/cmd";
  topic_state = "iot/lab3/" + String(student_id) + "/state";
  topic_note = "iot/lab3/" + String(student_id) + "/note";
  topic_lwt = "iot/lab3/" + String(student_id) + "/lwt";
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

/* ===== LOOP ===== */
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}