#include "SH1106.h"

SH1106::SH1106(int i2cAddress) {
  myI2cAddress = i2cAddress;
  }

void SH1106::init() {
  sendInitCommands();
  resetDisplay();
}

void SH1106::resetDisplay(void) {
  displayOff();
  clear();
  display();
  displayOn();
}

void SH1106::displayOn(void) {
    sendCommand(0xaf);        //display on
}

void SH1106::displayOff(void) {
  sendCommand(0xae);          //display off
}

void SH1106::setContrast(char contrast) {
  sendCommand(0x81);
  sendCommand(contrast);
}

void SH1106::clear(void) {
    memset(buffer, 0, (128 * 64 / 8));
}

void SH1106::display(void) {
    sendCommand(COLUMNADDR);
    sendCommand(0x00);
    sendCommand(0x7F);

    sendCommand(PAGEADDR);
    sendCommand(0x0);
    sendCommand(0x7);

    sendCommand(SETSTARTLINE | 0x00);

      i2c_start();
      i2c_write8(myI2cAddress << 1);
      i2c_write8(0x40);

      for (uint8_t x=0; x<16; x++) {
        i2c_write8(buffer[i]);
        i++;
      }

      i--;
      yield();
      i2c_stop();

    }

void SH1106::setPixel(int x, int y) {
  if (x >= 0 && x < 128 && y >= 0 && y < 64) {
      buffer[x + (y/8)*128] |=  (1 << (y&7));
  }
}

void SH1106::drawBitmap(int x, int y, int width, int height, const char *bitmap) {
  for (int i = 0; i < width * height / 8; i++ ){
    unsigned char charColumn = 255 - pgm_read_byte(bitmap + i);
    for (int j = 0; j < 8; j++) {
      int targetX = i % width + x;
      int targetY = (i / (width)) * 8 + j + y;
      if (bitRead(charColumn, j)) {
        setPixel(targetX, targetY);
      }
    }
  }
}

void SH1106::drawXbm(int x, int y, int width, int height, const char *xbm) {
  if (width % 8 != 0) {
    width =  ((width / 8) + 1) * 8;
  }
  for (int i = 0; i < width * height / 8; i++ ){
    unsigned char charColumn = pgm_read_byte(xbm + i);
    for (int j = 0; j < 8; j++) {
      int targetX = (i * 8 + j) % width + x;
      int targetY = (8 * i / (width)) + y;
      if (bitRead(charColumn, j)) {
        setPixel(targetX, targetY);
      }
    }
  }
}

void SH1106::sendCommand(unsigned char com) {
   i2c_start();
   i2c_write8(myI2cAddress << 1);     //begin transmitting
   i2c_write8(0x80);                  //command mode
   i2c_write8(com);
   i2c_stop();                        // stop transmitting
  }

void SH1106::sendInitCommands(void) {
  sendCommand(DISPLAYOFF);
  sendCommand(NORMALDISPLAY);
  sendCommand(SETDISPLAYCLOCKDIV);
  sendCommand(0x80);
  sendCommand(SETMULTIPLEX);
  sendCommand(0x3F);
  sendCommand(SETDISPLAYOFFSET);
  sendCommand(0x00);
  sendCommand(SETSTARTLINE | 0x00);
  sendCommand(CHARGEPUMP);
  sendCommand(0x14);
  sendCommand(MEMORYMODE);
  sendCommand(0x00);
  sendCommand(SEGREMAP);
  sendCommand(COMSCANDEC);
  sendCommand(SETCOMPINS);
  sendCommand(0x12);
  sendCommand(SETCONTRAST);
  sendCommand(0xCF);
  sendCommand(SETPRECHARGE);
  sendCommand(0xF1);
  sendCommand(SETVCOMDETECT);
  sendCommand(0x40);
  sendCommand(DISPLAYALLON_RESUME);
  sendCommand(NORMALDISPLAY);
  sendCommand(0x2e);            // stop scroll
  sendCommand(DISPLAYON);
}