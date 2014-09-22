/* The MIT License (MIT)
 *
 * Copyright (c) 2014 to nils minor (contact nilsminor@web.de)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 /* This is an example of how to use the CESI-32/LX with openRotary libary.
 * I wrote this libary to make it easier to handle the rotary encoder stuff.
 * I am using this rotary encoder in a current projekt for controlling menu, 
 * the CESI-32 is easy to use and industrial improved with 2 x10^6 rotations.
 * 
 * here is some info about the rotary encoder:
 * CESI-32/LX (CESI L: 5 V– /10 mA) http://www.ebe-gmbh.de/cont/de/ebe/pro/itro/CESI.html 
 * datasheet http://www.ebe-gmbh.de/load/ebe/i-tronic/de/ebe_i-tro_de_CSI.pdf
 * 
 * CESI-32 Pin  function        Arduino Micro
 *          1      5V             5V-Pin
 *          2      GND            GND-Pin
 *          4      direction      A3                         
 *          5      rotation       A3
 *          7      button1        5V
 *          8      button2        A4 (pulled with 4k7 down)
 * 
 * so have fun with it :)
*/

#include "Arduino.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "openRotary.h"

OpenRotary::OpenRotary (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin)
{
                          rotaryCountSupported      = false;    // rotary count used
                          rotaryButtonSupported     = false;    // rotary button supported
                                                
                          rotaryInit ( _rotaryStatePin,  _rotaryRotationDirectionPin,
                                       NULL, NULL);
}
OpenRotary::OpenRotary (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin, int32_t _rotaryCount)
{
                          rotaryCountSupported      = true;    // rotary count used
                          rotaryButtonSupported     = false;   // rotary button supported
                          
                          // Boundaries by default could be change by setRotaryCountBoundaries (min,max)
                          rotaryMaxCount = 100;
                          rotaryMinCount = 0;
                          
                          rotaryInit ( _rotaryStatePin,  _rotaryRotationDirectionPin,
                                       NULL, _rotaryCount);
}
OpenRotary::OpenRotary (uint8_t _rotaryStatePin,  uint8_t _rotaryRotationDirectionPin,
                        uint8_t _rotaryButtonPin, int32_t _rotaryCount) {
                        
                          rotaryCountSupported      = true;    // rotary count used
                          rotaryButtonSupported     = true;    // rotary button supported
                          
                          // Boundaries by default could be change by setRotaryCountBoundaries (min,max)
                          rotaryMaxCount = 100;
                          rotaryMinCount = 0;
                          
                          rotaryInit ( _rotaryStatePin,  _rotaryRotationDirectionPin,
                                       _rotaryButtonPin, _rotaryCount);
                        }
                        
void OpenRotary::rotaryInit (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin,
                             uint8_t _rotaryButtonPin, int32_t _rotaryCount) {
                          
                          rotaryStatePin             = _rotaryStatePin;
                          rotaryRotationDirectionPin = _rotaryRotationDirectionPin;
                          rotaryButtonPin            = _rotaryButtonPin;
                                   
                          pinMode (rotaryStatePin,                   INPUT);
                          pinMode (rotaryRotationDirectionPin,       INPUT);
                          if (rotaryButtonSupported)
                            pinMode (rotaryButtonPin,                INPUT);
                            
                          rotaryCount               = _rotaryCount;
                          rotaryLastDebounceTime    = 0;
                          rotaryButtonMind          = LOW;
                          lastRotaryButtonState     = LOW;
                          rotaryButtonDebounceDelay = 50;
                            
                          rotaryLastState           = LOW;
                          rotaryDebounceTime        = 0;

                        }    
     

void OpenRotary::setRotaryButtonCallback  (void (*_rotaryButtonCallback)(void))
{
  rotaryButtonCallback = _rotaryButtonCallback;
}

void OpenRotary::setRotaryCountBoundaries  (int _rotaryMinCount, int _rotaryMaxCount)
{
  if (_rotaryMinCount > -32768 && _rotaryMaxCount < 32767)
  {
    rotaryMaxCount = _rotaryMaxCount;
    rotaryMinCount = _rotaryMinCount;
  }
}

void OpenRotary::setRotaryRotationCallback (void (*_rotaryTurnRightCallback)(void), void (*_rotaryTurnLeftCallback)(void))
{
  rotaryTurnRightCallback = _rotaryTurnRightCallback;
  rotaryTurnLeftCallback  = _rotaryTurnLeftCallback;
}

void OpenRotary::rotaryButtonCheck (void)
{
  int reading = digitalRead(rotaryButtonPin);                         // Einlesen des Tasterzustandes
  if (reading != lastRotaryButtonState) {                             // Taster gedrückt
    rotaryLastDebounceTime = millis();                                // aktuelle Zeit merken
    rotaryButtonMind = LOW;                                           // Tastergedächtnis zurück setzen
  }
  if (  (reading == HIGH)                                               // Taster gedrückt   
  && ((millis() - rotaryLastDebounceTime) > rotaryButtonDebounceDelay)      // && Entprellzeit verstrichen  
  ){ 
    if (rotaryButtonMind == LOW) {                                    // Merken das der Taster bereits gedrückt wurde
      rotaryButtonCallback ( );
     
      rotaryButtonMind = HIGH;                                        // Tastergedächtnis wird aktiviert
    }
  } 
  lastRotaryButtonState = reading;                                    // lastButtonState wird auf den aktuellen Stand gebracht
}           


int OpenRotary::getRotaryCount (void)
{
 return rotaryCount; 
}

void OpenRotary::check (void)
{
  rotaryButtonCheck   ( );
  rotaryRotationCheck ( );
}

void OpenRotary::rotaryRotationCheck (void)
{
  int reading = digitalRead(rotaryStatePin);
  if (reading != rotaryLastState)
  {
    if(reading == HIGH)
    {
      int readingLeftRight = digitalRead(rotaryRotationDirectionPin);
      if (readingLeftRight == LOW)         // Turn right
      {
        leftRightDirection = true;
        if ((rotaryCountSupported == true) && (rotaryCount < rotaryMaxCount))
           rotaryCount++;
           
        rotaryTurnRightCallback ( );     
         
      }
      else
      {   
        leftRightDirection = false; 
        if ((rotaryCountSupported == true) && (rotaryCount > rotaryMinCount))
           rotaryCount--;
           
        rotaryTurnLeftCallback ( );        // runterdrehen
      }
    }
  }
  rotaryLastState = reading;
}

bool OpenRotary::getDirection            (void) {
  if (leftRightDirection)  return true;
  else                     return false;
}
void OpenRotary::setRotaryCount  (uint8_t _rotaryCount) {
  rotaryCount = _rotaryCount;
}

