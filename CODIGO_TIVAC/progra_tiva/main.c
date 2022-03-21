
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "sensorlib/i2cm_drv.h"
#include "sensorlib/hw_mpu6050.h"
#include "sensorlib/mpu6050.h"

#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"

#include "driverlib/uart.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"



#include "Librerias/MPU6050.h"
#include "Librerias/uartstdio.h"
#include "Librerias/timer.h"
#include "Librerias/controlador.h"
#include "Librerias/UART.h"
#include <math.h>



#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/ssi.h"




#define NUM_SPI_DATA    1                 // Número de palabras que se envían cada vez
#define SPI_FREC  4000000                 // Frecuencia para el reloj del SPI
#define SPI_ANCHO      16                 // Número de bits que se envían cada vez, entre 4 y 16

uint16_t dato = 0b0111000000000000;       // Para lo que se envía por SPI.
uint16_t contador = 2048; // Para lo que se envía por SPI.


/**
 * main.c
 */
double alpha= 0.8;
double dt =0.001;
int update;
int serial;
float kI=0;
float kD=0;
float Kp=0.1;
double ParteDecimal;
double ParteEntera;
float Ek_1=0;
float ek_1=0;

float ed=0;
float ek=0;
float Ek=0;
uint16_t ukf=0;
float uk=0;

int update;
char  n;
int serial;
double mensaje;
char prueba [9];
float ref;
float conv;

void Timer0Handler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    uint32_t pui32DataTx[NUM_SPI_DATA]; // la función put pide tipo uint32_t
    uint8_t ui32Index;
    update = 1;
    pui32DataTx[0] = (uint32_t)(dato);
    for(ui32Index = 0; ui32Index < NUM_SPI_DATA ; ui32Index++)
    {
        SSIDataPut(SSI0_BASE, pui32DataTx[ui32Index]);
    }
    while(SSIBusy(SSI0_BASE))
    {
    }

}



struct DATOS_STRUCT imu;
struct PID_STRUCT pid;


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
    InitI2C0();
    MPU6050INIT();
    timer_init();
    UART_init();
    uint32_t pui32residual[NUM_SPI_DATA];
// Configuración de SPI
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SPI_FREC, SPI_ANCHO);
    SSIEnable(SSI0_BASE);
    UARTCharPutNonBlocking(UART0_BASE, '6');
    while(1)
    {
        n = UARTCharGetNonBlocking(UART0_BASE);
        if(update==1)
        {


            imu.acelerometro = MPU6050READDATA(&imu);
            imu.filtro_posicion = filtro_complementario_posicion(&imu, imu.Filtrox, imu.gx, imu.Accx, imu.Filtroy, imu.gy, imu.Accy, dt, alpha);
            imu.filtro_velocidad = filtro_complementario_velocidad(&imu, imu.Velocidadx, imu.gx, imu.Velocidady, imu.gy, imu.Velocidadz, imu.gz, alpha);

            ParteDecimal = modf(imu.Filtrox, &ParteEntera);
            //posicion.control_pid = PID_control(&posicion, ParteEntera,4, posicion.Ek, posicion.ek,  Kp, kI, kD, dt);

            ek  = ref - ParteEntera;
                ed = ek - ek_1;
                Ek  = Ek_1+ek;
                uk = (Kp*ek) + (kI*Ek)*((float)dt) + (kD*ed)/((float)dt);
                ek_1 = ek;
                Ek_1 = Ek;

                if(uk<-10)
                {
                    uk =-10;
                }
                if(uk>10)
                {
                    uk = 10;
                }
                uk = (uk+10)*(4095/20);

                ukf=(int)uk;
            update=0;
            dato = 0b0111000000000000;
            dato = dato + ukf;

        }
        if(n == '1')
        {
            UARTCharPut(UART0_BASE,'1');
            send_uart (imu.Filtrox);
            send_uart (imu.gx);
            send_uart (imu.gy);
            send_uart (imu.gz);
            send_uart (imu.ax);
            send_uart (imu.ay);
            send_uart (imu.az);
            n=0;
        }
        if(n == '2')
        {
            prueba[0]=UARTCharGet(UART0_BASE);
            prueba[1]=UARTCharGet(UART0_BASE);
            prueba[2]=UARTCharGet(UART0_BASE);
            prueba[3]=UARTCharGet(UART0_BASE);
            prueba[4]=UARTCharGet(UART0_BASE);
            prueba[5]=UARTCharGet(UART0_BASE);
            prueba[6]=UARTCharGet(UART0_BASE);
            prueba[7]=UARTCharGet(UART0_BASE);
            prueba[8]=UARTCharGet(UART0_BASE);
            n=0;
        }
        char deco[7];
        deco[0]=prueba[1];
        deco[1]=prueba[2];
        deco[2]=prueba[3];
        deco[3]=prueba[4];
        deco[4]=prueba[5];
        deco[5]=prueba[6];
        deco[6]=prueba[7];
        conv = atof(deco);
        if (prueba[0] == '1')
        {
            ref = -conv;
        }
        else
        {
            ref = conv;
        }
        ref = ref *0.3525411765;

    }
    //return 0;
}
