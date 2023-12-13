#ifndef ULTRASONIC_H
#define ULTRASONIC_H

const int trigPin = 5; // Defines trigger pin as number 9
const int echoPin = 6; // Defines echo pin as number 10

long duration; // Defines a long variable named duration
int distance; // Defines an integer variable named distance
int speed = 0;
float previousSpeed = 0;
unsigned long time = 0;
unsigned long measure1 = 0;
unsigned long measure2 = 0;
int time1, time2;

void ultrasonic() {

  digitalWrite(trigPin, LOW); // Resets the trigPin
  delayMicroseconds(2); // Waits 2 microseconds

  digitalWrite(trigPin, HIGH); // Sets the trigPin state to HIGH
  delayMicroseconds(10); // Waits 10 microseconds
  digitalWrite(trigPin, LOW); // Sets the trigPin state to LOW

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2; // Calculation of the distance
  
}

void speedCalc() {
  if (distance <= 30 && measure1 == 0) {
    time1 = millis();
    measure1 = 1;
  }
  
  if (distance <= 20 && measure2 == 0) {
    time2 = millis();
    measure2 = 1;
  }

  if (measure1 == 1 && measure2 == 1) {
    measure1 = 0;
    measure2 = 0;
    unsigned long timeDiff = (time2 > time1) ? (time2 - time1) : (time1 - time2);
    float timeInSeconds = (float)timeDiff / 1000.0; // Convert time to seconds
    speed = 10 / timeInSeconds; // Calculate speed in cm/s
  }
}

#endif
