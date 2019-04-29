#include "Moto.h"

Moto::Moto()
{
    this->IN3_1 = _3_1;
    this->IN3_2 = _3_2;
    this->IN4_1 = _4_1;
    this->IN4_2 = _4_2;
    this->coefficient = 0.6;
}

void Moto::initPin(int8_t IN3_1, int8_t IN3_2, int8_t IN4_1, int8_t IN4_2)
{
    this->IN3_1 = IN3_1;
    this->IN3_2 = IN3_2;
    this->IN4_1 = IN4_1;
    this->IN4_2 = IN4_2;
}

void Moto::initMoto()
{
    pinMode(IN3_1, OUTPUT);
    pinMode(IN3_2, OUTPUT);
    pinMode(IN4_1, OUTPUT);
    pinMode(IN4_2, OUTPUT);

    /* initPWM 3.9k */
    setPwmFrequency(3, 8);
    setPwmFrequency(9, 8);
    setPwmFrequency(10, 8);
    setPwmFrequency(11, 8);
}

void Moto::setMoto(int8_t side, int8_t PWM)
{
    int PWM_rate;
    if(PWM > 100 || PWM < -100)
    {
      return;
    }
    int real_speed = 1.8 * PWM;
    if(side == Left && PWM >= 0)
    {
        digitalWrite(IN3_2, LOW);
        analogWrite(IN3_1, real_speed); 
    }
    if(side == Left && PWM < 0)
    {
        digitalWrite(IN3_1, LOW);
        analogWrite(IN3_2, abs(real_speed)); 
    }
    if(side == Right && PWM >= 0)
    {
        digitalWrite(IN4_2, LOW);
        analogWrite(IN4_1, real_speed); 
    }
    if(side == Right && PWM < 0)
    {
        digitalWrite(IN4_1, LOW);
        analogWrite(IN4_2, abs(real_speed)); 
    }
}

void Moto::setCoefficient(float coefficient)
{
    this->coefficient = coefficient;
}

void Moto::RUN(int8_t ls, int8_t rs)
{
    setMoto(Left, ls * this->coefficient);
    setMoto(Right, -rs * this->coefficient);
}

void Moto::setPwmFrequency(int pin, int divisor) {
    byte mode;  
    if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
        switch(divisor) {
            case 1: mode = 0x01; break;
            case 8: mode = 0x02; break;
            case 64: mode = 0x03; break;
            case 256: mode = 0x04; break;
            case 1024: mode = 0x05; break;
            default: return;
        }
        if(pin == 5 || pin == 6) {
            TCCR0B = TCCR0B & 0b11111000 | mode;
        } else {
            TCCR1B = TCCR1B & 0b11111000 | mode;
        }
    } else if(pin == 3 || pin == 11) {
        switch(divisor) {
            case 1: mode = 0x01; break;
            case 8: mode = 0x02; break;
            case 32: mode = 0x03; break;
            case 64: mode = 0x04; break;
            case 128: mode = 0x05; break;
            case 256: mode = 0x06; break;
            case 1024: mode = 0x7; break;
        default: return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
}
