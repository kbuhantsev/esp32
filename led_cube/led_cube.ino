#include <Arduino.h>
#include <FastIMU.h>
#include <Wire.h>
#include "Madgwick.h"
#include <OneButton.h>
#include <GRGB.h>

#define RED_PIN 9     // Пин для красного канала
#define GREEN_PIN 10  // Пин для зелёного канала
#define BLUE_PIN 11   // Пин для синего канала
#define BUTTON_PIN 3  // Пин кнопки


#define IMU_ADDRESS 0x68  //Change to the address of the IMU
//#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU6500 IMU;  //Change to the name of any supported IMU!

// Currently supported IMUS: MPU9255 MPU9250 MPU6886 MPU6500 MPU6050 ICM20689 ICM20690 BMI055 BMX055 BMI160 LSM6DS3 LSM6DSL QMI8658

calData calib = { 0 };  //Calibration data
calData preload_calib = { true, { -0.02, 0.00, 0.05 }, { -2.74, 2.21, -0.63 } };

AccelData accelData;  //Sensor data
GyroData gyroData;
MagData magData;

const float k = 0.1;
int effect = 0;  // Текущий эффект

OneButton button(BUTTON_PIN, true);

GRGB led(COMMON_CATHODE, RED_PIN, GREEN_PIN, BLUE_PIN);

void checkTicks() {
  // include all buttons here to be checked
  button.tick();  // just call tick() to check the state.
}

// this function will be called when the button was pressed 1 time only.
void singleClick() {
  Serial.println("singleClick() detected.");
  effect = (effect + 1) % 10;
  Serial.println(effect);
}  // singleClick

void setup() {
  Wire.begin();
  Wire.setClock(400000);  //400khz clock
  Serial.begin(9600);
  delay(50);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), checkTicks, CHANGE);
  button.attachClick(singleClick);

  //int err = IMU.init(calib, IMU_ADDRESS);
  int err = IMU.init(preload_calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

#ifdef PERFORM_CALIBRATION
  Serial.println("FastIMU calibration & data example");

  delay(5000);
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib.gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib.gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib.gyroBias[2]);

  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
#endif

  //err = IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  //err = IMU.setAccelRange(2);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g

  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {

  button.tick();

  switch (effect) {
    case 0: solidColor(255, 0, 0); break;  // Красный
    case 1: solidColor(0, 255, 0); break;  // Зелёный
    case 2: solidColor(0, 0, 255); break;  // Синий
    case 3: rainbow(); break;
    case 4: gyro(); break;              // Гороскоп
    case 5: blink(255, 255, 0); break;  // Жёлтое мерцание
    case 6: colorFade(); break;
    case 7: runningLight(); break;
    case 8: strobe(255, 255, 255); break;  // Белый стробоскоп
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
  static byte count = 0;  
  static unsigned long previousMillis = 0;
  const int interval = 50;  // Интервал смены цвета (мс)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    
    led.setWheel8(count); // Меняем цвет
    count++; // Переполняется до 255, потом снова 0
  }
}

// Плавное мигание
void gyro() {

  IMU.update();

  IMU.getAccel(&accelData);
  short accelX = constrain(abs(accelData.accelX * 100), 0, 100);
  accelX = expRunningAverageX(accelX);
  accelX = map(accelX, 0, 100, 0, 255);
  //Serial.print(accelX);
  //Serial.print("\t");

  short accelY = constrain(abs(accelData.accelY * 100), 0, 100);
  accelY = expRunningAverageY(accelY);
  accelY = map(accelY, 0, 100, 0, 255);
  //Serial.print(accelY);
  //Serial.print("\t");

  short accelZ = constrain(abs(accelData.accelZ * 100), 0, 100);
  accelZ = expRunningAverageZ(accelZ);
  accelZ = map(accelZ, 0, 100, 0, 255);
  //Serial.print(accelZ);
  //Serial.println("\t");

  analogWrite(RED_PIN, accelX);
  analogWrite(GREEN_PIN, accelY);
  analogWrite(BLUE_PIN, accelZ);

  delay(10);
}

// Мерцание случайными цветами
void randomFlash() {
  static unsigned long previousMillis = 0;
  const int interval = 200;  // Интервал вспышек (мс)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    solidColor(random(256), random(256), random(256));  // Случайный цвет
  }
}


// Бегущий огонёк (эффект волны)
void runningLight() {
  static int i = 0;
  static unsigned long previousMillis = 0;
  const int interval = 200;  // Интервал смены цвета (мс)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    analogWrite(RED_PIN, i == 0 ? 255 : 0);
    analogWrite(GREEN_PIN, i == 1 ? 255 : 0);
    analogWrite(BLUE_PIN, i == 2 ? 255 : 0);

    i = (i + 1) % 3;  // Переключение между цветами (0 → 1 → 2 → 0 ...)
  }
}

// Стробоскоп
void strobe(int r, int g, int b) {
  static int count = 0;
  static unsigned long previousMillis = 0;
  static bool isOn = false;
  const int interval = 50;  // Интервал мигания (мс)

  if (count < 10 && millis() - previousMillis >= interval) {
    previousMillis = millis();
    isOn = !isOn;  // Переключаем состояние

    if (isOn) {
      solidColor(r, g, b);  // Включаем цвет
    } else {
      solidColor(0, 0, 0);  // Выключаем
      count++;              // Увеличиваем счетчик только после полного цикла (вкл-выкл)
    }
    if (count >= 10) {
      count = 0;  // Автоматический сброс
    }
  }
}


// Мигание цветами
void blink(int r, int g, int b) {
  static unsigned long previousMillis = 0;
  static bool isOn = false;
  const int interval = 500;  // Интервал мигания (мс)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    isOn = !isOn;  // Переключаем состояние

    if (isOn) {
      solidColor(r, g, b);  // Включаем цвет
    } else {
      solidColor(0, 0, 0);  // Выключаем
    }
  }
}

// Плавная смена цветов
void colorFade() {
  static int i = 0;
  static unsigned long previousMillis = 0;
  const int interval = 10;  // Интервал обновления (мс)

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    analogWrite(RED_PIN, i);
    analogWrite(GREEN_PIN, 255 - i);
    analogWrite(BLUE_PIN, i / 2);
    i++;
    if (i > 255) i = 0;  // Зацикливание эффекта
  }
}

float expRunningAverageX(float newVal) {
  static float filVal = 0;
  filVal += (newVal - filVal) * k;
  return filVal;
}

float expRunningAverageY(float newVal) {
  static float filVal = 0;
  filVal += (newVal - filVal) * k;
  return filVal;
}

float expRunningAverageZ(float newVal) {
  static float filVal = 0;
  filVal += (newVal - filVal) * k;
  return filVal;
}
