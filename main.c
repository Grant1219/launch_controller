#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile uint8_t launch_armed = 0;
volatile uint8_t key_switch = 0;

ISR(PCINT0_vect) {
    // this interrupt fires whenever input is received on a pin
    key_switch = PINB & _BV(PB3);

    if (key_switch) {
        launch_armed = 1;
        // TODO enable "armed" LED
    }
}

int main(void) {
    // set all ports to output mode
    DDRB = 0xff;
    // set output of all ports to zero
    PORTB = 0x00;

    // configure pins 3 and 4 as input
    DDRB &= ~(_BV(PB3));
    DDRB &= ~(_BV(PB4));
    // activate pull-up resistors on pins 3 and 4
    PORTB |= _BV(PB3);
    PORTB |= _BV(PB4);

    while (1) {
        // disable interrupts
        cli();

        if (launch_armed) {
            key_switch = 0;
            uint8_t ignite_button = 0;

            // check for ignite button
            key_switch = PINB & _BV(PB3);
            ignite_button = PINB & _BV(PB4);

            if (ignite_button) {
                // delay for 1 second and check if all inputs are still set
                _delay_ms(1000);
                key_switch = PINB & _BV(PB3);
                ignite_button = PINB & _BV(PB4);

                if (key_switch && ignite_button) {
                    // launch! set pin 0 high for 300ms
                    PORTB |= _BV(PB0);
                    _delay_ms(300);
                    PORTB &= ~(_BV(PB0));
                    // stop the loop to prevent triggering again
                    break;
                }
                else {
                    launch_armed = 0;
                }
            }

            if (!key_switch) {
                launch_armed = 0;
            }
        }
        else {
            // sleep until inputs change
            sleep_enable();
            sleep_bod_disable();
            // enable interrupts
            sei();
            // actually sleep here
            sleep_cpu();
            sleep_disable();
        }
    }

    // TODO turn on reset LED
    // require a power cycle of the controller to allow launching again
    while (1) {}
}
