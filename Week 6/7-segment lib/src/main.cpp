#include <avr/io.h>
#include <util/delay.h>
#include "seven.h"

int main()
{
  int myPins[8] = {10, 9, 2, 3, 4, 5, 6, 7};

  setSegmentPins(myPins);

  DDRC = 1;
  PORTC = 0;

  // quick test
  while (true)
  {
    for (int i = -1; i < 21; i++)
    {
      setDisplayNumber(i);
      _delay_ms(500);
    }
  }

  return 0;
}