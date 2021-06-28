#include <avr/io.h>
#include "seven.h"

int segmentPins[8];

unsigned char portBMask = 0;
unsigned char portDMask = 0;
// contains 0-19 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
unsigned char digits[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01101111};

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

    if (nr < 0 || nr > 19)
    {
        PORTB = (PORTB & ~portBMask);
        PORTD = (PORTD & ~portDMask);
    }
    else
    {
        unsigned char digit = digits[nr % 10] + ((nr > 9) ? 0b10000000 : 0);
        for (int i = 0; i < 8; i++)
        {
            char pin = segmentPins[i];
            if (pin < 14 && pin > 7)
            {
                portB |= (digit & 1) << (pin - 8);
            }
            else
            {
                portD |= (digit & 1) << (pin);
            }
            digit >>= 1;
        }
        PORTB = portB | (PORTB & ~portBMask);
        PORTD = portD | (PORTD & ~portDMask);
    }
}