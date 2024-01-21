#ifndef SREG_H_
#define SREG_H_

#include <stdbool.h>
#include <stdint.h>

#define	SPI_CS 2
#define SPI_MOSI 3
#define SPI_MISO 4
#define	SPI_SCK 5

void SREG_Init();
void SREG_Stop();
void SREG_Start();
void SREG_Sign(uint8_t position, uint8_t sign, bool dot);
void SREG_Int(int16_t value);

extern volatile uint8_t PORTX, PINX, PORTY[4];

#endif