#ifndef DISPLAY_SPEED_H
#define DISPLAY_SPEED_H

#include <U8g2lib.h> //bibliotek for oled skjerm

//spesifikk initialisering for min skjer, hentet fra kode tidligere i semsesteret
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI display(U8G2_R0, /* clock=*/13, /* mos=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);


void dizplay(float speed)
{
  if (speed > 10 && speed < 150) //bare vis fart hvis den er realistisk for en bil
  {
    display.firstPage();
    do
    {
      display.drawStr(5, 15, "DIN FART:");
      display.drawStr(5, 50, String(speed).c_str());
      display.drawStr(50, 50, "cm/s");
    } while (display.nextPage());
  }
}

#endif