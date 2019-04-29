#include "Moto.h"
#include <Wire.h>
#include <JY901.h>
#define Led 13
#define WHITE 1
#define GREEN 0
#define GRAY  0
#define FPin A0
#define FLPin A0/**车底左红外**/
#define FRPin A1/**车底右红外**/
#define FPin A3/**车前红外**/
#define flagFB 0/**0=forward 1=reverse**/
#define flagFix 0

Moto moto;
int gray = 0x00;
int mGround = 0x00;
float initAngle, newAngle;
unsigned long starttime;
int F1 = 12, F2 =8, F3 =7, F4 = 6, F5 = 5, F6 = 4, F7 = 2, F8 = A2; /**Front gray level sensor IO**/

void wait(int sec)
{
  delay(sec);
}
void RUN( int LeftV, int RightV )//v= 0~255
{ 
     moto.RUN(LeftV,RightV);
}//V 0~255
void RUNB( int LeftV, int RightV )//v= 0~255
{ 
     moto.RUN(-RightV,-LeftV);
}//V 0~255
void Stop()
{
    while(1)
    {
      moto.RUN(0,0);
    }
}
int colorReco(int sensor)/**color recognition（white 1 green 0）**/
{
    return digitalRead(sensor);  
}
int grayValue()/**地面灰度检测**/
{
    gray = (colorReco(F8) << 7)+ (colorReco(F7) << 6) + (colorReco(F6) << 5) + (colorReco(F5) << 4) + (colorReco(F4) << 3) + (colorReco(F3) << 2) + (colorReco(F2) << 1) + (colorReco(F1));
    return gray;  
}
void checkSensor()/**传感器自检**/
{
    while(1)
    {
        mGround = grayValue();
        /**测试灰度传感器**/
        Serial.print("F1-F8:");
        Serial.print(colorReco(F8));
        Serial.print(colorReco(F7));
        Serial.print(colorReco(F6));
        Serial.print(colorReco(F5));
        Serial.print("  ");
        Serial.print(colorReco(F4));
        Serial.print(colorReco(F3));
        Serial.print(colorReco(F2));
        Serial.print(colorReco(F1));
        Serial.print("  ");
        Serial.println(mGround); 
        
        /**测试红外传感器**/
        Serial.print("FDIS:");
        Serial.print(frontDis());
        Serial.print("  FLDIS:");
        Serial.print(FLDis());
        Serial.print("  FRDIS:");
        Serial.println(FRDis());    
        wait(500);
    }
}
//return distance (cm)
float frontDis () /**车前红外**/
{
    float dis = analogRead(FPin) < 60 ? 100 : 2076.0 / (analogRead(FPin) - 11.0);
    return dis < 0 ? 100 : dis;
}
double FLDis ()/**车底左红外**/
{
    return analogRead(FLPin);
}
double FRDis ()/**车底右红外**/
{
    return analogRead(FRPin);
}
void LED(int count)/**闪2次黄灯**/
{
    while(count--){
        digitalWrite(Led,1);
        delay(500);
        digitalWrite(Led,0);
        delay(500);
    }
}
void bridgeWalking()/**过桥函数**/
{
    if (FLDis() < 400 && FRDis() > 400) {RUN(50, 35);wait(2);}
    else if (FLDis() > 400 && FRDis() < 400) {RUN(35, 50);wait(2);}
    else RUN(35,35);
}
void lineWalking()/**正向寻线函数**/
{ 
    mGround = grayValue();
    if ( mGround == 0x18 ) RUN(60, 60);                     /**0001 1000**/
    else if ( mGround == 0x08 ) RUN(65, 60);                /**0000 1000**/
    else if ( mGround == 0x10 ) RUN(60, 65);                /**0001 0000**/
    else if ( mGround == 0x0C ) RUN(70, 60);                /**0000 1100**/
    else if ( mGround == 0x30 ) RUN(60, 70);                /**0011 0000**/
    else if ( mGround == 0x04 ) RUN(80, 50);                /**0000 0100**/
    else if ( mGround == 0x20 ) RUN(50, 80);                /**0010 0000**/
    else if ( mGround == 0x06 ) { RUN(60, -40); wait(20); } /**0000 0110**/
    else if ( mGround == 0x60 ) { RUN(-40, 60); wait(20); } /**0110 0000**/
    else if ( mGround == 0x02 ) { RUN(80, -60); wait(20); } /**0000 0010**/
    else if ( mGround == 0x40 ) { RUN(-60, 80); wait(20); } /**0100 0000**/
    else if ( mGround == 0x01 ) { RUN(90, -90); wait(20); } /**0000 0001**/
    else if ( mGround == 0x80 ) { RUN(-90, 90); wait(20); } /**1000 0000**/
    else RUN(60,60);
}
void lineWalkingLow()/**正向寻线函数**/
{ 
    mGround = grayValue();
    if ( mGround == 0x0018 ) RUN(40, 40);                   /**0001 1000**/
    else if ( mGround == 0x08 ) RUN(45, 40);                /**0000 1000**/
    else if ( mGround == 0x10 ) RUN(40, 45);                /**0001 0000**/
    else if ( mGround == 0x0C ) RUN(50, 40);                /**0000 1100**/
    else if ( mGround == 0x30 ) RUN(40, 50);                /**0011 0000**/
    else if ( mGround == 0x04 ) RUN(55, 30);                /**0000 0100**/
    else if ( mGround == 0x20 ) RUN(30, 55);                /**0010 0000**/
    else if ( mGround == 0x06 ) { RUN(40, -40); wait(20); } /**0000 0110**/
    else if ( mGround == 0x60 ) { RUN(-40, 40); wait(20); } /**0110 0000**/
    else if ( mGround == 0x02 ) { RUN(60, -60); wait(20); } /**0000 0010**/
    else if ( mGround == 0x40 ) { RUN(-60, 60); wait(20); } /**0100 0000**/
    else if ( mGround == 0x01 ) { RUN(70, -70); wait(20); } /**0000 0001**/
    else if ( mGround == 0x80 ) { RUN(-70, 70); wait(20); } /**1000 0000**/
    else RUN(40,40);
}
float getAngle()
{
    JY901.GetAngle();
    return (float)JY901.stcAngle.Angle[2]/32768*180 +180;
}
void turnBack()
{
    initAngle = getAngle();
//    Serial.println(initAngle);
    newAngle = 0;
    if(initAngle < 180){
//      Serial.println("<180");
        while(1)
        {
//            Serial.print(newAngle);
//            Serial.print("  ");
//            Serial.println(newAngle - initAngle);
            if(abs(newAngle - initAngle) > 180)
            {
              RUN(0,0);
              wait(50);
              break;
            }
            newAngle = getAngle();
            RUN(-60,60);
        }
    }
    else
    {
//            Serial.println(">180");
        newAngle = initAngle;
        while(1)
        {
//            Serial.print(newAngle);
//            Serial.print("  ");
//            Serial.println(initAngle - newAngle);
            if( (newAngle < initAngle) && (initAngle - newAngle < 180))
            {
              RUN(0,0);
              wait(50);
              break;
            }
            newAngle = getAngle();
            RUN(-60,60);
        }        
    }
    
    RUN(0,0);
    wait(200);

    RUN(60,-60);
    wait(100);
    
    RUN(0,0);
    wait(200);
    
    int turnCount = 0;
    int finalAngle = getAngle();
    int fixAngle = 0;
    if(finalAngle > 180)
    {
       fixAngle = finalAngle - 180 - initAngle;
    }else{
      fixAngle = finalAngle - initAngle + 180;
    }
    int fixInitAngle = getAngle();
    int fixNewAngle = fixInitAngle;
    while(abs(fixNewAngle - fixInitAngle) < fixAngle)
    {
        fixNewAngle = getAngle();
        RUN(20, -20);
        wait(50);
        RUN(0, 0);
        wait(100);
    }
    RUN(-20, 20);
    wait(50);
    RUN(0, 0);
    wait(100);
    RUN(0, 0);
    wait(200);
}
void turnLeftAngle(int degree)/*0<degree<=360*/
{
    int d1 = getAngle();
    int d2 = 0;
    int d3 = 0;
    do
    {
        RUN(-240,210);
        wait(80);
        RUN(0,0);
        wait(60);
        d2 = getAngle();
        if( d1 >= d2 )
        {
            d3 += abs(d1 - d2);
        }
        else
        {
            d3 += abs(d1 + 360 - d2);
        }
        d1 = getAngle();
    }while(d3 < degree);
    RUN(0,0);
    wait(200);
}
void crossRun(int sec)//直走sec ms避开路口白线
{
    if(flagFB == 0)
    {
        RUN(30,30);
        wait(sec);
        RUN(0,0);
        wait(100);
    }
    else
    {
        RUNB(30,30);
        wait(sec);
        RUNB(0,0);
        wait(100);        
    }
}
void lineToLeft()/**遇左路口停**/
{
    while(1)
    {
        mGround = grayValue();
        if( (mGround & 0x80) == 0x80 && (mGround & 0xFC) != 0xFC )
        {
            RUN(20,20);
            wait(100);
            RUN(0,0);
            wait(300);
            break;
        }
        lineWalking();
    }
}
void lineToRight()/**遇右路口停**/
{
    while(1)
    {
        mGround = grayValue();
        if( (mGround & 0x01) == 1 && (mGround & 0x3F) != 0 )// 1100 0000  0011 1111
        {
            RUN(20,20);
            wait(100);
            RUN(0,0);
            wait(300);
            break;
        }
        lineWalking();
    }
}
void turnRight()/**右转遇白线停**/
{
    RUN(0,0);
    wait(100);
    
    do{RUN(60,-60);}
    while(colorReco(F1)!=WHITE);
    do{RUN(60,-60);}
    while(colorReco(F2)!=WHITE&&colorReco(F3)!=WHITE);
    do{RUN(55,-55);}
    while(colorReco(F3)!=WHITE&&colorReco(F4)!=WHITE);
    do{RUN(45,-45);}
    while(colorReco(F4)!=WHITE&&colorReco(F5)!=WHITE);
    
    RUN(0,0);
    wait(200);  
    
    starttime = millis();
    while (millis() - starttime < 200)
    {
        lineWalkingLow();
    } 
    
//    RUN(70,-70);
//    wait(150);
//    while(1){
//        RUN(70,-70);
//        if(colorReco(F4) == WHITE || colorReco(F5) == WHITE) break;
//    }
//    RUN(0,0);
//    wait(300);
}
void turnLeft()/**左转遇白线停**/
{
    RUN(0,0);
    wait(100);
    
    do{RUN(-60,60);}
    while(colorReco(F8)!=WHITE);
    do{RUN(-60,60);}
    while(colorReco(F7)!=WHITE&&colorReco(F6)!=WHITE);
    do{RUN(-55,55);}
    while(colorReco(F6)!=WHITE&&colorReco(F5)!=WHITE);
    do{RUN(-45,45);}
    while(colorReco(F5)!=WHITE&&colorReco(F4)!=WHITE);
    
    RUN(0,0);
    wait(200);  
    
    starttime = millis();
    while (millis() - starttime < 200)
    {
        lineWalkingLow();
    }  
  
//    RUN(-70,70);
//    wait(150);
//    while(1){
//        RUN(-70,70);
//      if(colorReco(F4) == WHITE || colorReco(F5) == WHITE) break;
//    }
//    RUN(0,0);
//    wait(300);
}
void lineToFlat(int dis)/**寻线遇景点挡板停**/
{
    if(frontDis() > dis)
    {
        wait(5);
        while(frontDis() > dis)
        {
            Serial.print("FDIS:");
            Serial.println(frontDis());
            lineWalkingLow();
        }
    }
    RUN(0,0);
    wait(200);
}
void Crash()/**碰撞景点挡板**/
{
    do{RUN(30,30);}
    while(frontDis() > 10);
    do{RUN(25,25);}
    while(frontDis() > 7);
    do{RUN(20,20);}
    while(frontDis() > 5);  
    RUN(20,20);
    wait(100);       
    RUN(40,40);
    wait(20);   
    RUN(30,30);
    wait(20);      
    RUN(-30,-30);
    wait(200);
    RUN(0,0);
    wait(200);
}
void Round1()
{
    /**出发**/
    while(frontDis() < 15);
    RUN(0,0);
    wait(1000);
    //下平台
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    //上桥
    while(1){
        lineWalkingLow();
        if(mGround == 0)
        {
          break;
        }
        delay(10);
    }
    RUN(0,0);
    wait(300);
    //过桥
    while(1){
        bridgeWalking();
        if(grayValue() != 0)
        {
          break;
        }
        delay(10);
    }
    //下桥
    RUN(30,30);
    wait(200);
    starttime = millis();
    while (millis() - starttime < 1200)
    {
        lineWalkingLow();
    }
    //上平台停
    lineToFlat(15);
    //闪灯
    LED(2);
    //转180
    turnBack();
    RUN(0,0);
    wait(200);
    /**网咖回程**/
    //下平台
    RUN(30,30);
    wait(500);
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalkingLow();
    }    
    RUN(20,20);
    wait(100);
    //上桥
    while(1){
        lineWalkingLow();
        if(grayValue() == 0)
        {
          break;
        }
        delay(10);
    }
    RUN(20,20);
    wait(100);
    //过桥
    while(1){
        bridgeWalking();
        if(grayValue() != 0)
        {
          break;
        }
        delay(10);
    }
    //下桥右路口停
    starttime = millis();
    while (millis() - starttime < 300)
    {
        lineWalkingLow();
    }
    lineToRight();
    //右转
    turnRight();
    //遇左路口停
    lineToLeft();      
    //左转
    turnLeft();  
    //遇右路口停
    lineToRight();
    //左转30度
    RUN(-60,60);
    wait(170);
    //寻线遇平台停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnBack();

    /**上04平台**/
    //左路口停
    lineToLeft();
    //直走320ms避开路口白线
    crossRun(320);
    //右转到直线上
    turnRight();
    //转180朝04平台方向
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //上平台停
    lineToFlat(15);
    //转180
    turnBack();
    //下平台左路口停
    lineToLeft();
    //直走200ms避开路口
    crossRun(200);
    //左转
    turnLeft();
    //右路口停
    lineToRight();
    
    /**第一个南湖(右)**/
    //右转
    turnRight();
    //右路口停
    lineToRight();
    //右转
    turnRight();
    //寻线遇景点停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    turnBack();
    lineToRight();
    turnRight();
    
    /**第二个南湖(左)**/
    //右路口停
    lineToRight();
    //直走300ms避开路口白线
    crossRun(200);
    //景点右路口停
    lineToRight();
    turnRight();
    //寻线遇平台停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    turnBack();  
    lineToRight();
    turnRight(); 
    lineToRight();
    turnRight(); 
    lineToLeft();
    turnLeft();
    
    /**03号平台**/
    //遇右路口停
    lineToRight();
    //直走200ms避开路口白线
    crossRun(200);
    //上平台
    lineToFlat(15);
    //转180
    turnBack();
    
    /**01龙王山**/
    //下03平台寻线左路口停
    lineToLeft();
    //直走220ms避开路口白线
    crossRun(220);
    //左转朝01龙王山
    turnLeft();
    //寻线遇平台停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnBack();  
    /**回家**/
    //寻线直接过两个路口
    starttime = millis();
    while (millis() - starttime < 6000)
    {
        lineWalkingLow();
    }
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    //右路口停
    lineToRight();
    //右转
    turnRight();  
    //上回家平台  
    while(1){
        lineWalkingLow();
        if(mGround == 0)
        {
          break;
        }
        delay(10);
    }  
    RUN(30,40);
    wait(500);
    RUN(0,0);
    wait(500);
    //转180
    turnBack();   
}
void setup() 
{
    moto.initMoto();
    pinMode(F1,INPUT);
    pinMode(F2,INPUT);
    pinMode(F3,INPUT);
    pinMode(F4,INPUT);
    pinMode(F5,INPUT);
    pinMode(F6,INPUT);
    pinMode(F7,INPUT);
    pinMode(F8,INPUT);
    
    Serial.begin(9600); 
    JY901.StartIIC(0x50);
}
void loop() 
{
//    checkSensor();//检测传感器

    //路线1
    Round1();
    //停机
    Stop();
}
