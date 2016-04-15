#include "usart.h"

struct Usart usart;

void usart_Init(void){
	usart.debug.open("debug.out");
}

void usart_GPSSetBaudrate(uint32_t baud){

}

void usart_putcStdComm(uint8_t c){
	usart.debug << char(c);
}

void usart_putcGPS(uint8_t c){

}
