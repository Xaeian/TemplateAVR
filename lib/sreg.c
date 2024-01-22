#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "sreg.h"

volatile uint8_t PORTX, PINX, PORTY[4];

//-----------------------------------------------------------------------------

void SREG_Init()
{
  PORTX = 0; PORTY[0] = 0; PORTY[1] = 0; PORTY[2] = 0; PORTY[3] = 0;
  
  PORTB |= (1 << SPI_CS);
  DDRB |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_CS);
  DDRB &= ~(1 << SPI_MISO);
  SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
  // Prescaler: 128
  
  PORTB &= ~(1 << SPI_CS);
  SPDR = PORTX;
  sei();
}

__inline void SREG_Stop() { SPCR &= ~(1<<SPIE); }
__inline void SREG_Start() { SPCR |= (1<<SPIE); }
  
uint8_t portx_state;

ISR(SPI_STC_vect)
{
  switch(portx_state)
  {
    case 0: PINX = SPDR;
    case 1: case 2: case 3: SPDR = PORTY[portx_state]; break;
    case 4: PORTB |= (1 << SPI_CS); _delay_us(1); PORTB &= ~(1 << SPI_CS); SPDR = PORTX;
  }
  portx_state++;
  if(portx_state > 4) { portx_state = 0; }
}


//-----------------------------------------------------------------------------

void SREG_Sign(uint8_t position, uint8_t sign, bool dot)
{
  // if(sign == 1) {
  //   PORTY[position] = 0x06;
  // }

  switch(sign)
  {
    case 1: case '1': PORTY[position] = 0x06; break;
    case 2: PORTY[position] = 0x2F; break;
    case 4: PORTY[position] = 0x37; break;
          // statements executed if expression does not equal
          // any case constant_expression
  }


  // TODO:
  // Napisz funkcję, która na konkretnym wyświetlaczu o pozycji `position`
  // wyświetla znak `sign`. Znak może być podany jako liczba lub w kodzie ASCII.
  // Ustawienie flagi dot powinno spowodować wyświetlanie kropki
  // na wskazanym wyświetlaczu.
}

//-----------------------------------------------------------------------------

void SREG_Error(void)
{
  // TODO
}

void SREG_Int(int16_t value)
{
  // TODO
}

//-----------------------------------------------------------------------------
