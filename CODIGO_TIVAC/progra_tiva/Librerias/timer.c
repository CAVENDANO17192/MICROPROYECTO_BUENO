/*
 * timer.c
 *
 *  Created on: 14/03/2022
 *      Author: charlie tito
 */
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "Librerias/timer.h"

void timer_init(void)
{
    //---------------------------------------SE ACTIVA EL PERIFERICO DE TIMER0---------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }
    //----------------------------------------INICIALIZACION DEL TIMER 0------------------------------------------
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    //----------------------------------------CONFIGURACION DEL TIEMPO DEL TIMER 0-------------------------------
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet()/1000) -1);
    //-----------------------------------------SE ACTIVAN LAS INTERRUPCIONES DEL TIMER0------------------------------------
    IntEnable(INT_TIMER0A);
    //----------------------------------------SE ACTIVA LAS INTERRUPCIONES DEL TIMER-----------------------------
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);

    //----------------------------------------ACTIVAMOS LAS INTERRUPCIONES--------------------------------------
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}
