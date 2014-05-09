#ifndef _MEGA_24_SHIELD_
#define _MEGA_24_SHIELD_

// Code provided by Smoke And Wires
// http://www.smokeandwires.co.nz
// This code has been taken from the Adafruit TFT Library and modified
//  by us for use with our TFT Shields / Modules
// For original code / licensing please refer to
// https://github.com/adafruit/TFTLCD-Library
 

// This header file serves two purposes:
//
// 1) Isolate non-portable MCU port- and pin-specific identifiers and
//    operations so the library code itself remains somewhat agnostic
//    (PORTs and pin numbers are always referenced through macros).
//
// 2) GCC doesn't always respect the "inline" keyword, so this is a
//    ham-fisted manner of forcing the issue to minimize function calls.
//    This sometimes makes the library a bit bigger than before, but fast++.
//    However, because they're macros, we need to be SUPER CAREFUL about
//    parameters -- for example, write8(x) may expand to multiple PORT
//    writes that all refer to x, so it needs to be a constant or fixed
//    variable and not something like *ptr++ (which, after macro
//    expansion, may increment the pointer repeatedly and run off into
//    la-la land).  Macros also give us fine-grained control over which
//    operations are inlined on which boards (balancing speed against
//    available program space).




// Smoke And Wires 2.4 Shield pin usage:
// LCD Data Bit :    7    6    5    4    3    2    1    0
// Digital pin #:    7    6    5    4    3    2    9    8
// Uno port/pin :  PD7  PD6  PD5  PD4  PD3  PD2  PB1  PB0
// Mega port/pin   PH4	PH3	 PE3  PG5  PE5  PE4  PH6  PH5		

#define LCD_CS A3 // Chip Select 
#define LCD_CD A2// Command/Data 
#define LCD_WR A1// LCD Write 
#define LCD_RD A0// LCD Read 

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// Pixel read operations require a minimum 400 nS delay from RD_ACTIVE
// to polling the input pins.  At 16 MHz, one machine cycle is 62.5 nS.
// This code burns 7 cycles (437.5 nS) doing nothing; the RJMPs are
// equivalent to two NOPs each, final NOP burns the 7th cycle, and the
// last line is a radioactive mutant emoticon.
#define DELAY7        \
  asm volatile(       \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "nop"      "\n"   \
    ::);


    
 // Because the MEGA port to pin mapping is very messy it is necessary to shift the data bits around a lot.
  #define write8inline(d) {                          \
    PORTE = (PORTE & B11001111) | ((d << 2) & B00110000); \
	PORTE = (PORTE & B11110111) | ((d >> 2) & B00001000); \
	PORTG = (PORTG & B11011111) | ((d << 1) & B00100000); \
	PORTH = (PORTH & B11100111) | ((d >> 3) & B00011000); \
	PORTH = (PORTH & B10011111) | ((d << 5) & B01100000); \
	WR_STROBE; }
  #define read8inline(result) {                       \
    RD_ACTIVE;                                        \
    DELAY7;                                           \
    result = ((PINH & B00011000) << 3) | ((PINE & B00001000) << 2) | ((PING & B00100000) >> 1) |((PINE & B00110000) >> 2) | ((PINH & B01100000) >> 5); \
    RD_IDLE; }

 // // These set the PORT directions as required before the write and read
 // // operations.
  #define setWriteDirInline() { DDRE |=  B00111000; DDRG |=  B00100000; DDRH |= B01111000;}
  #define setReadDirInline() { DDRE &=  ~B00111000; DDRG &=  ~B00100000; DDRH &= ~B01111000;}
  
 
 

  // As part of the inline control, macros reference other macros...if any
  // of these are left undefined, an equivalent function version (non-inline)
  // is declared later.  The Uno has a moderate amount of program space, so
  // only write8() is inlined -- that one provides the most performance
  // benefit, but unfortunately also generates the most bloat.  This is
  // why only certain cases are inlined for each board.
  #define write8 write8inline



 // When using the TFT breakout board, control pins are configurable.
 #define RD_ACTIVE  *rdPort &=  rdPinUnset
 #define RD_IDLE    *rdPort |=  rdPinSet
 #define WR_ACTIVE  *wrPort &=  wrPinUnset
 #define WR_IDLE    *wrPort |=  wrPinSet
 #define CD_COMMAND *cdPort &=  cdPinUnset
 #define CD_DATA    *cdPort |=  cdPinSet
 #define CS_ACTIVE  *csPort &=  csPinUnset
 #define CS_IDLE    *csPort |=  csPinSet

// #endif
// #endif

// Data write strobe, ~2 instructions and always inline
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }

// These higher-level operations are usually functionalized,
// except on Mega where's there's gobs and gobs of program space.

// Set value of TFT register: 8-bit address, 8-bit value
#define writeRegister8inline(a, d) { \
  CD_COMMAND; write8(a); CD_DATA; write8(d); }

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeRegister16inline(a, d) { \
  uint8_t hi, lo; \
  hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
  hi = (d) >> 8; lo = (d); CD_DATA   ; write8(hi); write8(lo); }

// Set value of 2 TFT registers: Two 8-bit addresses (hi & lo), 16-bit value
#define writeRegisterPairInline(aH, aL, d) { \
  uint8_t hi = (d) >> 8, lo = (d); \
  CD_COMMAND; write8(aH); CD_DATA; write8(hi); \
  CD_COMMAND; write8(aL); CD_DATA; write8(lo); }

#endif // _MEGA_24_SHIELD_
