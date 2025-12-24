#include <WiFi.h>  // ESP32 dùng WiFi.h thay vì ESP8266WiFi.h
#include <WiFiUdp.h>
#include <coap-simple.h>

// ===== CẤU HÌNH =====
const char* ssid = "TP-LINK_A0EA";
const char* password = "tumotden8";
IPAddress serverIP(192, 168, 0, 103);
const int serverPort = 5683;

// ===== KHỞI TẠO =====
WiFiUDP udp;
Coap coap(udp);

// ===== CALLBACK NHẬN RESPONSE =====
void callback(CoapPacket &packet, IPAddress ip, int port) {
    char payload[packet.payloadlen + 1];
    memcpy(payload, packet.payload, packet.payloadlen);
    payload[packet.payloadlen] = '\0';
    
    Serial.print("✓ Response: ");
    Serial.println(payload);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║   CoAP Client - PULL Mode (GET)       ║");
    Serial.println("╚════════════════════════════════════════╝\n");
    
    // Kết nối WiFi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n✅ WiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    // Khởi động CoAP
    coap.response(callback);
    coap.start();
    
    Serial.println("\n⏳ Sending GET requests every 2 seconds...\n");
}

void loop() {
    // Gửi GET request định kỳ
    Serial.print("→ Sending GET to server... ");
    coap.get(serverIP, serverPort, "sensor/temp");
    
    delay(2000);  // Đợi 2 giây
}