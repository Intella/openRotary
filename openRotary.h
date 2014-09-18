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

#ifndef _OPEN_ROTARY_H_
#define _OPEN_ROTARY_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

typedef void (*Callback)(void);   // callback function will look like "void callbackFuntion (void)"

class OpenRotary {
  public:
   OpenRotary (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin);                                                    // Constructor for rotary encoder with [left/right-pin] and [state-pin]
   OpenRotary (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin, int32_t _rotaryCount);                              // Constructor for rotary encoder with [left/right-pin] , [state-pin] and [rotary-counter]
   OpenRotary (uint8_t _rotaryStatePin, uint8_t _rotaryRotationDirectionPin, uint8_t _rotaryButtonPin, int32_t _rotaryCount);    // Constructor for rotary encoder with [left/right-pin] , [state-pin] , [rotary-button-pin] and [rotary-counter]
   void check (void);
  
   void setRotaryButtonCallback     (void (*_rotaryButtonCallback)(void));             // set callback function for buttonpress, passed function will be called if rotary button is pressed                                                
   void setRotaryRotationCallback   (void (*_rotaryTurnRightCallback)(void),           // set callback function for rotation, passed function will be called if rotary is turned left or right
                                     void (*_rotaryTurnLeftCallback)(void));  
   void setRotaryCountBoundaries    (int _rotaryMinCount, int _rotaryMaxCount);        // set boundaries of the button counter (from minCount to MaxCount)
   int  getRotaryCount              (void);                                            // returns the current rotaryCount
  
  private:
    void (*rotaryButtonCallback)    (void);                                           
    void (*rotaryTurnRightCallback) (void);
    void (*rotaryTurnLeftCallback)  (void);
    
    void      rotaryInit            (uint8_t _rotaryStatePin,                          // initializes the rotary will be called from constructor
                                     uint8_t _rotaryRotationDirectionPin,
                                     uint8_t _rotaryButtonPin, int32_t _rotaryCount);
    void      rotaryButtonCheck     (void);                                            // checks in loop the rotary button
    void      rotaryRotationCheck   (void);                                            // checks in loop the rotary rotation
      
    uint8_t   rotaryStatePin;                   // pin which will be toggled if rotary totates
    uint8_t   rotaryLastState;                  // value to store last H/L State
    uint8_t   rotaryRotationDirectionPin;       // pin which indicates the rotation direction (LOW = right, HIGH = left)
    bool      rotaryCountSupported;             // true if internal rotary counter is used
    int32_t   rotaryCount;                      // rotary counter can be used to count the rotation steps
    int       rotaryMaxCount;                   // upper boundarie for rotary counter
    int       rotaryMinCount;                   // lower boundarie for rotary counter
    int       rotaryDebounceTime;               // TODO debounce rotary ???
    bool      rotaryButtonSupported;            // true if button is supported
    uint8_t   rotaryButtonPin;                  // arduino pin connected to the rotary encoder pin for the button
    int       rotaryButtonMind;                 // value to store the button state 
    int       rotaryButtonDebounceDelay;        // button debounce delay in ms
    int       rotaryLastDebounceTime;           // value which holds the last debounce time
    int       lastRotaryButtonState;            // value which stores the last rotary button state
   
};

#endif
