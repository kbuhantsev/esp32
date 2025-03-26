#define RED_PIN   9   // Пин для красного канала
#define GREEN_PIN 10  // Пин для зелёного канала
#define BLUE_PIN  11  // Пин для синего канала
#define BUTTON_PIN 2  // Пин кнопки

int effect = 0;   // Текущий эффект
bool buttonState = HIGH;  
bool lastButtonState = HIGH;

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW && lastButtonState == HIGH) {  
        effect = (effect + 1) % 10;  // Переключение эффекта
        delay(50);  // Антидребезг
    }
    lastButtonState = buttonState;

    switch (effect) {
        case 0: solidColor(255, 0, 0); break; // Красный
        case 1: solidColor(0, 255, 0); break; // Зелёный
        case 2: solidColor(0, 0, 255); break; // Синий
        case 3: rainbow(); break;
        case 4: pulse(255, 0, 255); break; // Фиолетовый пульс
        case 5: blink(255, 255, 0); break; // Жёлтое мерцание
        case 6: colorFade(); break;
        case 7: runningLight(); break;
        case 8: strobe(255, 255, 255); break; // Белый стробоскоп
        case 9: randomFlash(); break;
    }
}

// Заполняет ленту одним цветом
void solidColor(int r, int g, int b) {
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
}

// Плавное изменение цветов
void rainbow() {
    for (int i = 0; i < 256; i++) {
        analogWrite(RED_PIN, sin(i * 0.024) * 127 + 128);
        analogWrite(GREEN_PIN, sin((i + 85) * 0.024) * 127 + 128);
        analogWrite(BLUE_PIN, sin((i + 170) * 0.024) * 127 + 128);
        delay(30);
    }
}

// Плавное мигание
void pulse(int r, int g, int b) {
    for (int i = 0; i < 255; i++) {
        analogWrite(RED_PIN, r * i / 255);
        analogWrite(GREEN_PIN, g * i / 255);
        analogWrite(BLUE_PIN, b * i / 255);
        delay(5);
    }
    for (int i = 255; i > 0; i--) {
        analogWrite(RED_PIN, r * i / 255);
        analogWrite(GREEN_PIN, g * i / 255);
        analogWrite(BLUE_PIN, b * i / 255);
        delay(5);
    }
}

// Мерцание случайными цветами
void randomFlash() {
    solidColor(random(256), random(256), random(256));
    delay(200);
}

// Бегущий огонёк (эффект волны)
void runningLight() {
    for (int i = 0; i < 3; i++) {
        analogWrite(RED_PIN, i == 0 ? 255 : 0);
        analogWrite(GREEN_PIN, i == 1 ? 255 : 0);
        analogWrite(BLUE_PIN, i == 2 ? 255 : 0);
        delay(200);
    }
}

// Стробоскоп
void strobe(int r, int g, int b) {
    for (int i = 0; i < 10; i++) {
        solidColor(r, g, b);
        delay(50);
        solidColor(0, 0, 0);
        delay(50);
    }
}

// Мигание цветами
void blink(int r, int g, int b) {
    solidColor(r, g, b);
    delay(500);
    solidColor(0, 0, 0);
    delay(500);
}

// Плавная смена цветов
void colorFade() {
    for (int i = 0; i < 255; i++) {
        analogWrite(RED_PIN, i);
        analogWrite(GREEN_PIN, 255 - i);
        analogWrite(BLUE_PIN, i / 2);
        delay(10);
    }
}