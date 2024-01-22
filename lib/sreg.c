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
  switch(sign) {
    case 0: case '0': PORTY[position] = 0x3F; break;
    case 1: case '1': PORTY[position] = 0x06; break;
    case 2: case '2': PORTY[position] = 0x5B; break;
    case 3: case '3': PORTY[position] = 0x4F; break;
    case 4: case '4': PORTY[position] = 0x66; break;
    case 5: case '5': PORTY[position] = 0x6D; break;
    case 6: case '6': PORTY[position] = 0x7D; break;
    case 7: case '7': PORTY[position] = 0x07; break;
    case 8: case '8': PORTY[position] = 0x7F; break;
    case 9: case '9': PORTY[position] = 0x6F; break;
    case 10: case 'A': case 'a': PORTY[position] = 0x77; break;
    case 11: case 'B': case 'b': PORTY[position] = 0x7C; break;
    case 12: case 'C': case 'c': PORTY[position] = 0x39; break;
    case 13: case 'D': case 'd': PORTY[position] = 0x5E; break;
    case 14: case 'E': case 'e': PORTY[position] = 0x79; break;
    case 15: case 'F': case 'f':  PORTY[position] = 0x71; break;
    case '-': PORTY[position] = 0x40; break;
    case '_': PORTY[position] = 0x08; break;
    case 'R': case 'r': PORTY[position] = 0x50; break;
    case 'O': case 'o': PORTY[position] = 0x5C; break;
    default: PORTY[position] = 0x00; break;
  }
  if(dot) PORTY[position] |= (1 << 7);
}

//-----------------------------------------------------------------------------

void SREG_Error(void)
{
  SREG_Sign(0, 'E', false);
  SREG_Sign(1, 'r', false);
  SREG_Sign(2, 'r', false);
  SREG_Sign(3, 'o', false);
}

const uint16_t SREG_DIV[] = {1000, 100, 10, 1};

void SREG_Int(int16_t value)
{
  if(value < -999 || value > 9999) {
    SREG_Error();
    return;
  }

  uint8_t start = 0;
  if(value < 0)
  {
    value = -value;
    start = 1;
    SREG_Sign(0, '-', false);
  }
  
  char space = ' ';
  uint8_t sign, i;

  for(i = start; i < 4; i++)
  {
    sign = value / SREG_DIV[i];
    if(sign || i == 3) space = 0;
    else sign = space;
    SREG_Sign(i, sign, false);
    value %= SREG_DIV[i];
  }
}

//-----------------------------------------------------------------------------
