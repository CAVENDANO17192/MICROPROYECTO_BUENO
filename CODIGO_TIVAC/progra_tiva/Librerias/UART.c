/*
 * UART.c
 *
 *  Created on: 13/03/2022
 *      Author: Charlie tito
 */

#include<stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "Librerias/UART.h"
#include "Librerias/uartstdio.h"

void UART_init(void)
{
    //---------------------------------------INICIALIZACION DE PERIFERICOS---------------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //---------------------------------------SE INICIALIZAN UART0----------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );
    //IntEnable(INT_UART0);
    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTStdioConfig(0, 115200, SysCtlClockGet());

}

void send_uart (double medicion)
{
    double entero;
    double decimal;
    float envio_e;
    float envio_d;

    decimal = modf(medicion,&entero) * 10000;


    if(entero<0)
    {
        envio_e = -((float)entero);
        envio_d = -((float)decimal);
        UARTCharPut(UART0_BASE,'2');
    }
    else
    {
        envio_e = ((float)entero);
        envio_d = ((float)decimal);
        UARTCharPut(UART0_BASE,'3');
    }

    UARTprintf("%u\n",(uint32_t)envio_e);
    UARTprintf("%u\n",(uint32_t)envio_d);
}
void deco_mensaje(char data[9], float ref)
{
    char mensaje[8];
    mensaje[1]=data[1];
    mensaje[2]=data[2];
    mensaje[3]=data[3];
    mensaje[4]=data[4];
    mensaje[5]=data[5];
    mensaje[6]=data[6];
    mensaje[7]=data[7];
    float conv = atof(mensaje);
    if (mensaje[0] == '1')
    {
        ref = (90/255)*(-conv);
    }
    else
    {
        ref = (90/255)*conv;
    }
}
