/*
 * UART.h
 *
 *  Created on: 13/03/2022
 *      Author: Charlie tito
 */

#ifndef UART_H_
#define UART_H_


extern void UART_init(void);
void send_uart (double medicion);
void deco_mensaje(char data[9], float ref);

#endif /* LIBRERIAS_UART_H_ */
