#include <avr/io.h>
#include <util/delay.h>

enum color
{
  Red,
  Yellow,
  Green,
  RedYellow
};

void turnOnLed(color color)
{
  switch (color)
  {
  case Red:
    PORTB = 1 << PB5;
    break;
  case Yellow:
    PORTB = 1 << PB4;
    break;
  case Green:
    PORTB = 1 << PB3;
    break;
  case RedYellow:
    PORTB = 1 << PB5 | 1 << PB4;
  default:
    PORTB = 0;
    break;
  };
}

void cycleTrafficlight()
{
  turnOnLed(Red);
  _delay_ms(3000);
  turnOnLed(Yellow);
  _delay_ms(2000);
  turnOnLed(Green);
  _delay_ms(5000);
  turnOnLed(RedYellow);
  _delay_ms(2000);
  turnOnLed(Red);
}

int main()
{
  DDRB = 0B00111000; // activate output on uno ports 11-13
  
  cycleTrafficlight();
}