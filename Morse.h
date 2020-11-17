/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class Morse
{
  public:
    Morse(uint8_t pin, uint8_t ddelay, uint8_t pattern);
    void dot();
    void dash();
    void pattern(uint8_t pattern);
    void patternNext();
    uint8_t getDelay();
  private:
    uint8_t _pin;
    uint8_t _delay;
    uint8_t _pattern;
    uint8_t _idx;    
};

#endif
