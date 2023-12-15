#ifndef GATEMUSIKANT_H
#define GATEMUSIKANT_H
 
//initialiserer kun de klassene som brukes i denne filen
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4OLED display;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC; 

bool exitFlagg; //for å hoppe inn og ut av sirkel og firkant
float rotationsRight, rotationsLeft = 0; // til kalkulasjon for firkant og sirkel
float distanceRight, distanceLeft, distance, distanceDiff = 0; // til kalkulasjon for firkant og sirkel

int select = 0; // switch variabel for gatemusikant
int status = 0; // switch variabel for main loop
unsigned long trykkTid; //tidskontroll for langt A trykk (bytte mellom gatemusikant og vanlig)
bool selectToggle = false; // for bytte av select
bool aKnappeTrykk; 
int saldo = 0; //bankkonto

unsigned long previousTime = 0; //tidskontroll menu

//oversikt
void distanceCalc();
void sirkel();
void firkant();
void selectMode();
void menu();
void gatemusikant();
void gatemusikantKnapp();
void toggleSelect();

void gatemusikant()
{
  display.setLayout21x8(); //bru

  switch (select)
  {
    case 1:
      display.clear();
      sirkel();
      select = 0;      //gjennofører underholdning en gang, så returner til menu
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
      playSong();      //fra dokumentasjon
      select = 0;
      saldo += 35;
      break;
    case 4:
      display.clear();
      playAnthem();     //egendefinert sang, spilles i videoen
      select = 0;
      saldo += 50;
      break;
    default:
      motors.setSpeeds(0, 0);
      menu(); 
      selectMode();
      gatemusikantKnapp();
      break;
  }
}

void gatemusikantKnapp()
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
      selectToggle = !selectToggle;
      toggleSelect();
      //buzzer.playFrequency(440, 200, 15);
    }
    aKnappeTrykk = false;
  }
}

void toggleSelect()     
{
  if (selectToggle) 
  {
    status = 3;
  }
  else 
  {
    status = 0;
  }
}

void distanceCalc()
{
  rotationsRight = encoders.getCountsRight() / (75.81 * 12); //CpR = 75.81 * 12 <=> C/CpR = R
  rotationsLeft = encoders.getCountsLeft() / (75.81 * 12);
  distanceRight = rotationsRight * (3.5 * 3.14);                  // diameter (3.5 cm) * pi = omkrets
  distanceLeft = rotationsLeft * (3.5 * 3.14);                    //verdi for både høyre og venstre gir flere målingsmuligheter
  distance = (distanceRight + distanceLeft) / 2;      // i centimeter
}

void selectMode() 
{
  if (buttonC.getSingleDebouncedPress()) //tilfeldig dans når du trykker C, enten firkant eller sirkel
  {
    select = random(1, 3);
    exitFlagg = false;  //gjør det mulig å gå inn i firkant og sirkel loop
  }
  else if (buttonB.getSingleDebouncedPress()) //tilfeldig sang når du trykker B
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
    else if (distance > 36 && distanceRight <= 51) // høyre sving = 36 + 15  = 51, høyre sving = 15 (inkl. margin, bil stopper etter den skal)
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
    else if (distance >= 165 && distanceRight <= 202) //svinger inn til starposisjon
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

    if (distanceRight < 83 && distanceLeft < 24) //magiske tall, men de er regnet ut metodisk
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

  if (currentTime - previousTime >= 500) //trenger ikke oppdatere skjermen konstant, sparer energi
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

#endif