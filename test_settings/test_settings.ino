#include <Arduino.h>

#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
// база данных для хранения настроек
// будет автоматически записываться в файл при изменениях
GyverDBFile db(&LittleFS, "/data.db");

// указывается заголовок меню, подключается база данных
SettingsGyver sett("My Settings", &db);

// имена ячеек базы данных
DB_KEYS(
  kk,
  input);

int slider;

// билдер! Тут строится наше окно настроек
void build(sets::Builder& b) {
  b.Input(kk::input, "My input");
  b.Slider("My slider", 0, 4096, 1, "", &slider);
  
  if (b.Button("Button")) {
    Serial.print("click: ");
    Serial.println(b.build.pressed());
  }

  if (b.ButtonHold("Button Hold")) {
    Serial.print("hold: ");
    Serial.println(b.build.pressed());
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_AP_STA);

  sett.begin();
  sett.onBuild(build);

  #ifdef ESP32
  LittleFS.begin(true);
  #else
  LittleFS.begin();
  #endif

  // запуск и инициализация полей БД
  db.begin();
  db.init(kk::input, 0);

  // ======= AP =======
  WiFi.softAP("AP ESP32");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
 
}

void loop() {
  // тикер, вызывать в лупе
  sett.tick();

  static uint32_t tmrs;
  if (millis() - tmrs >= 500) {
    tmrs = millis();
    static int i;
    i++;
    //Serial.println(slider);
  }
}