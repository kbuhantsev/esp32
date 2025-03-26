#include "FastIMU.h"
#include <Wire.h>
#include "Madgwick.h"

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
bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  Wire.begin();
  Wire.setClock(400000);  //400khz clock
  Serial.begin(115200);
  delay(50);

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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    effect = (effect + 1) % 10;  // Переключение эффекта
    delay(50);                   // Антидребезг
  }
  lastButtonState = buttonState;

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
  for (int i = 0; i < 256; i++) {
    analogWrite(RED_PIN, sin(i * 0.024) * 127 + 128);
    analogWrite(GREEN_PIN, sin((i + 85) * 0.024) * 127 + 128);
    analogWrite(BLUE_PIN, sin((i + 170) * 0.024) * 127 + 128);
    delay(30);
  }
}

// Плавное мигание
void gyro() {

  IMU.update();

  IMU.getAccel(&accelData);
  short accelX = constrain(abs(accelData.accelX * 100), 0, 100);
  accelX = expRunningAverageX(accelX);
  accelX = map(accelX, 0, 100, 0, 255);
  Serial.print(accelX);
  Serial.print("\t");

  short accelY = constrain(abs(accelData.accelY * 100), 0, 100);
  accelY = expRunningAverageY(accelY);
  accelY = map(accelY, 0, 100, 0, 255);
  Serial.print(accelY);
  Serial.print("\t");

  short accelZ = constrain(abs(accelData.accelZ * 100), 0, 100);
  accelZ = expRunningAverageZ(accelZ);
  accelZ = map(accelZ, 0, 100, 0, 255);
  Serial.print(accelZ);
  Serial.println("\t");

  analogWrite(RED_PIN, accelX);
  analogWrite(GREEN_PIN, accelY);
  analogWrite(BLUE_PIN, accelZ);

  delay(10);
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
