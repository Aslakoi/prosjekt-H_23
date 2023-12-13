#ifndef GATEMUSIKANT_H
#define GATEMUSIKANT_H

/*#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <SPI.h>*/
#include <musikk.h>

//Zumo32U4Buzzer buzzer; 
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4OLED display;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
//Zumo32U4LineSensors lineSensors;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

bool linjeFlagg = false;
bool exitFlagg;
//int previousTime = 0; //til kalk.
float rotationsRight = 0;
float rotationsLeft = 0;
float distanceRight, distanceLeft, distance, distanceDiff = 0;
long select = 0;

int status = 0;
unsigned long trykkTid;
bool transformToggle = false;
bool aKnappeTrykk;
int saldo = 0;

unsigned long previousTime = 0;


//void calibrateSensors();
//void teller();
void distanceCalc();
void sirkel();
void firkant();
void selectMode();
void menu();
void gatemusikant();
void transformersKnapp();
void toggleTrans();

/*void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  display.setLayout21x8();
  display.clear();
}

void loop() 
{
  gatemusikant();
}*/

void gatemusikant()
{
    display.setLayout21x8();
    //selectMode();
  switch (select)
  {
    case 1:
      display.clear();
      sirkel();
      select = 0;
      saldo += 15;
      break;
    case 2:
      display.clear();
      firkant();
      select = 0;
      saldo += 20;
      break;
    case 3:
      display.clear();
      playSong();
      select = 0;
      saldo += 35;
      break;
    case 4:
      display.clear();
      playAnthem();
      select = 0;
      saldo += 50;
      break;
    default:
      motors.setSpeeds(0, 0);
      menu();
      selectMode();
      transformersKnapp();
      break;
  }
}

void transformersKnapp()
{
  
  const unsigned long KNAPPE_VENT = 2000;
  if (buttonA.isPressed() && !aKnappeTrykk)
  {
    trykkTid = millis();
    aKnappeTrykk = true;
  }
  else if (!buttonA.isPressed())
  {
    if (millis() - trykkTid >= KNAPPE_VENT && aKnappeTrykk)
    {
      transformToggle = !transformToggle;
      toggleTrans();
      //buzzer.playFrequency(440, 200, 15);
    }
    aKnappeTrykk = false;
  }
}
void toggleTrans()
{
  if (transformToggle) {status = 3;}
  else {status = 0;}
}

void distanceCalc()
{
  rotationsRight = encoders.getCountsRight() / (75.81 * 12); //CpR = 75.81 * 12 <=> C/CpR = R
  rotationsLeft = encoders.getCountsLeft() / (75.81 * 12);
  distanceRight = rotationsRight * (3.5 * 3.14);                  // diameter (3.5 cm) * pi = omkrets
  distanceLeft = rotationsLeft * (3.5 * 3.14);                    //rotasjoner for både høyre og venstre gir bedre 
  distance = (distanceRight + distanceLeft) / 2;      // in centimeters
}

void selectMode()
{
  if (buttonC.getSingleDebouncedPress())
  {
    select = random(1, 3);
    exitFlagg = false;
  }
  else if (buttonB.getSingleDebouncedPress())
  {
    select = random(3,5);
  }
}

void firkant()
{
  encoders.getCountsAndResetLeft();  //reset distance variabler slik at if setningene er mulige å gjennomføre mer enn en gang
  encoders.getCountsAndResetRight();

  while (!exitFlagg)
  {

    distanceCalc();

    if (distance <= 36)
    {
      motors.setSpeeds(210, 200); // 210 gjør opp for skjeve motorer
    }
    else if (distance > 36 && distanceRight <= 51) // høyre sving = 36 + 15.5  = 51.5, høyre sving = 15.5 inkl. margin, bil stopper etter den skal)
    {
      motors.setSpeeds(0, 200);
    }
    else if (distance <= 81)
    {
      motors.setSpeeds(210, 200);
    }
    else if (distance > 81 && distanceRight <= 103) //ny distance = 89
    {
      motors.setSpeeds(0, 200);
    }
    else if (distance <= 125) // 89 + 36
    {
      motors.setSpeeds(210, 200);
    }
    else if (distance > 125 && distanceRight <= 156) //142 + 15.5 (høyre) ny distance = 125.64
    {
      motors.setSpeeds(0, 200);
    }
    else if (distance < 165) //ender opp der den startet 
    {
      motors.setSpeeds(210, 200);
    }
    else if (distance >= 165 && distanceRight <= 202)
    {
      motors.setSpeeds(0, 200);
    }
    else
    {
      exitFlagg = true;
      motors.setSpeeds(0, 0);
    }
  }
}

void sirkel()
{
  encoders.getCountsAndResetLeft();  //reset distance variabler slik at if setningene er mulige å gjennomføre mer enn en gang
  encoders.getCountsAndResetRight();

  while(!exitFlagg)
  {
    distanceCalc();

    if (distanceRight < 83 && distanceLeft < 24)
    {
      motors.setSpeeds(50, 200);
    }
    else 
    {
      exitFlagg = true;
      motors.setSpeeds(0, 0);
    }
  }
}


void menu()
{
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= 500)
  {
    display.gotoXY(0, 0);
    display.print("Press C for a 'dance'");
    display.gotoXY(0, 1);
    display.print("hehe");
    display.gotoXY(0, 3);
    display.print("Press B for a song ;)");
    display.gotoXY(0, 4);
    display.print("hehe");
    display.gotoXY(0, 7);
    display.print(F("saldo: "));
    display.print((saldo));
    display.gotoXY(10, 7);
    display.print("australske");

    previousTime = currentTime;
  }
  
}

//til setup:
//lineSensors.initFiveSensors();
//calibrateSensors();

/*
void calibrateSensors()  //denne blir også kun brukt for nye kalkulasjoner
{         
  for (int i = 0; i < 200; i++) {
    motors.setSpeeds(-200, 200);
    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void teller() // denne blir ikke brukt i faktisk kode, den ble bare brukt til å kalkulere kode for sirkel og firkant
{
  unsigned long currentTime = millis();

  if (!linjeFlagg) {
    if (currentTime - previousTime >= 50) // oppdater variabler hvert 50ms
    {
      previousTime = currentTime;

      motors.setSpeeds(0, 200);
      distanceCalc();                   // centimeters
      display.gotoXY(0, 0);
      display.print(distanceRight);
      display.gotoXY(0, 1);
      display.print(distanceLeft);
      display.gotoXY(0, 2);
      display.print(distance);

      int posisjon = lineSensors.readLine(lineSensorValues);
      linjeFlagg = (posisjon > 0); // flagg når tape er observert
    }
  } else {
    motors.setSpeeds(0, 0); // Stop the robot when tape is detected
  }
}
*/
#endif