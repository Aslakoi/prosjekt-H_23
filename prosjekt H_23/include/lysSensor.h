#ifndef LYSSENSOR_H
#define LYSSENSOR_H

#include <Adafruit_NeoPixel.h>

// neopixel variabler
int neopixelPin = 12;
int NUMPIXELS = 32;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neopixelPin, NEO_RGB + NEO_KHZ800);

const int sensorPin = A5;
int sensorVal;
unsigned long previousTimeLys = 0;
int select = 0;

void lesLys()
{
  unsigned long currentTime = millis();

  if (currentTime - previousTimeLys >= 3000)        //sjekker hver 3. sek for å unngå unødvendige endringer
  {
    sensorVal = analogRead(sensorPin);

    if (sensorVal >= 320)                          //enkel logikk for å velge case i main loop
    {
      select = 1;
    }
    else if (sensorVal <= 100)
    {
      select = 2;
    }
    else if (sensorVal < 320 && sensorVal > 100)
    {
      select = 3;
    }

    previousTimeLys = currentTime;
  }
}

void dagLys()    // hvitt lys på dagen, og relativt lavt lysnivå pga. det er dagslys
{
  pixels.clear();
  for (int i = 0; i < 16; i++)
  {
    // øk lysstyrke mot midten av tunellen
    pixels.setPixelColor(i, pixels.Color(map(i, 0, 15, 0, 70), map(i, 0, 15, 0, 70), map(i, 0, 15, 0, 70))); // GRB
  }
  for (int i = 16; i < 32; i++)
  {
    // senk lysverdi mot åpning av tunellen
    pixels.setPixelColor(i, pixels.Color(map(i - 16, 0, 15, 70, 0), map(i - 16, 0, 15, 70, 0), map(i - 16, 0, 15, 70, 0)));
  }
  pixels.show();
}

void nattLys() //rødt lys for å ikke skade mørkesynet
{
  pixels.clear();
  for (int i = 0; i < 16; i++)
  {
    // øk lysstyrke mot midten av tunellen
    pixels.setPixelColor(i, pixels.Color(0, map(i, 0, 15, 0, 200), 0)); // GRB
  }
  for (int i = 16; i < 32; i++)
  {
    // senk lysverdi mot åpning av tunellen
    pixels.setPixelColor(i, pixels.Color(0, map(i - 16, 0, 15, 200, 0), 0));
  }
  pixels.show();
}

void halvLys()   //lys for overgang mellom natt og dagslys, så litt sterkere lys og gult lys for overgang til rødt
{
  pixels.clear();
  for (int i = 0; i < 16; i++)
  {
    pixels.setPixelColor(i, pixels.Color(map(i, 0, 15, 0, 225), map(i, 0, 15, 0, 225), 0)); // GRB
  }
  for (int i = 16; i < 32; i++)
  {
    pixels.setPixelColor(i, pixels.Color(map(i - 16, 0, 15, 225, 0), map(i - 16, 0, 15, 225, 0), 0));
  }
  pixels.show();
}

#endif