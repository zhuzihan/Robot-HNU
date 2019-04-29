int E1 = 3, M1 = 48, E2 = 8, M2 = 49,  /**Moto IO**/
    F1 = 40, F2 = 39, F3 = 23, F4 = 24, F5 = 25, F6 = 26, F7 = 37, F8 = 38, /**Front gray level sensor IO**/
    G1 = 19, G2 = 22, G3 = 33, G4 = 34, G5 = 15, G6 = 16, G7 = 17, G8 = 18, /**Post gray level sensor IO**/
    FPin = A6, GPin = A7; /**Distance sensor IO**/
int flagFB = 0;/**0=forward 1=reverse**/


#define WHITE 1
#define GREEN 0

unsigned long starttime;
unsigned long stoptime;
unsigned long looptime;

//#include "U8glib.h"
//#include <MagneticSensorLsm303.h>
//#include <Wire.h>
//U8GLIB_SSD1306_128X64 u8g(52,51,53,50);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//MagneticSensorLsm303 compass;
//static unsigned char u8g_logo_bits[] U8G_PROGMEM= {
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0xFC,0xFF,0x1F,0x00,0x00,0x00,0x00,0xE0,0x07,0x00,0x7C,0x00,0x00,
//0x00,0x00,0x3F,0x00,0x00,0xE2,0x03,0x00,0x00,0xF0,0x07,0x00,0x00,0xC1,0x0E,0x00,
//0x00,0x5C,0x1C,0x00,0x80,0x80,0x19,0x00,0x00,0x26,0x30,0x00,0x80,0x00,0x31,0x00,
//0x00,0x33,0x20,0x00,0x40,0x00,0x63,0x00,0x80,0x11,0x60,0x00,0x60,0x00,0xC2,0x00,
//0x80,0x00,0x40,0x00,0x20,0x00,0x80,0x01,0xC0,0x00,0x00,0x00,0x20,0x00,0x00,0x01,
//0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
//0x20,0x00,0x00,0x00,0x80,0xFF,0x3F,0x02,0x20,0xC0,0xFF,0x01,0xE0,0x00,0xE0,0x07,
//0x30,0x7C,0x00,0x07,0x18,0x00,0x00,0x01,0x10,0x07,0x00,0x1C,0x3C,0x00,0x00,0x0C,
//0xD0,0x00,0x00,0x10,0x7E,0x00,0x00,0x18,0x30,0x00,0x00,0x20,0xFF,0x00,0x00,0x30,
//0xFC,0x00,0x00,0x20,0xFF,0x01,0x00,0x20,0xFE,0x01,0x00,0x20,0xFF,0xC1,0x01,0x40,
//0xFF,0x01,0x00,0xB0,0xFF,0x71,0x1F,0x40,0xFF,0xF8,0xFF,0x9C,0xFF,0x1D,0x60,0x60,
//0xFF,0x0F,0x80,0x17,0xFF,0x07,0xC0,0x39,0xFE,0x01,0x00,0x00,0xFE,0x00,0x00,0x0F,
//0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
//0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
//0x10,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x08,0x00,0x00,0x00,0x00,0x08,0x08,
//0x10,0x08,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x08,0x00,0x00,0x00,0x00,0x08,0x08,
//0x10,0x08,0x00,0x00,0x00,0x00,0x0C,0x08,0x10,0x18,0x00,0x00,0x00,0x00,0x04,0x08,
//0x30,0x10,0x00,0x00,0x00,0x00,0x04,0x08,0x20,0x30,0x00,0x00,0x00,0x00,0x04,0x0C,
//0x20,0x20,0x00,0x00,0x00,0x00,0x02,0x04,0x20,0x20,0x00,0x00,0x00,0x00,0x03,0x04,
//0x20,0x40,0x00,0x00,0x00,0x00,0x01,0x04,0x60,0xC0,0x00,0x00,0x00,0x80,0x00,0x06,
//0x40,0x80,0x00,0x00,0x00,0xC0,0x00,0x02,0x40,0x80,0x01,0x00,0x00,0x60,0x00,0x02,
//0x40,0x00,0x03,0x00,0x00,0x20,0x00,0x03,0x80,0x00,0x06,0x00,0x00,0x30,0x00,0x01,
//0x80,0x00,0x0C,0x00,0x00,0x18,0x00,0x01,0x80,0x01,0x30,0x00,0x00,0x0C,0x80,0x00,
//0x00,0x01,0xE0,0x00,0x00,0x06,0xC0,0x00,0x00,0x02,0x80,0x03,0x80,0x03,0x40,0x00,
//0x00,0x06,0x00,0x0E,0xF8,0x00,0x60,0x00,0x00,0x0C,0x00,0xF0,0x0F,0x00,0x30,0x00,
//0x00,0x08,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x0C,0x00,
//0x00,0x60,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0xC0,0x01,0x00,0x00,0x00,0x03,0x00,
//0x00,0x00,0x03,0x00,0x00,0xE0,0x01,0x00,0x00,0x00,0x1E,0x00,0x00,0x3C,0x00,0x00,
//0x00,0x00,0xF0,0x00,0x80,0x07,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//};
//void draw(void) {
//  u8g.drawXBMP( 0, 0, 64, 64, u8g_logo_bits);
//}

void wait(double sec)
{
  delay(sec);
}

void setMoto(int side, int v) //0=L 1=R
{
     if(side && v>=0){     //right side
          digitalWrite(M2, HIGH);
          analogWrite(E2, v); //PWM control velocity
     }
     if(side && v<0){
          digitalWrite(M2, LOW);
          analogWrite(E2, -v); //PWM control velocity
     }
     if(side==0 && v>=0){ //left side
          digitalWrite(M1, LOW);
          analogWrite(E1, v);
     }
     if(side==0 && v<0){
          digitalWrite(M1, HIGH);
          analogWrite(E1, -v);
     }
}
void RUN( int LeftV, int RightV ) //v= 0~255
{
     setMoto(0, LeftV);
     setMoto(1, RightV);
}//V 0~255
void RUNB( int LeftV, int RightV ) //v= 0~255
{
     setMoto(0, -LeftV);
     setMoto(1, -RightV);
}//V 0~255
void Stop()
{
    while(1)
    {
      RUN(0,0);
    }
}
int colorReco (int sensor)/**color recognition（white 1 green 0）**/
{
    return digitalRead(sensor);  
}
//return distance (cm)
double frontDis () /*front distance detection*/
{
//    value = analogRead(FPin);
//    if (value < 100)  value = 100;
//    return 2076.0 / (value - 11.0);
    return analogRead(FPin)<100? 100:2076.0/(analogRead(FPin)-11.0);
}
double postDis ()
{
    return analogRead(GPin)<100? 100:2076.0/(analogRead(GPin)-11.0);
}
void directionDet() /**directionDetection**/
{
  if(frontDis() < 30) flagFB = 1; //change direction to backward
  else if(postDis()<30) flagFB = 0; //change direction to forward
}
int compassRead()
{
  //return(atan2(compass.m.y,compass.m.x)+PI)*57.2956;
  //return(atan(compass.m.y/compass.m.x)+PI/2)*57.2956;
  //return atan2((double)compass.m.y,(double)compass.m.x) * (180 / 3.14159265)+180;
}
//void lineWalking()/**Forward line-tracking **/
//{
//    flagFB = 0;
//    if (colorReco(F4) == WHITE&&colorReco(F5) == WHITE) RUN(250, 250);
//    else if (colorReco(F4) == WHITE) RUN(200, 250);
//    else if (colorReco(F5) == WHITE) RUN(250, 200);
//    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) RUN(-50, 250);
//    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) RUN(250, -50);
//    else if (colorReco(F3) == WHITE) RUN(-100, 250);
//    else if (colorReco(F6) == WHITE) RUN(250, -100);
//    else if (colorReco(F2) == WHITE&&colorReco(F3) == WHITE) RUN(-50, 250);
//    else if (colorReco(F6) == WHITE&&colorReco(F7) == WHITE) RUN(250, -50);
//    else if (colorReco(F2) == WHITE) { RUN(-250, 250); wait(50); }
//    else if (colorReco(F7) == WHITE) { RUN(250, -250); wait(50); }
//    else if (colorReco(F1) == WHITE) { RUN(-250, 250); wait(50); }
//    else if (colorReco(F8) == WHITE) { RUN(250, -250); wait(50); }
//    else RUN(250,250);
//}
//void lineWalkingRev()/**reverse line-tracking**/
//{
//    flagFB = 1;
//    if (colorReco(F4) == WHITE&&colorReco(F5) == WHITE) RUNB(250, 250);
//    else if (colorReco(F4) == WHITE) RUNB(200, 250);
//    else if (colorReco(F5) == WHITE) RUNB(250, 200);
//    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) RUNB(-50, 250);
//    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) RUNB(250, -50);
//    else if (colorReco(F3) == WHITE) RUNB(-100, 250);
//    else if (colorReco(F6) == WHITE) RUNB(250, -100);
//    else if (colorReco(F2) == WHITE&&colorReco(F3) == WHITE) RUNB(-50, 250);
//    else if (colorReco(F6) == WHITE&&colorReco(F7) == WHITE) RUNB(250, -50);
//    else if (colorReco(F2) == WHITE) { RUNB(-250, 250); wait(50); }
//    else if (colorReco(F7) == WHITE) { RUNB(250, -250); wait(50); }
//    else if (colorReco(F1) == WHITE) { RUNB(-250, 250); wait(50); }
//    else if (colorReco(F8) == WHITE) { RUNB(250, -250); wait(50); }
//    else RUNB(250,250);
//}
void lineWalking_high()/**Forward line-tracking **/
{
    flagFB = 0;
    if (colorReco(F4) == WHITE&&colorReco(F5) == WHITE) RUN(150, 150);
    else if (colorReco(F4) == WHITE) RUN(100, 150);
    else if (colorReco(F5) == WHITE) RUN(150, 100);
    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) RUN(-50, 150);
    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) RUN(150, -50);
    else if (colorReco(F3) == WHITE) RUN(-100, 150);
    else if (colorReco(F6) == WHITE) RUN(150, -100);
    else if (colorReco(F2) == WHITE&&colorReco(F3) == WHITE) RUN(-50, 150);
    else if (colorReco(F6) == WHITE&&colorReco(F7) == WHITE) RUN(150, -50);
    else if (colorReco(F2) == WHITE) { RUN(-150, 150); wait(50); }
    else if (colorReco(F7) == WHITE) { RUN(150, -150); wait(50); }
    else if (colorReco(F1) == WHITE) { RUN(-150, 150); wait(50); }
    else if (colorReco(F8) == WHITE) { RUN(150, -150); wait(50); }
    else RUN(150,150);
}
void lineWalking()/**Forward line-tracking **/
{
    flagFB = 0;
    if (colorReco(F4) == WHITE||colorReco(F5) == WHITE) RUN(60, 60);
    else if (colorReco(F3) == WHITE) RUN(-140, 200);
    else if (colorReco(F6) == WHITE) RUN(200, -140);
    else if (colorReco(F2) == WHITE) { RUN(-160, 160); wait(50); }
    else if (colorReco(F7) == WHITE) { RUN(160, -160); wait(50); }
    else if (colorReco(F1) == WHITE) { RUN(-170, 170); wait(50); }
    else if (colorReco(F8) == WHITE) { RUN(170, -170); wait(50); }
    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) RUN(-80, 250);
    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) RUN(250, -80);
    else if (colorReco(F2) == WHITE&&colorReco(F3) == WHITE) RUN(-50, 130);
    else if (colorReco(F6) == WHITE&&colorReco(F7) == WHITE) RUN(130, -50);
    else RUN(60,60);
}
void lineWalkingRev()/**reverse line-tracking**/
{
    flagFB = 1;
    if (colorReco(F4) == WHITE&&colorReco(F5) == WHITE) RUNB(140, 140);
    else if (colorReco(F4) == WHITE) RUNB(100, 140);
    else if (colorReco(F5) == WHITE) RUNB(140, 100);
    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) RUNB(-40, 140);
    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) RUNB(140, -40);
    else if (colorReco(F3) == WHITE) RUNB(-30, 80);
    else if (colorReco(F6) == WHITE) RUNB(80, -30);
    else if (colorReco(F2) == WHITE&&colorReco(F3) == WHITE) RUNB(-40, 80);
    else if (colorReco(F6) == WHITE&&colorReco(F7) == WHITE) RUNB(80, -40);
    else if (colorReco(F2) == WHITE) { RUNB(-80, 80); wait(50); }
    else if (colorReco(F7) == WHITE) { RUNB(80, -80); wait(50); }
    else if (colorReco(F1) == WHITE) { RUNB(-80, 80); wait(50); }
    else if (colorReco(F8) == WHITE) { RUNB(80, -80); wait(50); }
    else RUNB(80,80);
}
void lineToLeft()/**line-tracking stop at the left corner**/
{
    while(1)
    {
        directionDet();
        if(flagFB == 0)
        {
            if(colorReco(F6)==WHITE && colorReco(F7)==WHITE)
            {
                RUN(-100,-100);
                wait(20);
                break;
            }
        }
        else
        {
            if(colorReco(G6)==WHITE && colorReco(G7)==WHITE)
            {
                RUN(-100,-100);
                wait(20);
                break;
            }
        }
        lineWalking();
    }
}
void lineToRight()/**line-tracking stop at the right corner**/
{
    while(1)
    {
        directionDet();
        if(flagFB == 0)
        {
            if(colorReco(F2)==WHITE && colorReco(F3)==WHITE)
            {
                RUN(-100,-100);
                wait(20);
                break;
            }
        }
        else
        {
            if(colorReco(G2)==WHITE && colorReco(G3)==WHITE)
            {
                RUN(-100,-100);
                wait(20);
                break;
            }
        }
        lineWalking();
    }
}
void Turn_R()
{
    RUN(140,140);
    wait(160);
    RUN(-160,160);
    wait(700);
}
void Turn_L()
{
    RUN(140,140);
    wait(160);
    RUN(-140,140);
    wait(700);
}
void Turn_B()
{
    //RUN(120,120);
    //wait(160);
    RUN(-240,240);
    wait(200);
    while(1){
      RUN(-240,240);
      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
    }
}
void Turn_RB()
{
    RUNB(140,140);
    wait(160);
    RUNB(-140,140);
    wait(700);
}
void Turn_LB()
{
    RUNB(140,140);
    wait(160);
    RUNB(-140,140);
    wait(700);
}
void setup() 
{
    Serial.begin(9600); 
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    pinMode(FPin,INPUT);
    pinMode(GPin,INPUT);
    //Wire.begin();
    //compass.enableDefault();
    //compass.init();
    //compass.enable();
} 
void loop() 
{
  /**
 * rpm = 600 ;
 * normal speed >= 60
 * upgrade speed >= 100
 */

 lineWalking_high() nht5;
//  u8g.firstPage(); 
//  do {
//    draw();
//  } while( u8g.nextPage() );

//  RUN(100,100);

//    Line_walking();
//    Line_To_Right();
//    Turn_R();
//      RUN(200,200);
//    //wait(1000);
//    while(colorReco(F1)==GREEN &&colorReco(F8)==GREEN){
//      Line_walking();
//    }

/**无平台路线**/

//starttime = millis();
//while (millis() - starttime < 3000)
//{
////   // lineWalking();
//}

//    while(1)
//    {
//      Serial.println(frontDis());
//      wait(300);
//    }

//RUN(200,200);

//Stop();

//while(frontDis()<10){}
//while(colorReco(F1)==WHITE ||colorReco(F2)==WHITE ||colorReco(F3)==WHITE ||colorReco(F4)==WHITE ||colorReco(F5)==WHITE ||colorReco(F6)==WHITE ||colorReco(F7)==WHITE ||colorReco(F8)==WHITE)
//{
//    lineWalking();  
//}
//    RUN(-160,-160);
//    wait(50);
//    Turn_B();
//    Stop();

// while(1)
// {
//    lineWalking();  
//    if(frontDis()<30)
//    {
//      break;
//    }
// }
// while(1)
// {
//    RUN(150,150);
//    if(frontDis()<30)
//    {
//      break;
//    }
// }

//      while(1){
//        lineWalking_600();
//        if(frontDis()<15)
//        {
//          RUN(-160,-160);
//          wait(50);
//          break;
//        }
//      }
//      Stop();

//    RUN(180,180);
//    wait(2000);
//    RUN(-300,-300);
//    wait(50);
//    Stop();

}
