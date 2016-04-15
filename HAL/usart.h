#ifndef USART_H_
#define USART_H_

#include <iostream>
#include <fstream>

extern struct Usart {
	std::ofstream debug;
} usart;

void usart_Init(void);

void usart_GPSSetBaudrate(uint32_t baud);

void usart_putcStdComm(uint8_t c);

void usart_putcGPS(uint8_t c);
#endif
