#include <avr/io.h>
#include "seven.h"

int segmentPins[8];

// contains 0-19 as bits for a 7 segment display, segments assume A-G from top clocwise G in center, and last bit for decimal
unsigned char digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b00111110, 0b11100000, 0b11111110, 0b11110110, 0b11111101, 0b01100001, 0b11011011, 0b11110011, 0b01100111, 0b10110111, 0b00111111, 0b11100001, 0b11111111, 0b11110111};

void setSegmentPins(int *pins)
{
    for (int i = 0; i < 8; i++)
    {
        segmentPins[i] = pins[i];
    }
}

void setDisplayNumber(int nr)
{
    if (nr < 0 && nr > 19)
    {
        nr = 0;
    }
    
    for (int i = 0; i < 8; i++)
    {
        char pin = segmentPins[i];
        if (pin < 14 && pin > 7)
        {
            DDRB |= 1 << (pin - 8);
            PORTB ^= (digits[nr] >> (i - 1) & 1) << (pin - 8);
        } else {
            DDRD |= 1 << (pin);
            PORTD ^= (digits[nr] >> (i - 1) & 1) << (pin);
        }
    }
}