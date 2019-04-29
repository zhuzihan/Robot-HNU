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
#define TurnLeft 0
#define TurnRight 1

Moto moto;
int gray = 0x00;
int mGround = 0x00;
float initAngle, newAngle;
unsigned long starttime;
unsigned char JY901_set_zero_hex[5] = {0xff,0xaa,0x01,0x04,0x00};
unsigned char JY901_save_hex[5] = {0xff,0xaa,0x00,0x00,0x00};
int F1 = 12, F2 =8, F3 =7, F4 = 6, F5 = 5, F6 = 4, F7 = 2, F8 = A2; /**Front gray level sensor IO**/
float initYAngle;
float newYAngle;

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
    float dis = analogRead(FPin)<60? 100:2076.0/(analogRead(FPin)-11.0);
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
void bridgeWalking  ()/**过桥函数**/
{
    if (FLDis() < 400 && FRDis() > 400) {RUN(45, 35);wait(2);}
    else if (FLDis() > 400 && FRDis() < 400) {RUN(35, 45);wait(2);}
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
    else if ( mGround == 0x06 ) { RUN(25, -35); wait(2); } /**0000 0110**/
    else if ( mGround == 0x60 ) { RUN(-35, 25); wait(2); } /**0110 0000**/
    else if ( mGround == 0x02 ) { RUN(35, -45); wait(2); } /**0000 0010**/
    else if ( mGround == 0x40 ) { RUN(-45, 35); wait(2); } /**0100 0000**/
    else if ( mGround == 0x01 ) { RUN(40, -60); wait(2); } /**0000 0001**/
    else if ( mGround == 0x80 ) { RUN(-60, 40); wait(2); } /**1000 0000**/
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
    else if ( mGround == 0x06 ) { RUN(20, -30); wait(2); } /**0000 0110**/
    else if ( mGround == 0x60 ) { RUN(-30, 20); wait(2); } /**0110 0000**/
    else if ( mGround == 0x02 ) { RUN(30, -40); wait(2); } /**0000 0010**/
    else if ( mGround == 0x40 ) { RUN(-40, 30); wait(2); } /**0100 0000**/
    else if ( mGround == 0x01 ) { RUN(40, -50); wait(2); } /**0000 0001**/
    else if ( mGround == 0x80 ) { RUN(-50, 40); wait(2); } /**1000 0000**/
    else RUN(40,40);
}
float getAngle()
{
    JY901.GetAngle();
    float angle = (float)JY901.stcAngle.Angle[2]/32768*180;
    return angle < 0 ? 360 + angle : angle;
}
float getAngleY()
{
    JY901.GetAngle();
    float angle = (float)JY901.stcAngle.Angle[0]/32768*180;
//    return angle < 0 ? 360 + angle : angle;
    return angle;
}
void setZZero()
{
    delay(50);
    Serial.write(JY901_set_zero_hex,5);
    delay(50);
//    Serial.write(JY901_save_hex,5);
//    delay(50);
}
void turnBack()
{
    wait(50);
    setZZero();
    wait(100);
    initAngle = getAngle();
    newAngle = initAngle;
    int tempAngle = 0;
//    Serial.print("initAngle:  ");
//    Serial.println(initAngle);
    while(abs(initAngle - newAngle) < 150)
    {
        newAngle = getAngle();
        RUN(-60,60);
        wait(50);
//        Serial.print("newAngle:  ");
//        Serial.println(newAngle);
//        Serial.println(abs(initAngle - newAngle));
    }
    RUN(0,0);
    wait(150);
    tempAngle = getAngle();
    wait(50);
    if(abs(tempAngle - 180) < 70)
    { 
        RUN(50,-50);
        wait(150);
    }else{
        RUN(60,-60);
        wait(160);
    }
    
    RUN(0,0);
    wait(200);
    int turnCount = 0;
    int finalAngle = getAngle();
//    Serial.print("finalAngle:  ");
//    Serial.println(finalAngle);
    int fixAngle = 0;
    fixAngle = finalAngle - 180;
    int fixInitAngle = getAngle();
    int fixNewAngle = fixInitAngle;
    if (fixAngle > 0){
        while(1)
        {
            fixNewAngle = getAngle();
            if(abs(fixNewAngle - fixInitAngle) > fixAngle)
            {
                RUN(0,0);
                delay(50);
                break;
            }
            RUN(30, -30);
            wait(30);
            RUN(0, 0);
            wait(70);
        }
        finalAngle = getAngle();
        if((finalAngle - initAngle) < 176.5){
            RUN(-30, 30);
            wait(50);
        }
        if((finalAngle - initAngle) > 177 && (finalAngle - initAngle) < 180){
            RUN(-25, 25);
            wait(30);
        }
        RUN(0, 0);
    }else{
        while(1)
        {
            fixNewAngle = getAngle();
            if(abs(fixNewAngle - fixInitAngle) > abs(fixAngle))
            {
                RUN(0,0);
                delay(50);
                break;
            }
            RUN(-30, 30);
            wait(30);
            RUN(0, 0);
            wait(70);
        }
        RUN(30, -30);
        wait(50);
        RUN(0, 0);
    }
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
    while (millis() - starttime < 300)
    {
        lineWalkingLow();
    }  
  
}
void lineToFlat(int dis)/**寻线遇景点挡板停**/
{
    if(frontDis() > dis)
    {
        wait(5);
        while(frontDis() > dis)
        {
//            Serial.print("FDIS:");
//            Serial.println(frontDis());
            lineWalkingLow();
        }
    }
    RUN(0,0);
    wait(200);
}
void grayToFlat()
{
    lineToLeft();
//    RUN(30,30);
//    wait(500);
//    init
    newYAngle = getAngleY();
    if(newYAngle < -1 || newYAngle > 1)
    {
        while(1)
        {
            RUN(30, 30);
            wait(5);
            newYAngle = getAngleY();
            if(newYAngle > -1.5 && newYAngle < 1.5)
            {
                RUN(0,0);
                wait(50);
                break;
            }
            
        }
    }
    RUN(30,30);
    wait(200);
    RUN(0,0);
    wait(400);
}

void angleToFlat()
{
    initYAngle = getAngleY();
    newYAngle = initYAngle;
    wait(50);
    while(1){
        lineWalkingLow();
        newYAngle = getAngleY();
//        Serial.print(initYAngle);
//        Serial.print("   ");
//        Serial.println(newYAngle);
//        Serial.println(abs(newYAngle - initYAngle));
        if(abs(newYAngle - initYAngle) > 18)
        {
            RUN(20,20);
            wait(200);
            RUN(0,0);
            wait(50);
            break;
        }
        delay(200);
    }
    delay(100);
    newYAngle = getAngleY();
    if(newYAngle < -1 || newYAngle > 1)
    {
        while(1)
        {
            RUN(15, 15);
            newYAngle = getAngleY();
            if(newYAngle > -1 && newYAngle < 1)
            {
                break;
            }
        }
    }
    RUN(0, 0);
    wait(100);
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
    wait(400);
}
void Round1()/**先走右**/
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
          wait(10);
          if(mGround == 0)
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
          wait(10);
          if(grayValue() != 0)
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
    //寻线遇景点停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();

    /**上04平台**/
    //左路口停
    lineToLeft();
    //直走320ms避开路口白线
    crossRun(340);
    //右转到直线上
    turnRight();
    RUN(0,0);
    wait(300);
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
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    //下平台左路口停
    lineToLeft();
    //直走200ms避开路口
    crossRun(150);
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
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    turnLeft();//turnBack();
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
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    turnLeft();//turnBack();  
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
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
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
    turnLeft();//turnBack();  
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
    RUN(30,40);/**不确定**/
    wait(500);
    RUN(0,0);
    wait(500);
    //转180
    turnBack();   
    RUN(0,0);
    wait(500);
}
void Round2()/**先走左**/
{
    /**出发**/
    while(frontDis() < 15);
    RUN(0,0);
    wait(1000);
    RUN(20,20);
    wait(500);
    //下平台
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalkingLow();
    }
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //上桥
    initYAngle = getAngle();
    newYAngle = initYAngle;
    wait(50);
    while(1){
        lineWalkingLow();
        newYAngle = getAngleY();
//        Serial.print(initYAngle);
//        Serial.print("   ");
//        Serial.println(newYAngle);
//        Serial.println(abs(newYAngle - initYAngle));
        if( abs(newYAngle - initYAngle) > 13)
        {
            if(abs(newYAngle - initYAngle) > 13)
                break;
        }
        delay(200);
    }
    RUN(0,0);
    wait(300);
    while(1){
      RUN(30,30);
      newYAngle = getAngleY();
      if(newYAngle > -1 && newYAngle < 1){
          break;
      }
    }
//    while(1){
//        lineWalkingLow();
//        if(mGround == 0)
//        {
//          wait(10);
//          if(mGround == 0)
//          break;
//        }
//        delay(10);
//    }

    //过桥
//    starttime = millis();
//    while (millis() - starttime < 1200)
//    {
//        bridgeWalking();
//    }
    initYAngle = 0;
    newYAngle = initYAngle;
    wait(50);
    while(1){
        bridgeWalking();
        newYAngle = getAngleY();
//        Serial.print(initYAngle);
//        Serial.print("   ");
//        Serial.println(newYAngle);
//        Serial.println(abs(newYAngle - initYAngle));
        if( (newYAngle - initYAngle) > 8 )
        {
            RUN(23,23);
            wait(50);            
            RUN(15,15);
            wait(100);
            RUN(5,5);
            wait(50);
            break;
        }
        delay(30);
    }
    RUN(15,15);
    wait(300);
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalkingLow();
    }
    //上平台停
    angleToFlat();//lineToFlat(15);
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
    while (millis() - starttime < 600)
    {
        lineWalkingLow();
    }    
    //上桥
    initYAngle = 0;
    newYAngle = initYAngle;
    wait(50);
    while(1){
        RUN(30,30);
        newYAngle = getAngleY();
//        Serial.print(initYAngle);
//        Serial.print("   ");
//        Serial.println(newYAngle);
//        Serial.println(abs(newYAngle - initYAngle));
        if(abs(newYAngle - initYAngle) > 13)
        {
            RUN(0, 0);
            wait(100);
            break;
        }
        delay(200);
    }
//    while(1){
//        lineWalkingLow();
//        if(grayValue() == 0)
//        {
//          break;
//        }
//        delay(10);
//    }
    starttime = millis();
    while (millis() - starttime < 1200)
    {
        bridgeWalking();
    }
    initYAngle = getAngleY();
    newYAngle = initYAngle;
    wait(50);
    while(1){
        bridgeWalking();
        newYAngle = getAngleY();
//        Serial.print(initYAngle);
//        Serial.print("   ");
//        Serial.println(newYAngle);
//        Serial.println(abs(newYAngle - initYAngle));
        if(abs(newYAngle - initYAngle) > 8)
        {
            RUN(23,23);
            wait(50);   
            RUN(15,15);
            wait(100);
            RUN(5,5);
            wait(50);
            break;
        }
        delay(30);
    }
    //下桥右路口停
    RUN(10,10);
    wait(250);
    starttime = millis();
    while (millis() - starttime < 500)
    {
        lineWalkingLow();
    }
    //遇右路口停
    lineToRight();
    //右转
    turnRight();

    /**前往03平台**/
    //遇右路口停
    lineToRight();      
    //右转
    turnRight();
    //寻线至03平台下
    starttime = millis();
    while (millis() - starttime < 3000)
    {
        lineWalking();
    }
    //寻线遇平台停
    grayToFlat();//lineToFlat(15);    
    //转180
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    /**01龙王山**/
    //下平台左路口停
    lineToLeft();
    //直走150ms避开路口
    crossRun(150);
    //左转
    turnLeft();
    //寻线至01景点下
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //寻线遇平台停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();  

    /**第一个南湖(左)**/
    //寻线过一个路口
    starttime = millis();
    while (millis() - starttime < 3800)
    {
        lineWalkingLow();
    }
    //右路口停
    lineToRight();
    //右转
    turnRight();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();    
    //左路口停
    lineToLeft();
    //左转
    turnLeft(); 
      
    //寻线遇景点停
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    //转180
    turnRight();//turnBack();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    
    /**第二个南湖(右)**/
    //右路口停
    lineToRight();
    //直走300ms避开路口白线
    crossRun(200);
    //景点左路口停
    lineToLeft();
    //左转
    turnLeft();
    //寻线遇平台停
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    //转180
    turnRight();//turnBack();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    //左路口停
    lineToLeft();
    //右转
    turnRight(); 

    /**02普陀山**/
    starttime = millis();
    while (millis() - starttime < 500)
    {
        lineWalkingLow();
    }
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalking();
    }
    //寻线遇景点停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();

    /**上04平台**/
    //左路口停
    lineToLeft();
    wait(100);
    //直走400ms避开路口白线
    while(1)
    {
      RUN(30, 30);
      if(colorReco(F1) == 1)
      {
          break;
      }
      wait(5);
    }
    turnRight();
    starttime = millis();
    while (millis() - starttime < 600)
    {
        lineWalkingLow();
    }
    RUN(0,0);
    wait(100);
    //右转到直线上
    turnRight();
    RUN(0,0);
    wait(100);
    //转180朝04平台方向
    //turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
//    //上平台停
    grayToFlat();//lineToFlat(15);
    //转180
    turnBack();
//    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    //下平台左路口停
    lineToLeft();
    //直走200ms避开路口
    crossRun(150);

    /**回家**/
    //右路口停
    lineToRight();    
    //右转
    turnRight();
    //右路口停
    lineToRight();
    //右转
    turnRight();  
//    上回家平台  
    while(1){
        lineWalkingLow();
        if(mGround == 0)
        {
          break;
        }
        delay(10);
    }  
    RUN(30,30);
    wait(500);
    RUN(0,0);
    wait(500);
    //转180
    turnBack();   
    RUN(0,0);
    wait(500);
}
void Round3()/**先走右 不用红外上平台**/
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
          wait(10);
          if(mGround == 0)
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
          wait(10);
          if(grayValue() != 0)
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
    grayToFlat();
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
    //寻线遇景点停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();

    /**上04平台**/
    //左路口停
    lineToLeft();
    //直走320ms避开路口白线
    crossRun(340);
    //右转到直线上
    turnRight();
    RUN(0,0);
    wait(300);
    //转180朝04平台方向
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //上平台停
    grayToFlat();
    //转180
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    //下平台左路口停
    lineToLeft();
    //直走200ms避开路口
    crossRun(150);
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
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    turnLeft();//turnBack();
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
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    turnLeft();//turnBack();  
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
    grayToFlat();
    //转180
    turnBack();
    
    /**01龙王山**/
    //下03平台寻线左路口停
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
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
    turnLeft();//turnBack();  
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
    RUN(30,40);/**不确定**/
    wait(500);
    RUN(0,0);
    wait(500);
    //转180
    turnBack();   
    RUN(0,0);
    wait(500);
}
void Round4()/**先走左 不用红外上平台**/
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
          wait(10);
          if(mGround == 0)
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
          wait(10);
          if(grayValue() != 0)
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
    grayToFlat();
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
    //遇右路口停
    lineToRight();
    //右转
    turnRight();

    /**前往03平台**/
    //遇右路口停
    lineToRight();      
    //右转
    turnRight();
    //寻线至03平台下
    starttime = millis();
    while (millis() - starttime < 3000)
    {
        lineWalking();
    }
    //寻线遇平台停
    grayToFlat();  
    //转180
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    /**01龙王山**/
    //下平台左路口停
    lineToLeft();
    //直走150ms避开路口
    crossRun(150);
    //左转
    turnLeft();
    //寻线至01景点下
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //寻线遇平台停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();  

    /**第一个南湖(左)**/
    //寻线过一个路口
    starttime = millis();
    while (millis() - starttime < 3800)
    {
        lineWalkingLow();
    }
    //右路口停
    lineToRight();
    //右转
    turnRight();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();    
    //左路口停
    lineToLeft();
    //左转
    turnLeft();   
    //寻线遇景点停
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    //转180
    turnRight();//turnBack();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    
    /**第二个南湖(右)**/
    //右路口停
    lineToRight();
    //直走300ms避开路口白线
    crossRun(200);
    //景点左路口停
    lineToLeft();
    //左转
    turnLeft();
    //寻线遇平台停
    lineToFlat(10);
    //撞景点挡板得分
    Crash();
    //转180
    turnRight();//turnBack();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    //左路口停
    lineToLeft();
    //左转
    turnLeft();
    //左路口停
    lineToLeft();
    //右转
    turnRight(); 

    /**02普陀山**/
    starttime = millis();
    while (millis() - starttime < 500)
    {
        lineWalkingLow();
    }
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalking();
    }
    //寻线遇景点停
    lineToFlat(15);
    //撞景点挡板得分
    Crash();
    //转180
    turnLeft();//turnBack();

    /**上04平台**/
    //左路口停
    lineToLeft();
    //直走340ms避开路口白线
    crossRun(340);
    //右转到直线上
    turnRight();
    RUN(0,0);
    wait(300);
    //转180朝04平台方向
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1000)
    {
        lineWalking();
    }
    //上平台停
    grayToFlat();
    //转180
    turnBack();
    //寻线越过路口
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingLow();
    }
    //下平台左路口停
    lineToLeft();
    //直走200ms避开路口
    crossRun(150);

    /**回家**/
    //右路口停
    lineToRight();    
    //右转
    turnRight();
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
    RUN(0,0);
    wait(500);    
}
void setup() 
{
    moto.initMoto();
    moto.setCoefficient(0.65);
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

//    RUN(0,0);
//    wait(1000);
//    RUN(20,20);
//    wait(500);
//    //下平台
//    starttime = millis();
//    while (millis() - starttime < 1000)
//    {
//        lineWalkingLow();
//    }
//    starttime = millis();
//    while (millis() - starttime < 3000)
//    {
//        lineWalking();
//    }
//    Stop();
    //路线2
//    while(1){
//      Serial.println(getAngleY());
//      delay(200);
//    }
    
    Round2();
    //路线2
    Round2();
    Stop();
//    //停机
//    while(1)
//    {
//      Serial.println(getAngleY());
//    }
//    lineToLeft();
//    wait(100);
//    //直走400ms避开路口白线
//    while(1)
//    {
//      RUN(30, 30);
//      if(colorReco(F1) == 1)
//      {
//          break;
//      }
//      wait(5);
//    }
//    turnRight();
//    starttime = millis();
//    while (millis() - starttime < 500)
//    {
//        lineWalkingLow();
//    }
//    RUN(0,0);
//    wait(300);
//    //右转到直线上
//    turnRight();
//    RUN(0,0);
//    wait(200);
//    //转180朝04平台方向
//    //turnBack();
//    //寻线越过路口
//    starttime = millis();
//    while (millis() - starttime < 1000)
//    {
//        lineWalking();
//    }
////    //上平台停
//    grayToFlat();//lineToFlat(15);
//    //转180
//    turnBack();
////    //寻线越过路口
//    starttime = millis();
//    while (millis() - starttime < 1500)
//    {
//        lineWalkingLow();
//    }
//    //下平台左路口停
//    lineToLeft();
//    //直走200ms避开路口
//    crossRun(150);
//
//    /**回家**/
//    //右路口停
//    lineToRight();    
//    //右转
//    turnRight();
//    //右路口停
//    lineToRight();
//    //右转
//    turnRight();  
////    上回家平台  
//    while(1){
//        lineWalkingLow();
//        if(mGround == 0)
//        {
//          break;
//        }
//        delay(10);
//    }  
//    RUN(30,30);
//    wait(500);
//    RUN(0,0);
//    wait(500);
//    //转180
//    turnBack();   
//    RUN(0,0);
//    wait(500);    
//    Stop();    
}
