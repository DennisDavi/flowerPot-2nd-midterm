/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/flower_pot_system/src/flower_pot_system.ino"
/*
 * Project flower_pot_system
 * Description:making the flower pot work
 * Author:Dennis Davis
 * Date:21 March 2022
 */

#include "Adafruit_BME280.h"
#include "math.h"
#include "Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT.h"
#include "credentials.h"

void setup();
void loop();
#line 14 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/flower_pot_system/src/flower_pot_system.ino"
TCPClient theClient;

Adafruit_MQTT_SPARK mqtt(&theClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Subscribe mqttObj0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/PumpButton");

int motor = D11;
int motorState;
int button1 = A1;
bool buttonState;
int oldButton;
int OnOff;
int soilMeter=A2;
int soilRead;
int airSensor=A3;
int airQuality;
int dustSensor = A4;
int dustReading;
Adafruit_BME280 bme;
float tempC;
float tempF;
float pres;
float hum;
bool status;
int hexAddress = 0x76;
unsigned long last, lastTime;
int value1, value2;



//SYSTEM_MODE(SEMI_AUTOMATIC)


void setup() {
  Serial.begin(9600);
  status = bme.begin(hexAddress);
  if (status == false){
    Serial.printf("BME280 at address 0x%02X failed to start", hexAddress);
  }
    pinMode(button1, INPUT_PULLDOWN);
    pinMode(motor,OUTPUT);
    pinMode(soilMeter,INPUT);

    waitFor(Serial.isConnected, 15000);

    WiFi.connect();
    while(WiFi.connecting()){
      Serial.printf(".");
    }

    mqtt.subscribe(&mqttObj0);

}

void loop() {
  //MQTT_connect();

  if ((millis()-last)>120000){
    Serial.printf("Pinging MQTT \n");
    if(! mqtt.ping()){
      Serial.printf("Disconnecting \n");
      mqtt.disconnect();
    }
    last=millis();
  }

  Adafruit_MQTT_Subscribe *subscription;
  while((subscription=mqtt.readSubscription(100))){
    if(subscription == &mqttObj0){
      value2 = atof((char *)mqttObj0.lastread);
    }
    
    }
    
  
  tempC = bme.readTemperature();
  pres = bme.readPressure();
  hum = bme.readHumidity();

  tempF = tempC * (9.0/5.0)+32.2;

  buttonState = digitalRead(button1);
    if (buttonState != oldButton) { // turning the button on and off
        if (buttonState == true) {
            OnOff = !OnOff;
            Serial.printf("button:%i\n", OnOff);
        }
        oldButton = buttonState;
    }
    if(OnOff == HIGH || value2 == HIGH){
    digitalWrite(motor,HIGH);
    }else{
      digitalWrite(motor,LOW);
    }

    soilRead=analogRead(soilMeter);
    airQuality=analogRead(airSensor);
    dustReading=analogRead(dustSensor);
    Serial.printf("soil reading:%i air reading:%i dust reading: %i Temp is:%0.2f Humidity:%0.2f\n",soilRead, airQuality, dustReading, tempF, hum);

}