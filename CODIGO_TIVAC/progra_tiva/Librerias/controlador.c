/*
 * controlador.c
 *
 *  Created on: 11/03/2022
 *      Author: charlie tito
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Librerias/controlador.h"

void PID_control (struct PID_STRUCT *self, float imu, float ref, float Ek_1, float ek_1, float Kp, float Ki, float Kd,float dt)
{
    float ed;
    float ek;
    float Ek;

    ek  = ref - imu;
    ed = ek - ek_1;
    Ek  = Ek_1+ek;
    self-> uk = (Kp*ek) + (Ki*Ek)*dt + (Kd*ed)/dt;
    self-> ek = ek;
    self-> Ek = Ek;
    return self;
}


