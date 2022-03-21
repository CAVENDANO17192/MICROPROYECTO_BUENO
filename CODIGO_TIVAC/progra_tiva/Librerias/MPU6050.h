/*
 * MPU6050.h
 *
 *  Created on: 10/03/2022
 *      Author: charlie tito
 */

#ifndef LIBRERIAS_MPU6050_H_
#define LIBRERIAS_MPU6050_H_


//
// A boolean that is set when a MPU6050 command has completed.
//
volatile bool g_bMPU6050Done;

//
// I2C master instance
//
tI2CMInstance g_sI2CMSimpleInst;

//
//Device frequency
//
float fAccel[3];
float fGyro[3];
tMPU6050 sMPU6050;


void InitI2C0(void);
void MPU6050Callback(void *pvCallbackData, uint_fast8_t ui8Status);
void I2CMSimpleIntHandler(void);
void MPU6050INIT(void);


struct DATOS_STRUCT
{
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
    double Accx;
    double Accy;
    double Filtrox;
    double Filtroy;
    double Velocidadx;
    double Velocidady;
    double Velocidadz;
    void (*acelerometro)(struct DATOS_STRUCT *self);
    void (*filtro_posicion)(struct DATOS_STRUCT *self, double Filtrox, double gyrox, double Accx, double Filtroy, double gyroy, double Accy, double dt, double alpha);
    void (*filtro_velocidad)(struct DATOS_STRUCT *self, double Velocidadx, double gyrox, double Velocidady, double gyroy, double Velocidadz, double gyroz, double alpha);
};

#endif /* LIBRERIAS_MPU6050_H_ */
