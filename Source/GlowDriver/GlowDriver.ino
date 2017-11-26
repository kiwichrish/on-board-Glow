/*
MIT License

Copyright (c) 2017 Chris Hellyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

// For an ATtiny85...

//#define DEBUG

const uint8_t _LED = 4;  // LED output.
const uint8_t _INPUT = 3; // the servo input.
const uint8_t _OUTPUT = 0; // the mosfet.

uint32_t pulse_length = 0; // the pulselegth

// Watchdog code, from avr libc
#include <avr/wdt.h>

void setup()
{
  // Clear WDT to make sure we don't get kicked straight back out.
  MCUSR &= ~(1 << WDRF); // reset status flag  // FIXME: do something about the flag being set?
  wdt_disable();

#ifdef DEBUG
  Serial.begin(115200);
#endif

  pinMode(_LED, OUTPUT);
  pinMode(_OUTPUT, OUTPUT);

  wdt_enable(WDTO_8S);  // eight second WDT

}

void loop()
{
  pulse_length = pulseIn(_INPUT, HIGH, 50000); // 50ms timeout, looking for high pulse.

#ifdef DEBUG
  Serial.println(pulse_length);
#endif

  if (pulse_length == 0) // if no pulse blink fast, turn off output.
  {
    digitalWrite(_OUTPUT, LOW);
    digitalWrite(_LED, HIGH);
    delay(80);
    digitalWrite(_LED, LOW);
    delay(80);
  }
  else if (pulse_length > 1500) // on. output on, blink mostly on.
  {
    digitalWrite(_OUTPUT, HIGH);

    digitalWrite(_LED, HIGH);
    delay(400);
    digitalWrite(_LED, LOW);
    delay(40);
  }
  else // off, output off blink mostly off.
  {
    digitalWrite(_OUTPUT, LOW);

    digitalWrite(_LED, HIGH);
    delay(40);
    digitalWrite(_LED, LOW);
    delay(400);
  }

  wdt_reset();
}
