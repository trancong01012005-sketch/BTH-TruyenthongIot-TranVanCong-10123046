#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

const char* ssid = "TP-LINK_A0EA";
const char* password = "tumotden8";

WiFiUDP udp;
Coap coap(udp);

/* Callback khi nhận từng block */
void callback(CoapPacket &packet, IPAddress ip, int port) {
  Serial.print("Received block, size = ");
  Serial.println(packet.payloadlen);

  // In dữ liệu nhận được
  for (int i = 0; i < packet.payloadlen; i++) {
    Serial.print((char)packet.payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  /* Kết nối WiFi */
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());

  /* Khởi động CoAP */
  coap.response(callback);
  coap.start();

  /* Gửi request firmware */
  coap.get(
    IPAddress(192,168,0,103), 5683,"firmware");
}

void loop() {
  coap.loop();
}
