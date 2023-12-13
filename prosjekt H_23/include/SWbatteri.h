#ifndef SWBATTERI_H
#define SWBATTERI_H

#include  <gatemusikant.h>

// Variabler for EEPROM
const int HELSE_ADRESSE = 651;
const int OPPLADNINGER_ADRESSE = 652;
const int SERVICER_ADRESSE = 653;
const int SALDO_ADRESSE = 654;
const unsigned long INTERVAL_DISPLAY_MINNE = 2000;
unsigned long sisteMillisDM = 0;

// varibaler for fart og distanse
float distanse, hastighet;
unsigned long sisteMillis = millis();
const unsigned long INTERVAL = 200;

// variabler for batteri
float prosent = 100;
int helse, oppladninger, servicer, teller, prosentC = 0;
int hastigheter[10];
float sisteDistanse = 0;
const unsigned long INTERVAL_GJN = 1500;
const int TELLER_INTERVAL = INTERVAL_GJN / INTERVAL;
bool kjorer = 0;
bool batteriFeil = 0;
bool bKnappeTrykk = 0;
bool cKnappeTrykk = 0;
unsigned long trykkTidC, ryggeTidStart, sisteMillis1 = 0;

// Linjesensorer
#define ANTALL_SENSORER 5
unsigned int linjeSensorVerdier[ANTALL_SENSORER];
const int MAX_FART = 400;

//Zumo32U4OLED display;
//Zumo32U4Motors motors;
//Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;
//Zumo32U4ButtonC buttonC;
//Zumo32U4ButtonB buttonB;
//Zumo32U4ButtonA buttonA;
//Zumo32U4Buzzer buzzer;

// Måler fart, om bilen kjører og distasnen den har kjørt siden oppstart
void fartOgDistanse()
{
    float enkoderL = encoders.getCountsAndResetLeft() * 0.0135;
    float enkoderR = encoders.getCountsAndResetRight() * 0.0135;
    distanse += abs((enkoderL + enkoderR) / 200);
    hastighet = ((enkoderL + enkoderR) / 200) / (0.2); // avhengig av millis intervallet i loopen,
    //men funket ikke å bruke variabelen direkte
    kjorer = (enkoderL != 0 || enkoderR != 0);
}
// Simulerer batteriforbruken basert på fart og distasne tilbakelagt
void batteriProsent()
{
    hastigheter[teller] = hastighet; //Array for å beregne gjennomsnittsfart
    prosent = constrain(prosent - ((distanse - sisteDistanse) / 2), 0, 100);
    sisteDistanse = distanse;
    if (teller == TELLER_INTERVAL)
    {
        teller = 0;
    }
    else
    {
        teller++;
    }
    if (millis() - sisteMillis1 >= INTERVAL_GJN)
    {
        float avgHastighet = 0;
        for (int i = 0; i < 11; i++)
        {
            avgHastighet += hastigheter[i]; //regner ut gjennomsnittshastighet
        }
        prosent = constrain(prosent - ((avgHastighet / 11) / 3), 0, 100); // gjennomsnittelig hastighet trekkes fra på prosenten
        sisteMillis1 = millis();
    };

    if (prosent < 2)
    {
        status = 0; //setter switch casen til default, som stopper bilen
    }
}
// Lader batteri til 100, trekker fra helse basert på hvor mange oppladniger som er gjort og en tilfedig mulighet for feil på batteriet
void ladeKnapp()
{
    if (buttonC.getSingleDebouncedPress() && saldo >= 5)
    {
        prosent = 100;
        saldo -= 5;
        helse = constrain(helse - (oppladninger / 2), 1, 100);
        // tilfedig batterifeil ved lading:
        if (random(100) == 42)
        {
            helse = constrain(helse - 50, 1, 100);
            batteriFeil = 1;
        }
        oppladninger++;
    }
}
/*Ved kort knappetrykk på B-knappen får den service på batteriet.
ved å holde inne i 2 sek bytter man batteri */
void batteriHelseKnapp()
{
    const unsigned long KNAPPE_VENT = 2000;
    if (buttonB.isPressed() && !bKnappeTrykk)
    {
        trykkTid = millis();
        bKnappeTrykk = 1;
    }
    else if (!buttonB.isPressed())
    {
        //setter inn nytt oppladet batteri og sletter servicer og oppladniger fra minnet
        if (millis() - trykkTid >= KNAPPE_VENT && bKnappeTrykk && saldo >= 40)
        {
            helse = 100;
            prosent = 100;
            servicer = 0;
            oppladninger = 0;
            saldo -= 40;
        }
        bKnappeTrykk = 0;
    }
    if (buttonB.getSingleDebouncedPress() && saldo >= 10)
    {
        //utfører service på batteriet
        helse = constrain(helse + 15, 1, 100);
        saldo -= 10;
        servicer++;
    }
}
// Bestemmer om case 1 eller default blir gjennomført
void kjoreKnapp()
{
    if (buttonA.getSingleDebouncedPress())
    {
        if (status == 1)
        {
            status = 0;
        }
        else if (status == 0)
        {
            status = 1;
            delay(200);
        }
    }
    
}
// registrerer langt knappetrykk på knapp C og skifter case
void ryggeLadingKnapp()
{
    const unsigned long KNAPPE_VENT = 2000;
    if (buttonC.isPressed() && !cKnappeTrykk)
    {
        trykkTidC = millis();
        cKnappeTrykk = 1;
        prosentC = prosent; //lagrer batteriprosenten når knappen trykkes, brukes videre i rygg()
    }
    else if (!buttonC.isPressed())
    {
        //Bytter case og starter timer for rygging
        if (millis() - trykkTidC >= KNAPPE_VENT && cKnappeTrykk)
        {
            status = 2;
            ryggeTidStart = millis();
        }
        cKnappeTrykk = 0;
    }
}
// Rygger bilen i 3 sekunder og gir 20 prosent ekstra batteri
void rygg()
{
    const unsigned long ryggeTid = 3000;
    motors.setSpeeds(-200, -200);
    if (millis() - ryggeTidStart >= ryggeTid)
    {
        prosent = constrain(prosentC + 20, 0, 100);
        status = 0;
    }
}
// buzzer forskjellige toner basert på hvor mye prosent som er igjen på batteriet
void buzz()
{
    if (prosent <= 10 && prosent >= 9)
    {
        buzzer.playFrequency(440, 500, 10);
    }
    else if (prosent <= 5 && prosent >= 4)
    {
        buzzer.playFrequency(400, 500, 15);
    }
}
// Viser displayet med ulike meldinger avhengig av verdiene til prosent og helse
void visDisplay()
{
    int prosentI = prosent;
    if (prosent <= 10 && prosent >= 9)
    {
        display.setLayout8x2();
        display.gotoXY(2, 0);
        display.print("<10%");
    }
    else if (prosent <= 5 && prosent >= 4)
    {
        helse = constrain(helse - (oppladninger * 2), 1, 100);
        display.setLayout8x2();
        display.gotoXY(2, 0);
        display.print("<5%");
    }
    else if (prosent < 2)
    {
        display.setLayout8x2();
        display.gotoXY(2, 0);
        display.print("<2%");
        display.gotoXY(0, 1);
        display.print("Trykk C");
    }
    else
    {
        display.setLayout21x8();
        display.clear();
        display.print(F("Distanse: "));
        display.print(distanse);
        display.gotoXY(0, 1);
        display.print(F("Hastighet: "));
        display.print(hastighet);
        display.gotoXY(0, 2);
        display.print(F("BatteriProsent: "));
        display.print(prosentI);
        display.gotoXY(0, 3);
        display.print(F("Batteri-helse: "));
        display.print(helse);
        display.gotoXY(0, 4);
        display.print(F("Ladinger: "));
        display.print(oppladninger);
        display.gotoXY(0,7);
        display.print(F("Saldo:"));
        display.print(saldo * 24); //konverterer fra baht til australse dollar
        display.gotoXY(10, 7);
        display.print("  BAHT");
        if (batteriFeil)
        {
            display.gotoXY(0, 5);
            display.print("Feil pa batteriet.");
            display.gotoXY(0, 6);
            display.print("Bytt batteripakke!");
        }
        else if (helse <= 60 && helse > 50)
        {
            display.gotoXY(0, 5);
            display.print("Batteri-helse <60%");
            display.gotoXY(0, 6);
            display.print("Service anbefalt");
        }
        else if (helse <= 50)
        {
            display.gotoXY(0, 5);
            display.print("batteri-helse <50%");
            display.gotoXY(0, 6);
            display.print("Bytt batteripakke!");
        }
    }
}
// følger teipet linje på bakken
void folgeLinje()
{
    long posisjon = lineSensors.readLine(linjeSensorVerdier);
    long venstreFart = 200;
    long hoyreFart = 200;

    long feil = map(posisjon - 2000, -2000, 2000, -200, 200);
    venstreFart += feil;
    hoyreFart -= feil;
    venstreFart = constrain(venstreFart, 0, MAX_FART);
    hoyreFart = constrain(hoyreFart, 0, MAX_FART);
    // Får bilen til å kjøre til høyre i kryss
    if (linjeSensorVerdier[2] == 1000 && linjeSensorVerdier[3] >= 1000 || linjeSensorVerdier[4] >= 1000)
    {
        motors.setSpeeds(300, -250);
        delay(150);
    }
    motors.setSpeeds(venstreFart, hoyreFart);
}
// Kalibrer linjefølgersensorene
void kalibrerSensorer()
{
    unsigned long start = millis();
    while (millis() - start <= 2500)
    {
        motors.setSpeeds(200, -200);
        lineSensors.calibrate();
    }
    motors.setSpeeds(0, 0);
}
// Oppdaterer minnet
void oppdaterEEPROM()
{
    EEPROM.update(HELSE_ADRESSE, helse);
    EEPROM.update(OPPLADNINGER_ADRESSE, oppladninger);
    EEPROM.update(SERVICER_ADRESSE, servicer);
    EEPROM.update(SALDO_ADRESSE, saldo);
}
#endif