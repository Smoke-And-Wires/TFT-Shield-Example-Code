// Code provided by Smoke And Wires
// http://www.smokeandwires.co.nz
// This code has been taken from the Adafruit TFT Library and modified
//  by us for use with our TFT Shields / Modules
// For original code / licensing please refer to
// https://github.com/adafruit/TFTLCD-Library

// IMPORTANT: SEE COMMENTS @ LINE 15 REGARDING SHIELD VS BREAKOUT BOARD USAGE.

// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef _SWTFT_H_
#define _SWTFT_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_GFX.h>


class SWTFT : public Adafruit_GFX {

 public:

  SWTFT(void);
 // SWTFT(void);

  void     begin(uint16_t id = 0x9325);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  /////////////////////////////////////////
  void     writeA16B3(uint16_t address, uint8_t ub, uint8_t mb, uint8_t lb);
  void     drawPixel3(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);
  void     drawLine3Byte(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t r, uint8_t g, uint8_t b) ;
  //////////////////////////////////////////
  void     drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
  void     drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
  void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
  void     fillScreen(uint16_t color);
  void     reset(void);
  void     setRegisters8(uint8_t *ptr, uint8_t n);
  void     setRegisters16(uint16_t *ptr, uint8_t n);
  void     setRotation(uint8_t x);
       // These methods are public in order for BMP examples to work:
  void     setAddrWindow(int x1, int y1, int x2, int y2);
  void     pushColors(uint16_t *data, uint8_t len, boolean first);

      // swap
  void swap(int16_t *a, int16_t *b);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b),
           readPixel(int16_t x, int16_t y),
           readID(void);

 private:

  void     init(),
           // These items may have previously been defined as macros
           // If not, function versions are declared:
#ifndef write8
           write8(uint8_t value),
#endif
#ifndef setWriteDir
           setWriteDir(void),
#endif
#ifndef setReadDir
           setReadDir(void),
#endif
#ifndef writeRegister8
           writeRegister8(uint8_t a, uint8_t d),
#endif
#ifndef writeRegister16
           writeRegister16(uint16_t a, uint16_t d),
#endif
#ifndef writeRegisterPair
           writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d),
#endif
           setLR(void),
           flood(uint16_t color, uint32_t len);
  uint8_t  driver;

#ifndef read8
  uint8_t  read8fn(void);
  #define  read8isFunctionalized
#endif

#ifndef USE_ADAFRUIT_SHIELD_PINOUT

  #ifdef __AVR__
    volatile uint8_t *csPort    , *cdPort    , *wrPort    , *rdPort;
	uint8_t           csPinSet  ,  cdPinSet  ,  wrPinSet  ,  rdPinSet  ,
					  csPinUnset,  cdPinUnset,  wrPinUnset,  rdPinUnset,
					  _reset;
  #endif
  #if defined(__SAM3X8E__)
    Pio *csPort    , *cdPort    , *wrPort    , *rdPort;
	uint32_t          csPinSet  ,  cdPinSet  ,  wrPinSet  ,  rdPinSet  ,
					  csPinUnset,  cdPinUnset,  wrPinUnset,  rdPinUnset,
					  _reset;
  #endif

#endif
};

// For compatibility with sketches written for older versions of library.
// Color function name was changed to 'color565' for parity with 2.2" LCD
// library.
#define Color565 color565

#endif //_SWTFT_H_
