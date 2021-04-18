#include <avr/io.h>
#include "seven.h"

int segmentPins[8];

unsigned char portBMask = 0;
unsigned char portDMask = 0;
// contains 0-19 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
unsigned char digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b00111110, 0b11100000, 0b11111110, 0b11110110};

void setSegmentPins(int *pins)
{
    for (int i = 0; i < 8; i++)
    {
        segmentPins[i] = pins[i];
    }

    portBMask = 0;
    portDMask = 0;
    for (int i = 0; i < 8; i++)
    {
        char pin = segmentPins[i];
        if (pin < 14 && pin > 7)
        {
            portBMask |= 1 << (pin - 8);
        }
        else
        {
            portDMask |= 1 << (pin);
        }
    }

    DDRB |= portBMask;
    DDRD |= portDMask;
}

void setDisplayNumber(int nr)
{
    unsigned char portB = 0;
    unsigned char portD = 0;

    for (int i = 0; i < 8; i++)
    {
        char pin = segmentPins[i];
        if (pin < 14 && pin > 7)
        {
            portB |= (((digits[nr % 10] + nr / 10) >> i) & 1) << (pin - 8);
        }
        else
        {
            portD |= (((digits[nr % 10] + nr / 10) >> i) & 1) << (pin);
        }
    }

    if (nr < 0 || nr > 19)
    {
        PORTB = (PORTB & ~portBMask);
        PORTD = (PORTD & ~portDMask);
    }
    else
    {
        PORTB = portB | (PORTB & ~portBMask);
        PORTD = portD | (PORTD & ~portDMask);
    }
}