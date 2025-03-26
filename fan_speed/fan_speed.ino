#include <Arduino.h>
#include "FanMonitor.h"

// ***
// *** Pins
// ***
#define FAN_MONITOR_PIN 5

// ***
// *** Define the FanMonitor instance to monitor
// *** the 3-wire fan.
// ***
FanMonitor _fanMonitor = FanMonitor(FAN_MONITOR_PIN, FAN_TYPE_BIPOLE);

int pin = 5;
unsigned long duration;

void setup()
{
  // ***
  // *** Initialize the serial port.
  // ***
  Serial.begin(115200);

  // ***
  // *** Initialize the fan monitor.
  // ***
  _fanMonitor.begin();

  //pinMode(pin, INPUT);

}

void loop() {
  
  duration = pulseIn(pin, HIGH);
  Serial.println(duration);

  // ***
  // *** Get the fan speed.
  // ***
  //uint16_t rpm = _fanMonitor.getSpeed();

  // ***
  // *** Print the speed to the serial port.
  // ***
  //Serial.print("Speed = "); Serial.print(rpm); Serial.println(" RPM");
  
  // ***
  // *** Delay 1 second.
  // ***
  delay(1000);
  
}