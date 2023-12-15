#ifndef ULTRASONIC_H
#define ULTRASONIC_H

const int trigPin = 5; // Definerer trigger pin som nummer 9
const int echoPin = 6; // Defines echo pin som nummer 10

long duration; // for kalkulasjon av avstand
int distance; // avstand som brukes til speecalc
int speed = 0;
int previousSpeed = 0;
unsigned long time = 0;
int measure1 = 0;    
int measure2 = 0;
unsigned long time1, time2; //tid ved første og andre måling

void ultrasonic() {

  digitalWrite(trigPin, LOW); // Resetter trigpin
  delayMicroseconds(2); // mikrosekund går så raskt at det ikke blokkerer annen kode

  digitalWrite(trigPin, HIGH); // sender en 10 mikrosekund lang puls
  delayMicroseconds(10); // 
  digitalWrite(trigPin, LOW); // 

  duration = pulseIn(echoPin, HIGH); // registrerer når echo pin fanger opp pulsen og finner ut hvor lang tid det tok

  distance = duration * 0.034 / 2; // regner ut avstand med lydens hastighet fram og tilbake
  
}

void speedCalc() 
{
  if (distance <= 30 && measure1 == 0) //registrer objekt og tar første måling
  {
    time1 = millis();
    measure1 = 1;
  }
  
  if (distance <= 20 && measure2 == 0)  //andre måling
  {
    time2 = millis();
    measure2 = 1;
  }

  if (measure1 == 1 && measure2 == 1)  //regner ut tidsforskjell mellom målingene
  {
    measure1 = 0;
    measure2 = 0;
    unsigned long timeDiff = (time2 > time1) ? (time2 - time1) : (time1 - time2);
    float timeInSeconds = (float)timeDiff / 1000.0; // tid i sekund
    speed = 10 / timeInSeconds; // fart i cm/s
  }
}

#endif
