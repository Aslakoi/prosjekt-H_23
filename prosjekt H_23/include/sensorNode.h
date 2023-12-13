#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <DisplaySpeed.h>
#include <lysSensor.h>
#include <ultrasonic.h>
#include <flexSensor.h>

//pratform.ini detaljer for sensornode
/*
platform = atmelavr
board = uno
framework = arduino
lib_deps = 
	adafruit/Adafruit NeoPixel@^1.12.0
	olikraus/U8g2@^2.35.8
*/

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledArray[i], OUTPUT);
  }
  pinMode(flexPin, INPUT);
  pinMode(neopixelPin, OUTPUT);
  pixels.begin();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  Serial.begin(9600);       
  display.begin();
  display.setFont(u8g2_font_ncenB14_tr);
}

void loop()
{
  flexSensor();

  lesLys();

  switch (select)
  {
  case 1:
    dagLys();
    break;
  case 2:
    nattLys();
    break;
  case 3:
    halvLys();
    break;
  }

  ultrasonic();
  speedCalc();

  if (speed != previousSpeed)
  {
    dizplay(speed);
    previousSpeed = speed;
  }
}