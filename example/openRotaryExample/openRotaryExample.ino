#include "openRotary.h"

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

// Arduino micro input-pins to connect to CESI-32/LX
#define ROTARY_STATE_PIN         A3
#define ROTARY_DIRECTION_PIN     A5
#define ROTARY_BUTTON_PIN        A4
#define ROTARY_BUTTON_DEBOUNCE   50

OpenRotary rotary = OpenRotary (ROTARY_STATE_PIN, ROTARY_DIRECTION_PIN, ROTARY_BUTTON_PIN, 0);    // make the rotary encoder object 

void callbackRotaryButtonPressed (void)            // handler which will be called if the rotary button is pressed
{
  Serial.println("callbackRotaryButtonPressed");
}
void callbackRotaryTurnRight (void)                // handler which will be called if the rotary encoder is turned right
{
  Serial.print("callbackRotaryTurnRight");
  Serial.print("  rotaryCount: ");
  Serial.println(rotary.getRotaryCount ( ));
}
void callbackRotaryTurnLeft (void)                 // handler which will be called if the rotary encoder is turned right
{
  Serial.print("callbackRotaryTurnLeft");
  Serial.print("   rotaryCount: ");
  Serial.println(rotary.getRotaryCount ( ));
}


void setup() {
  
  Serial.begin (115200);
  rotary.setRotaryCountBoundaries (-10,10);                            // set rotary counter boundaries 
  rotary.setRotaryButtonCallback  (&callbackRotaryButtonPressed);      // set callback for button press
  rotary.setRotaryRotationCallback(&callbackRotaryTurnRight,           // set callback for rotation
                                   &callbackRotaryTurnLeft);


}

void loop() {
  rotary.check ( );    // in loop just check the rotary (maybe i will change it in futre to support intterupts but not needed in the moment)

  
}


