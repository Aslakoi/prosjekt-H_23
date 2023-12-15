// pratform.ini detaljer for sensornode
/*
platform = atmelavr
board = uno
framework = arduino
lib_deps =
  adafruit/Adafruit NeoPixel@^1.12.0
  olikraus/U8g2@^2.35.8
*/



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
