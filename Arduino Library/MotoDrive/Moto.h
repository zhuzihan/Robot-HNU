#ifndef Moto_h
#define Moto_h

#include "Arduino.h"

#define Left 0
#define Right 1

/* 
 * default Pin for Arduino UNO
 * IN3_1 ---> 3
 * IN3_2 ---> 11
 * IN4_1 ---> 9
 * IN4_2 ---> 10
 */

#define _3_1 3
#define _3_2 11
#define _4_1 9
#define _4_2 10

class Moto
{
    public:
        Moto();
        void initPin(int8_t IN3_1, int8_t IN3_2, int8_t IN4_1, int8_t IN4_2);
        void initMoto();
        void setMoto(int8_t side, int8_t PWM);
        void RUN(int8_t ls, int8_t rs);
        void setCoefficient(float coefficient);
        void setPwmFrequency(int pin, int divisor);
    
    private:
        int8_t IN3_1;
        int8_t IN3_2;
        int8_t IN4_1;
        int8_t IN4_2;
        float coefficient;
        int Hz;
};

#endif