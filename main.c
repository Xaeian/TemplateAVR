#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lib/sreg.h"

int main(void)
{
  // init();
  DDRD |= (1 << 3);
  DDRB |= (1 << 2);

  while (1)
  {
    // loop();
    _delay_ms(200);
    PORTD |= (1 << 3);
    PORTB &= ~(1 << 2);
    _delay_ms(200);
    PORTD &= ~(1 << 3);
    PORTB |= (1 << 2);
  }
}