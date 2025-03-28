#include <GyverOLED.h>
//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER, OLED_SPI, 8, 7, 6> oled;
GyverOLED<SSH1106_128x64> oled;

int width = 72;
int height = 40;
int xOffset = 30; // = (132-w)/2 
int yOffset = 12; // = (64-h)/2 

char Lorem_ipsum[] = "Lorem ipsum dolor sit amet, лорем ипсум долор сит амет привет народ ё, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip...";
void setup() {
  Serial.begin(115200);
  oled.init(5, 6);   
}

void loop() {
  
  for (int i = 1; i < width - 2; i++) {
    oled.clear();  
    
    int y_end = yOffset + height - 1;
    oled.rect(xOffset, yOffset, xOffset + i, y_end, OLED_STROKE);
    
    oled.setCursorXY(xOffset + 5, yOffset + 5);
    oled.textMode(BUF_ADD);
    oled.print(i);
    oled.print(" x ");
    oled.print(y_end - yOffset);
    //oled.dot(x, 13, 1);
    oled.update();

    Serial.print("RECT :  x ");
    Serial.print(xOffset);
    Serial.print(" y: ");
    Serial.print(yOffset);
    Serial.print("  x_ : ");
    Serial.print(xOffset + i);
    Serial.print(" y_ : ");
    Serial.println(y_end);

    delay(300);

  }
  
  delay(1000);
  
  
  //printScale(1);
  /*
  printScale(2);
  printScale(3);
  printScale(4);
  overlapTest();
  printTest();
  party();
  showText();
  scrollText();
  scaleText();
  */
}

void printScale(byte x) {
  oled.clear();
  oled.setScale(x);
  for (byte i = 0; i < 8; i += x) {
    oled.setCursor(0, i);
    oled.print("Hello!");
  }
  oled.update();
  delay(1000);
}

void party() {
  oled.clear();
  uint32_t tmr = millis();
  oled.setScale(3);
  oled.setCursor(10, 2);
  oled.print("ПЕННАЯ");
  oled.setScale(2);
  oled.setCursor(6, 5);
  oled.print("ВЕЧЕРИНКА!");
  oled.update();
  for (;;) {
    oled.invertDisplay(true);
    delay(200);
    oled.invertDisplay(false);
    delay(200);
    if (millis() - tmr > 5000) return;
  }
}

void overlapTest() {
  oled.clear();
  oled.setScale(1);
  oled.setCursorXY(0, 0);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(0, 8);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setCursorXY(40, 4);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(40, 12);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setScale(2);
  oled.setCursorXY(0, 24);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(0, 40);
  oled.print("KEK!");
  oled.update();
  delay(500);

  oled.setCursorXY(60, 28);
  oled.print("LOL");
  oled.update();
  delay(500);
  oled.setCursorXY(60, 44);
  oled.print("KEK!");
  oled.update();
  delay(5000);
}

void printTest() {
  oled.clear();
  char data0[] = "Привет!";
  char data1 = 'b';
  int data2 = -50;
  float data3 = 1.25;
  oled.home();
  oled.setScale(1);
  oled.println(data0);
  oled.println(data1);
  oled.invertText(true);
  oled.println(data2);
  oled.println(data3);
  oled.invertText(false);
  oled.update();
  delay(5000);
}

void showText() {
  oled.clear();
  oled.home();
  oled.autoPrintln(true);
  oled.setScale(1);
  oled.print(Lorem_ipsum);
  oled.update();
  delay(5000);
}

void scrollText() {
  oled.clear();
  uint32_t tmr = millis();
  oled.autoPrintln(false);
  int val = 128;
  for (;;) {
    oled.clear();   // ЗАКОММЕНТИРУЙ, ЕСЛИ ВКЛЮЧЕН БУФЕР
    oled.setScale(1);
    oled.setCursor(val, 0);
    oled.print(Lorem_ipsum);

    oled.setScale(2);
    oled.setCursor(val, 1);
    oled.print(Lorem_ipsum);

    oled.setScale(3);
    oled.setCursor(val, 4);
    oled.print(Lorem_ipsum);
    oled.update();
    val--;
    if (millis() - tmr > 5000) return;
  }
}

void scaleText() {  
  uint32_t tmr = millis();
  byte scale = 1;
  oled.autoPrintln(true);
  for (;;) {
    oled.clear();
    oled.setScale(scale);
    oled.home();
    oled.print("Привет!");
    oled.update();
    delay(1000);
    oled.clear();
    scale++;
    if (scale > 4) scale = 1;
    if (millis() - tmr > 5000) return;
  }
}
