#include <avr/io.h>
#include <util/delay.h>

// contains 0-9 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
unsigned char digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b00111111, 0b11100000, 0b11111110, 0b11110110};

int main()
{
  DDRD = digits[8];
  for (unsigned char i = 9; i >= 0; i--)
  {
    PORTD = digits[i];
    _delay_ms(1000);
  }
  return 0;
}