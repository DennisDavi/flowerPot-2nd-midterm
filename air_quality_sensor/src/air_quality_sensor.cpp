/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/air_quality_sensor/src/air_quality_sensor.ino"
/*
 * Project air_quality_sensor
 * Description:
 * Author:
 * Date:
 */
#include "Air_Quality_Sensor.h"

void setup();
void loop();
#line 9 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/air_quality_sensor/src/air_quality_sensor.ino"
int airSensor= A0;
int airValue;

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
Serial.begin(9600);
pinMode(airSensor, INPUT);

}

void loop() {
  airValue=analogRead(airSensor);
  Serial.printf("air Value is:%i\n", airValue);

}