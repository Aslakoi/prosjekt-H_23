// Zumo32U4 klassene er definert i header filene der de brukes

#include <EEPROM.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <Arduino.h>
#include <SPI.h>
#include <musikk.h>
#include <gatemusikant.h>
#include <SWbatteri.h>

void setup()
{
    //fysiske buzzerpin på zumo for egendefinert sang
    pinMode(BUZZER_PIN, OUTPUT);
    // EEPROM.write(HELSE_ADRESSE, 100); //redigere batterihelsen direkte
    // EEPROM.write(OPPLADNINGER_ADRESSE, 0); //redigere oppladninger direkte
    // EEPROM.write(SERVICER_ADRESSE, 0); //redigere servicer direkte
    // EEPROM.write(SALDO_ADRESSE, 50); //redigere saldoen dirkete

    lineSensors.initFiveSensors();
    helse = EEPROM.read(HELSE_ADRESSE);
    oppladninger = EEPROM.read(OPPLADNINGER_ADRESSE);
    servicer = EEPROM.read(SERVICER_ADRESSE);
    saldo = EEPROM.read(SALDO_ADRESSE);
    display.setLayout11x4();
    display.gotoXY(2, 1);
    display.print("Trykk A");
    buttonA.waitForButton();
    delay(200);
    kalibrerSensorer();
}

void loop()
{
    // To kjøre-caser, en hvor bilen følger linja og en hvor den rygger, i default står den stille og
    // venter på instrukser fra knapper
    switch (status)
    {
    case 1:
        folgeLinje();
        hovedDisplay();
        break;
    case 2:
        rygg();
        hovedDisplay();
        break;
    case 3:
        gatemusikant();
        break;
    default:
        // Bilen stopper og man kan benytte andre knapper enn start stopp
        motors.setSpeeds(0, 0);
        ryggeLadingKnapp();
        ladeKnapp();
        batteriHelseKnapp();
        hovedDisplay();

        break;
    }
    // kjører funksjonen i intervaller for å begrense if-tester arduinoen må gjennomføre,
    // må til for at linjefølgingen skal funke
    if (millis() - sisteMillis >= INTERVAL)
    {
        fartOgDistanse();
        buzz();
        if (kjorer)
        {
            batteriProsent();
        }

        sisteMillis = millis();
    }
    // Kjøres hele tiden for å få meg seg alle knappetrykk
    kjoreKnapp();
    gatemusikantKnapp();
}