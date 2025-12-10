#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(2000);
} 

void loop() {
  delay(2000);
  float doam = dht.readHumidity();
  float nhietdo = dht.readTemperature();
  
  // Gửi JSON qua Serial
  Serial.print("{\"temp\":");
  Serial.print(nhietdo);
  Serial.print(",\"hum\":");
  Serial.print(doam);
  Serial.println("}");
}