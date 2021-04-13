#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "main.h"

#define DIG1 PC0
#define DIG2 PC1
#define DIG3 PC2
#define DIG4 PC3

#define BUTTON PB4

enum Times
{
  ReactionTimerStart,
  RandomTimerStart,
  AnimationTimer,
  DebounceTimer,
  NTimers
};
enum Modes
{
  Animating,
  ReactionTest,
  ShowingResult
};

// contains 0-9 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
const unsigned char digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b00111111, 0b11100000, 0b11111110, 0b11110110};
unsigned char displayedDigit[4] = {0};

bool buttonLastState = 0;

unsigned long timers[NTimers] = {0};
unsigned long currentMillisecond = 0;

unsigned char animationStep = 0;
unsigned char digit = 0;
unsigned char mode = 0;

ISR(TIMER0_COMPA_vect)
{
  currentMillisecond++;
}

int main()
{
  setup();

  while (1)
  {
    loop();
  }

  return 0;
}

void setup()
{
  // setup timer 0 to count millisecons
  TCCR0A = 1 << WGM01;            // timer 0 in mode 2, CTC
  TCCR0B = 1 << CS01 | 1 << CS00; // prescaler clock / 64
  OCR0A = 249;                    // every 1ms
  TIMSK0 = 1 << OCIE0A;           // activate interupt on OCR0A match
  sei();

  DDRB = 0;            // button input
  DDRC = 0xff;         // digit selection output
  DDRD = 0xff;         // segments output
  PORTB = 1 << BUTTON; // enable internal pull-up on uno pin 12

  timers[RandomTimerStart] = currentMillisecond + randomTime();
}

void loop()
{
  switch (mode)
  {
  case Animating:
    if (currentMillisecond < timers[RandomTimerStart])
    {
      animate();
    }
    else
    {
      mode++;
      PORTC = 0;
      PORTD = digits[8];
      timers[ReactionTimerStart] = currentMillisecond;
    }
    break;

  case ReactionTest:
    if (readButton())
    {
      separateNumbers(currentMillisecond - timers[ReactionTimerStart]);
      timers[DebounceTimer] = currentMillisecond + 1000;
      mode++;
    }
    break;

  case ShowingResult:
    displayReactionTime();

    if (readButton() && currentMillisecond > timers[DebounceTimer])
    {
      mode = 0;
      timers[RandomTimerStart] = currentMillisecond + randomTime();
    }
    break;

  default:
    mode = 0;
    timers[RandomTimerStart] = currentMillisecond + randomTime();
    break;
  }
}

bool readButton()
{
  if ((PINB & 1 << BUTTON) == 0 && buttonLastState == 0) // check if uno pin 12 is pulled to ground
  {
    buttonLastState = 1;
    return true;
  }
  else
  {
    buttonLastState = 0;
    return false;
  }
}

void animate()
{
  PORTC = 0;
  PORTD = 0b10000000 >> animationStep;
  if (currentMillisecond > timers[AnimationTimer])
  {
    timers[AnimationTimer] = currentMillisecond + 100;
    animationStep++;

    if (animationStep >= 6)
    {
      animationStep = 0;
    }
  }
}

// returns a number between 1-6
int randomTime()
{
  int result;
  do
  {
    result = rand() & 0b11111111111111; // randomize numbers between 0-16383
  } while (result > 10000 || result < 3000);
  return result;
}

void separateNumbers(unsigned int timeInMs)
{
  unsigned char number[4] = {0};
  unsigned int modulo = 1000;
  for (int i = 0; i < 4; i++)
  {
    number[i] = char(timeInMs / modulo);
    timeInMs %= modulo;
    modulo /= 10;
    displayedDigit[i] = digits[number[i]]; // save the digit, the first digit gets a decimal point added on the display
  }
  displayedDigit[0] |= 1;
}

void displayReactionTime()
{
  PORTC = 0xff;
  PORTD = displayedDigit[digit];
  PORTC = 0xff & ~(1 << digit);

  digit++;

  if (digit > 3)
    digit = 0;
}
