#include <Moto.h>


Moto moto;

void setup() {
  // put your setup code here, to run once:
  moto.initMoto();
}

void loop() {
  // put your main code here, to run repeatedly:
  moto.RUN(30,30);
}
