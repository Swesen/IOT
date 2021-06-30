#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <seven.h>
#include "main.h"

#define ABUTTON PINB3
#define BBUTTON PINB4
#define BUZZER PINB5

// segment pins for display
int segPins[8] = {7, 6, 5, 4, 3, 2, 9, 10};

// timekeeping variables
char hour = 12, minute = 0;
unsigned int millisecond = 0;

// alarm variables
char alarmHour = 12, alarmMinute = 0;
bool alarmEnabled = 0, alarmActive = 0;

// button state variables
unsigned char lastButtonStatus = 0;
unsigned long lastButtonStatusChange = 0;

// system variables
unsigned long currentMillisecond = 0;
unsigned char mode = 0;
// the MS to
unsigned long nextHoldRepeat = 0;

const unsigned int buttonDebounceMS = 50,
                   timeBetweenHoldInputs = 500,
                   longPress = 2000,
                   blinkingDisplayInterval = 250;

ISR(TIMER0_COMPA_vect)
{
  currentMillisecond++;
  millisecond++;
}

void setup()
{
  // setup timer 0 to count millisecons
  TCCR0A = 1 << WGM01;                // timer 0 in mode 2, CTC
  TCCR0B = (1 << CS01) | (1 << CS00); // prescaler clock / 64
  OCR0A = 249;                        // every 1ms
  TIMSK0 = 1 << OCIE0A;               // activate interupt on OCR0A match
  sei();

  setSegmentPins(segPins);
  DDRC = 0b1111; // sets up A0-A3 as digital output to be able to select digit on the 4x7segment display
  PORTC = 0b1111;

  DDRB &= ~(1 << ABUTTON) & ~(1 << BBUTTON);
  DDRB |= (1 << BUZZER);
  PORTB |= (1 << ABUTTON) | (1 << BBUTTON);
}

void loop()
{
  keepTime();

  input();

  alarm();

  clockDisplay();
}

void input()
{
  char currentButtonStatus = readButtons();

  // new button pressed or released
  if (currentButtonStatus != lastButtonStatus)
  {
    lastButtonStatusChange = currentMillisecond;
    lastButtonStatus = currentButtonStatus;
    nextHoldRepeat = currentMillisecond;
  }
  // a button has been held longer than debounce threshold
  else if (currentButtonStatus != 0 && lastButtonStatusChange + buttonDebounceMS < currentMillisecond && nextHoldRepeat < currentMillisecond)
  {
    handleButtonPresses(currentButtonStatus);

    nextHoldRepeat = currentMillisecond + timeBetweenHoldInputs;
  }

  // in case of currentMillisecond overflow about every 50 day
  if (currentMillisecond < nextHoldRepeat - timeBetweenHoldInputs || currentMillisecond < lastButtonStatusChange)
  {
    nextHoldRepeat = currentMillisecond;
    lastButtonStatusChange = currentMillisecond;
  }
}

void handleButtonPresses(char buttonStatus)
{
  switch (buttonStatus)
  {
  case 1:
  case 2:
    switch (mode)
    {
    case 0:
      if (alarmActive)
      {
        alarmActive = false;
        alarmEnabled = false;
      }
      else
      {
        return;
      }

      break;

    case 1:
      hour = changeOnInput(buttonStatus, hour, 0, 23);
      break;

    case 2: // set minute mode
      minute = changeOnInput(buttonStatus, minute, 0, 59);
      break;

    case 3: // set alarm hour
      alarmHour = changeOnInput(buttonStatus, alarmHour, 0, 23);
      break;

    case 4: // set alarm minute
      alarmMinute = changeOnInput(buttonStatus, alarmMinute, 0, 59);
      break;

    case 5: // set alarm enabled
      alarmEnabled = !alarmEnabled;
      break;

    default:
      mode = 0;
      break;
    }
    makeSound(20);

    break;

  case 3:
    if (lastButtonStatusChange + longPress < currentMillisecond) // both buttons held longer than long press
    {
      mode++;
      makeSound(20);
      _delay_ms(50);
      makeSound(20);
    }
    break;

  default:
    break;
  }
}

void alarm()
{
  if (alarmEnabled && (hour == alarmHour && minute == alarmMinute))
  {
    alarmActive = true;
    if (((millisecond / 100) % 2) == 0)
    {
      PORTB |= 1 << PB5;
    }
    else
    {
      PORTB &= ~(1 << PB5);
    }
  }
}

void alternatingDisplay(char hour, char altHour, char minute, char altMinute, bool alarmDot, bool altAlarmDot)
{
  if (((millisecond / blinkingDisplayInterval) % 2) == 0)
  {
    display(hour, minute, alarmDot);
  }
  else
  {
    display(altHour, altMinute, altAlarmDot);
  }
}

void clockDisplay()
{
  switch (mode)
  {
  case 0: // clock mode
    display(hour, minute, alarmEnabled);
    break;

  case 1:
    // set hour mode
    alternatingDisplay(hour, -1, minute, minute, false, false);
    break;

  case 2:
    // set minute mode
    alternatingDisplay(hour, hour, minute, -1, false, false);
    break;
  case 3:
    // set alarm hour
    alternatingDisplay(alarmHour, -1, alarmMinute, alarmMinute, true, false);
    break;

  case 4:
    // set alarm minute
    alternatingDisplay(alarmHour, alarmHour, alarmMinute, -1, true, false);
    break;

  case 5:
    // set alarm enable
    alternatingDisplay(alarmHour, -1, alarmMinute, -1, alarmEnabled, alarmEnabled);
    break;

  default:
    mode = 0;
    break;
  }
}

char changeOnInput(char buttons, char value, char min, char max)
{
  if (buttons == 2)
  {
    value++;
    if (value > max)
    {
      value = min;
    }
  }
  else if (buttons == 1)
  {
    value--;
    if (value < min)
    {
      value = max;
    }
  }
  return value;
}

int readButtons()
{
  int result = 0;

  // check if button A is pressed, inverted since I use internal pullup
  if (!(bool(PINB & 1 << ABUTTON)))
  {
    result += 1;
  }

  // check if button B is pressed, inverted since I use internal pullup
  if (!(bool(PINB & 1 << BBUTTON)))
  {
    result += 2;
  }
  return result;
}

void makeSound(int lengthMS)
{
  PORTB |= 1 << PB5;
  _delay_ms(lengthMS);
  PORTB &= ~(1 << PB5);
}

void displayNumber(char digit, char number)
{
  PORTC = 0b1111;           // turn of display to be ready to show next digit without ghosting
  setDisplayNumber(number); // setup number to display
  PORTC = ~(1 << digit);    // enable the digit that is to be displayed
  _delay_ms(1);             // delay to make display brighter
}

void display(char hour, char minute, bool alarmOn)
{
  int numberDivisor = 10;

  if (hour > -1)
  {
    // display hour
    for (char i = 0; i < 2; i++)
    {
      int nrToDisplay = hour / numberDivisor; // picks out the single digit to display
      hour %= numberDivisor;                  // remove the digit that is being displayed
      numberDivisor /= 10;                    // setup to pick out the next digit on the next loop
      nrToDisplay += ((i == 1) ? 10 : 0);     // add 10 to digit to display decimal point
      displayNumber(i, nrToDisplay);
    }
  }

  numberDivisor = 10;

  if (minute > -1)
  {
    // display minutes
    for (char i = 2; i < 4; i++)
    {
      int nrToDisplay = minute / numberDivisor;      // picks out the single digit to display
      minute %= numberDivisor;                       // remove the digit that is being displayed
      numberDivisor /= 10;                           // setup to pick out the next digit on the next loop
      nrToDisplay += ((i == 3 && alarmOn) ? 10 : 0); // add 10 to second digit to display decimal point
      displayNumber(i, nrToDisplay);
    }
  }
  PORTC = 0b1111; // turn of display to be ready to show next digit without ghosting
}

void keepTime()
{
  if (millisecond > 59999)
  {
    incrementMinute();
    millisecond -= 60000;
  }
}

void incrementMinute()
{
  minute++;
  if (minute > 59)
  {
    incrementHour();
    minute = 0;
  }
}

void incrementHour()
{
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