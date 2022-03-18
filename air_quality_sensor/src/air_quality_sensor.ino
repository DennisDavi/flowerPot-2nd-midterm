/*
 * Project air_quality_sensor
 * Description:
 * Author:
 * Date:
 */
#include "Air_Quality_Sensor.h"

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