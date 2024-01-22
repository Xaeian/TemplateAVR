#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lib/sreg.h"
#include "lib/adc.h"

// PC2 (ADC2) ⟶ POT | JOY | MIC | FT | ...

int main(void)
{
  SREG_Init();
  ADC_Init();
  
  while(1) {
    uint16_t value = ADC_Run(2);
    SREG_Int(value);
  }
}