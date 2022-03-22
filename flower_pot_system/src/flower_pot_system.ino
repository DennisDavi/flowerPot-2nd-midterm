/*
 * Project flower_pot_system
 * Description:making the flower pot work
 * Author:Dennis Davis
 * Date:21 March 2022
 */

#include "Adafruit_BME280.h"
#include "Adafruit_MQTT_SPARK.h"
#include "math.h"

#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "credentials.h"

TCPClient theClient;

Adafruit_MQTT_SPARK mqtt(&theClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish mqttObj1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/SoilReading");
Adafruit_MQTT_Publish mqttObj2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/AirQuality");
Adafruit_MQTT_Publish mqttObj3 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DustReading");
Adafruit_MQTT_Publish mqttObj4 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TemperatureReading");
Adafruit_MQTT_Publish mqttObj5 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/HumidityReading");

Adafruit_MQTT_Subscribe mqttObj0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/PumpButton");

int motor = D11;
int motorState;
int button1 = A1;
bool buttonState;
int oldButton;
int OnOff;
int soilMeter = A2;
int soilRead;
int airSensor = A3;
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

// SYSTEM_MODE(SEMI_AUTOMATIC)

void setup() {
    Serial.begin(9600);
    status = bme.begin(hexAddress);
    if (status == false) {
        Serial.printf("BME280 at address 0x%02X failed to start", hexAddress);
    }
    pinMode(button1, INPUT_PULLDOWN);
    pinMode(motor, OUTPUT);
    pinMode(soilMeter, INPUT);

    waitFor(Serial.isConnected, 15000);

    WiFi.connect();
    while (WiFi.connecting()) {
        Serial.printf(".");
    }

    mqtt.subscribe(&mqttObj0);
}

void loop() {
    MQTT_connect();

    if ((millis() - last) > 120000) {
        Serial.printf("Pinging MQTT \n");
        if (!mqtt.ping()) {
            Serial.printf("Disconnecting \n");
            mqtt.disconnect();
        }
        last = millis();
    }

    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(100))) {
        if (subscription == &mqttObj0) {
            value2 = atof((char *)mqttObj0.lastread);
        }
    }

    tempC = bme.readTemperature();
    pres = bme.readPressure();
    hum = bme.readHumidity();

    tempF = tempC * (9.0 / 5.0) + 32.2;

    buttonState = digitalRead(button1);
    if (buttonState != oldButton) { // turning the button on and off
        if (buttonState == true) {
            OnOff = !OnOff;
            Serial.printf("button:%i\n", OnOff);
        }
        oldButton = buttonState;
    }
    if (OnOff == HIGH || value2 == HIGH) {
        digitalWrite(motor, HIGH);
    } else {
        digitalWrite(motor, LOW);
    }

    soilRead = analogRead(soilMeter);
    airQuality = analogRead(airSensor);
    dustReading = analogRead(dustSensor);
    Serial.printf("soil reading:%i air reading:%i dust reading: %i Temp is:%0.2f Humidity:%0.2f\n", soilRead, airQuality, dustReading, tempF, hum);

    if ((millis() - lastTime > 30000)) {
        if (mqtt.Update()) {
            mqttObj1.publish(soilRead);
            mqttObj2.publish(airQuality);
            mqttObj3.publish(dustReading);
            mqttObj4.publish(tempF);
            mqttObj5.publish(hum);
            Serial.printf("soil reading:%i air reading:%i dust reading: %i Temp is:%0.2f Humidity:%0.2f\n", soilRead, airQuality, dustReading, tempF, hum);
        }
        lastTime = millis();
    }
}

void MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.printf("%s\n", (char *)mqtt.connectErrorString(ret));
        Serial.printf("Retrying MQTT connection in 5 seconds..\n");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
    }
    Serial.printf("MQTT Connected!\n");
}