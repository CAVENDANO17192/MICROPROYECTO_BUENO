/*
 * controlador.h
 *
 *  Created on: 11/03/2022
 *      Author: charlie tito
 */

#ifndef LIBRERIAS_CONTROLADOR_H_
#define LIBRERIAS_CONTROLADOR_H_


typedef struct PID_STRUCT
{
    float ek;
    float uk;
    float Ek;
    void (*control_pid)(struct PID_STRUCT *self, float imu, float ref, float Ek_1, float ek_1, float Kp, float Ki, float Kd, float dt);
}pid_T;


#endif /* LIBRERIAS_CONTROLADOR_H_ */
