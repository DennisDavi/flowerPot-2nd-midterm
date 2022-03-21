/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/Flower_Pot_Motor/src/Flower_Pot_Motor.ino"
/*
 * Project Flower_Pot_Motor
 * Description:running code for the motor
 * Author:Dennis Davis
 * Date:21 March 2022
 */

//#include "Adafruit_BME280.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
//#include "Air_Quality_Sensor.h"
#include "Wire.h"
#include "SPI.h"

void setup();
void loop();
#line 15 "c:/Users/DennisDavis/Documents/IoT/flowerPot-2nd-midterm/Flower_Pot_Motor/src/Flower_Pot_Motor.ino"
int motor = D11;
int motorState;
int button1 = A1;
bool buttonState;
int oldButton;
int OnOff;
int OLED_RESET = D4;
int SCREEN_WIDTH = 128;
int SCREEN_HIGHT = 64;
int SCREEN_ADDRESS = 0x3c;
Adafruit_SSD1306 display(OLED_RESET);
int currentTime;
int lastTime;
int soilMeter = A2;
int soilRead;


//SYSTEM_MODE(SEMI_AUTOMATIC)

void setup() {
    Serial.begin(9600);
    pinMode(button1, INPUT_PULLDOWN);
    pinMode(motor, OUTPUT);
    pinMode(soilMeter,OUTPUT);

    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

    display.display();
    delay(2000);

    display.clearDisplay();
}

void loop() {
  
    currentTime = millis();
    buttonState = digitalRead(button1);
    if (buttonState != oldButton) { // turning the button on and off
        if (buttonState == true) {
            OnOff = !OnOff;
            Serial.printf("button:%i\n", OnOff);
        }
        oldButton = buttonState;
    }
    if (OnOff == HIGH) {               //the button is turning on and off the motor
        digitalWrite(motor, HIGH);
    } else {                   
        digitalWrite(motor, LOW);
    }
  soilRead=analogRead(soilMeter);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.printf("Soil meter:%i\n",soilRead);
    display.display();

    if ((currentTime - lastTime) - 2000) {
        display.clearDisplay();
        lastTime = millis();
    }
}