#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

//egendefinerte bibliotek
#include <DisplaySpeed.h>
#include <lysSensor.h>
#include <ultrasonic.h>
#include <flexSensor.h>


void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledArray[i], OUTPUT);  //kompakt måte å sette opp flere leds
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
  flexSensor();   // alt for vind - vimpel i en funksjon

  lesLys();  //leslys velger også select 

  switch (select)     //velger belysning i tunell avhengig av lysforhold
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

  if (speed != previousSpeed)    //hvis fartsmåleren gjør en ny måling, oppdater skjerm
  {
    dizplay(speed);
    previousSpeed = speed;
  }
}