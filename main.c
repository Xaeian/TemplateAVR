#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lib/sreg.h"

int main(void)
{
  SREG_Init();

  PORTY[0] = 1;
  PORTY[1] = 2;
  PORTY[2] = 3;
  PORTY[3] = 4;

  while(1);
}