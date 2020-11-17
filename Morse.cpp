/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

Morse::Morse(uint8_t pin, uint8_t ddelay, uint8_t pattern)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _delay = ddelay;
  _pattern = pattern;
}


uint8_t Morse::getDelay()
{
  return _delay;
}


void Morse::patternNext()
{
  if ( _idx == 0 ) {
    digitalWrite(_pin, LOW);
    delay(_delay);
  }
  
  uint8_t val = bitRead(_pattern, _idx++);
  if ( _idx >= 7 )
    _idx = 0;
  digitalWrite(_pin, val);
//  delay(_delay);
}

void Morse::pattern(uint8_t pattern)
{
  digitalWrite(_pin, LOW);
  delay(_delay);
  for ( uint8_t i = 0; i < 8; i++ ){
    uint8_t val = bitRead(pattern, i);
    digitalWrite(_pin, val);
    delay(_delay);
  }  
}

void Morse::dot()
{
  digitalWrite(_pin, HIGH);
  delay(_delay);
  digitalWrite(_pin, LOW);
  delay(_delay);  
}

void Morse::dash()
{
  digitalWrite(_pin, HIGH);
  delay(4*_delay);
  digitalWrite(_pin, LOW);
  delay(_delay);
}
