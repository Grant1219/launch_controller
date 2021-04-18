#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // set all ports to output mode
    DDRB = 0xff;
    // set output of all ports to zero
    PORTB = 0x00;

    while (1) {
        // turn pin 0 on
        //PORTB |= _BV(PB0);
        PORTB = 0xff;
        _delay_ms(500);
        // turn pin 0 off
        //PORTB &= ~(_BV(PB0));
        PORTB = 0x00;
        _delay_ms(500);
    }
}
