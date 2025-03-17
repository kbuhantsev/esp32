#include <Arduino.h>
#include <GyverNTP.h>
#include <WiFi.h>

const char* ssid = "STVOL_OFFICE";
const char* password = "stvolodessa";

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  NTP.begin(2);                        // запустить и указать часовой пояс
  NTP.setPeriod(60);                   // период синхронизации в секундах
  NTP.updateNow();                     // обновить прямо сейчас

}

void loop() {
  
  /*
  if (NTP.tick()) {
    Serial.println(NTP.toString());
  }
  */

  if (NTP.updateNow()){
    Serial.println(NTP.toString());
  }
  delay(5000);

}
