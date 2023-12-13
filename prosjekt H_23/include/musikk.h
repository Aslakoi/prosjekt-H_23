#ifndef MUSIKK_H
#define MUSIKK_H

#define BUZZER_PIN A7 
#define NUMBER_OF_NOTES 15

Zumo32U4Buzzer buzzer;

#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330 
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494

#define NOTE_SIXTEENTH 208
#define NOTE_EIGHTH 2 * NOTE_SIXTEENTH
#define NOTE_DOTTED_EIGHTH 3 * NOTE_SIXTEENTH
#define NOTE_QUARTER 4 * NOTE_SIXTEENTH
#define NOTE_DOTTED_QUARTER 6 * NOTE_SIXTEENTH
#define NOTE_HALF 8 * NOTE_SIXTEENTH
#define NOTE_WHOLE 16 * NOTE_SIXTEENTH

int anthem[NUMBER_OF_NOTES][2] = {
  {NOTE_C4, NOTE_EIGHTH},
  
  {NOTE_F4, NOTE_QUARTER},
  {NOTE_C4, NOTE_DOTTED_EIGHTH},
  {NOTE_D4, NOTE_SIXTEENTH},
  {NOTE_E4, NOTE_QUARTER},
  {NOTE_A3, NOTE_EIGHTH},
  {NOTE_A3, NOTE_EIGHTH},

  {NOTE_D4, NOTE_QUARTER},
  {NOTE_C4, NOTE_DOTTED_EIGHTH},
  {NOTE_AS3, NOTE_SIXTEENTH},
  {NOTE_C4, NOTE_QUARTER},
  {NOTE_F3, NOTE_EIGHTH},
  {NOTE_F3, NOTE_EIGHTH},

  {NOTE_G3, NOTE_QUARTER},
  {NOTE_G3, NOTE_EIGHTH}

};

//drittsang
const char mySong[] PROGMEM =
  "O5 L8 CDEFGAB "
  "O6 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
  "O5 L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f "
  "O6 L16 ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca";


void note(unsigned long frequency, unsigned long duration) 
{
  tone(BUZZER_PIN, frequency);
  delay(duration);
  noTone(BUZZER_PIN);
}

void playAnthem()
{
  for (int i = 0; i < NUMBER_OF_NOTES; i++) 
  {
    note(anthem[i][0], anthem[i][1]);
  }
  
}

void playSong()
{
  buzzer.playFromProgramSpace(mySong);
  while(buzzer.isPlaying()) {} //fra dokumentasjonen

}

#endif
