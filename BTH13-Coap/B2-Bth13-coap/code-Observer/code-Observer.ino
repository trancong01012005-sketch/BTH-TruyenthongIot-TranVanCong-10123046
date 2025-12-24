#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

/* ===== WIFI CONFIG ===== */
const char* ssid = "DYP05";
const char* password = "12344321";

/* ===== SERVER CONFIG ===== */
IPAddress serverIP(192, 168, 1, 114);
const int serverPort = 5683;
const int clientPort = 5683;

/* ===== COAP ===== */
WiFiUDP udp;
Coap coap(udp);

bool observerRegistered = false;

/* ===== CALLBACK NHẬN NOTIFY ===== */
void notifyCallback(CoapPacket &packet, IPAddress ip, int port) {
  char payload[packet.payloadlen + 1];
  memcpy(payload, packet.payload, packet.payloadlen);
  payload[packet.payloadlen] = '\0';

  Serial.print("📩 Notify from server: ");
  Serial.println(payload);
}

/* ===== CALLBACK RESPONSE ĐĂNG KÝ ===== */
void responseCallback(CoapPacket &packet, IPAddress ip, int port) {
  char payload[packet.payloadlen + 1];
  memcpy(payload, packet.payload, packet.payloadlen);
  payload[packet.payloadlen] = '\0';

  if (!observerRegistered) {
    Serial.print("✅ Observer registered, initial value: ");
    Serial.println(payload);
    observerRegistered = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n=== ESP32 CoAP Observer Client ===");

  /* ===== WIFI CONNECT ===== */
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  /* ===== START COAP ===== */
  coap.start(clientPort);

  coap.response(responseCallback);
  coap.server(notifyCallback, "sensor/temp/notify");

  delay(1000);

  /* ===== REGISTER OBSERVER ===== */
  Serial.println("📡 Register observer...");
  coap.put(serverIP, serverPort,
           "sensor/temp/observe",
           "register");

  Serial.println("⏳ Waiting for notifications...\n");
}

void loop() {
  coap.loop();
}
