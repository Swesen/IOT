#include <avr/io.h>
#include <util/delay.h>

unsigned char step = 0;
bool decending = false;

// since we don't know PWM yet this only turns the LEDs on or off
void knightRiderAnim()
{
    switch (step)
    {
    case 1:
        PORTB = 1 << PB1;
        break;
    case 2:
        PORTB = 1 << PB2;
        break;
    case 3:
        PORTB = 1 << PB3;
        PORTD = 0;
        break;
    case 4:
        PORTB = 0;
        PORTD = 1 << PD3;
        break;
    case 5:
        PORTD = 1 << PD5;
        break;
    case 6:
        PORTD = 1 << PD6;
        break;

    default:
        break;
    }

    if (step >= 6)
    {
        decending = true;
    }
    else if (step <= 1)
    {
        decending = false;
    }

    if (decending)
    {
        step--;
    }
    else
    {
        step++;
    }
}

int main()
{
    DDRB = 0B00001110; // enable output on pins 9,10,11
    DDRD = 0B01101000; // enable output on pins 3,5,6

    while (1)
    {
        knightRiderAnim();
        _delay_ms(50);
    }
    return 0;
}