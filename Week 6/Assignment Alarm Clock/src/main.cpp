#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <seven.h>
#include "main.h"

#define ABUTTON PINB3
#define BBUTTON PINB4
#define ABUTTONLAST 0b1
#define BBUTTONLAST 0b10
#define ABUTTONSTATE 0b100
#define BBUTTONSTATE 0b1000

int segPins[8] = {7, 6, 5, 4, 3, 2, 9, 10};
unsigned char hour = 12, minute = 0, second = 0;
unsigned int currentMillisecond = 0;
unsigned int lastButtonChange = 0;
bool buttonALastState = 0, buttonBLastState = 0, buttonAState = 0, buttonBState = 0;

ISR(TIMER0_COMPA_vect)
{
  currentMillisecond++;
}

void setup()
{
  // setup timer 0 to count millisecons
  TCCR0A = 1 << WGM01;            // timer 0 in mode 2, CTC
  TCCR0B = 1 << CS01 | 1 << CS00; // prescaler clock / 64
  OCR0A = 249;                    // every 1ms
  TIMSK0 = 1 << OCIE0A;           // activate interupt on OCR0A match
  sei();

  setSegmentPins(segPins);
  DDRC = 0b1111; // sets up A0-A3 as digital output to be able to select digit on the 4x7segment display
  PORTC = 0b1111;

  DDRB |= ~(1 << PB3) | ~(1 << PB4) | (1 << PB5);
  PORTB |= (1 << PB3) | (1 << PB4);
}

void loop()
{
  keepTime();
  display(hour * 100 + minute);
  readButtons();
}

int readButtons()
{
  bool buttonA = (PINB & 1 << ABUTTON);
  bool buttonB = (PINB & 1 << BBUTTON);

  // check if button A has changed state
  if (buttonA == 0 && buttonALastState == 0)
  {
    lastButtonChange = second;
    buttonALastState = 1;
  }
  else if (buttonA == 1 && buttonALastState == 1)
  {
    lastButtonChange = second;
    buttonALastState = 0;
  }
  else if (buttonA == 0 && buttonALastState == 1 && lastButtonChange < second)
  {
    //do thing
    PORTB |= 1 << PB5;
    _delay_ms(300);
    PORTB &= ~(1 << PB5);
  }

  // check if button A has changed state
  if (buttonB == 0 && buttonBLastState == 0)
  {
    lastButtonChange = second;
    buttonBLastState = 1;
  }
  else if (buttonB == 1 && buttonBLastState == 1)
  {
    lastButtonChange = second;
    buttonBLastState = 0;
  }
  else if (buttonB == 0 && buttonBLastState == 1 && lastButtonChange < second)
  {
    //do thing
    display(9999);
  }
  return 0;
}

void display(int nr)
{
  int numberDivisor = 1000;
  for (char i = 0; i < 4; i++)
  {
    int nrToDisplay = nr / numberDivisor;
    nr %= numberDivisor;
    numberDivisor /= 10;
    PORTC = 0b1111;
    setDisplayNumber(nrToDisplay + ((i == 1) ? 10 : 0));
    PORTC = ~(1 << i);
    _delay_ms(1);
  }
}

void keepTime()
{
  if (currentMillisecond > 999)
  {
    incrementSecond();
  }
}

void incrementSecond()
{
  currentMillisecond -= 1000;
  second++;
  if (second > 59)
  {
    incrementMinute();
  }
}

void incrementMinute()
{
  second = 0;
  minute++;
  if (minute > 59)
  {
    incrementHour();
  }
}

void incrementHour()
{
  minute = 0;
  hour++;
  if (hour > 23)
  {
    hour = 0;
  }
}

int main()
{
  setup();
  while (true)
  {
    loop();
  }
  return 0;
}