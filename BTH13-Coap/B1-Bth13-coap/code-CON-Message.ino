#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

const char* ssid = "TP-LINK_A0EA";
const char* password = "tumotden8";

IPAddress coap_server_ip(192, 168, 0, 103);

const int coap_port = 5683;

WiFiUDP udp;
Coap coap(udp);

unsigned long lastSend = 0;

#ifndef COAP_CON
#define COAP_CON (COAP_TYPE)0
#endif

#ifndef COAP_NON
#define COAP_NON (COAP_TYPE)1
#endif

void setup() {
  Serial.begin(115200);
  
  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP ESP32: "); Serial.println(WiFi.localIP());

  coap.start();
}

void loop() {
  coap.loop();

  if (millis() - lastSend >= 3000) { 
    lastSend = millis();

    String payload = "temp=" + String(random(250, 350) / 10.0);
    Serial.print("Nhiet do gui: "); Serial.println(payload);
    
    coap.send(coap_server_ip, coap_port, "sensor/temp", COAP_CON, COAP_POST, NULL, 0, (uint8_t*)payload.c_str(), payload.length());
  }
}