#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// contains 0-9 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
unsigned char digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b00111111, 0b11100000, 0b11111110, 0b11110110};

bool buttonLastState = 0;

// returns a number between 1-6
int rollDice()
{
  int result;
  do
  {
    result = rand() & 0b111; // randomize numbers between 0-7, have not tested if results are biased
  } while (result > 6 || result < 1);
  return result;
}

int main()
{
  DDRB = 0;
  DDRD = 255;
  PORTB = 1 << PB4; // enable internal pull-up on uno pin 12

  while (1)
  {
    if ((PINB & 1 << PB4) == 0) // check if uno pin 12 is pulled to ground
    {
      _delay_ms(50); // debouncing
      if ((PINB & 1 << PB4) == 0 && !buttonLastState)
      {
        buttonLastState = true;
        PORTD = digits[rollDice()];
      }
    }
    else
    {
      buttonLastState = false;
    }
  }

  return 0;
}