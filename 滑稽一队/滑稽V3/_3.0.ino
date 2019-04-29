#include "Moto.h"
#define IN3_1 12
#define IN3_2 13 
#define IN4_1 11
#define IN4_2 10
#define B 25//19

#define WHITE 1
#define GREEN 0
#define GRAY  0

Moto moto;
int F1 = 27, F2 =26, F3 =25, F4 = 24, F5 = 23, F6 = 22, F7 = 21, F8 = 20; /**Front gray level sensor IO**/
//    G1 = 14, G2 =15, G3 =16, G4 = 17, G5 = 18, G6 = 19, G7 = 22, G8 = 23; /**Post gray level sensor IO**/
//    FPin = A6, GPin = A7,/**Distance sensor IO**/
//    FLPin = A12, FRPin = A13,
//    BLPin = A10, BRPin = A11,
//    LPin = A14, RPin = A15;
    
#define flagFB 0;/**0=forward 1=reverse**/
#define flagFix 0;

int offsetX,offsetY,offsetZ;
unsigned long starttime;


void wait(double sec)
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
//return distance (cm)
//double frontDis () /**车前红外**/
//{
//    return analogRead(FPin)<60? 100:2076.0/(analogRead(FPin)-11.0);
//}
//double postDis () /**车后红外**/
//{
//    return analogRead(GPin)<40? 100:2076.0/(analogRead(GPin)-11.0);
//}
//double FLDis ()/**车底左红外**/
//{
//    return analogRead(FLPin);
//}
//double FRDis ()/**车底右红外**/
//{
//    return analogRead(FRPin);
//}
//double leftReco ()/**车前左灰度**/
//{
//    return analogRead(LPin);
//}
//double rightReco ()/**车前右灰度**/
//{
//    return analogRead(RPin);
//}
//void lineWalking()/**正向寻线函数**/
//{ 
//    //FlagFB = 0;
//    if (colorReco(F4) == WHITE) {RUN(140, 70);wait(2);}
//    else if (colorReco(F5) == WHITE) {RUN(70, 140);wait(2);}
//    else if (colorReco(F5) == WHITE && colorReco(F4) == WHITE) RUN(100, 100);
//    
//    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) {RUN(170, 70);wait(2);}
//    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) {RUN(70, 170);wait(2);}
//    
//    else if (colorReco(F3) == WHITE) {RUN(140,-120);wait(2);}
//    else if (colorReco(F6) == WHITE) {RUN(-120,140);wait(2);}
//
//    else if (colorReco(F2) == WHITE) { RUN(170, -160);wait(2); }
//    else if (colorReco(F7) == WHITE) { RUN(-160, 170);wait(2); }
//
//    else if (colorReco(F1) == WHITE) { RUN(190, -180);wait(2); }
//    else if (colorReco(F8) == WHITE) { RUN(-180, 190);wait(2); }
//    
//    else RUN(100,100);
//}
//void lineWalkingLow()
//{
//    //FlagFB = 0;
//    if (colorReco(F4) == WHITE) {RUN(140, 70);wait(2);}
//    else if (colorReco(F5) == WHITE) {RUN(70, 140);wait(2);}
//    else if (colorReco(F5) == WHITE && colorReco(F4) == WHITE) RUN(60, 60);
//    
//    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) {RUN(170, 70);wait(2);}
//    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) {RUN(70, 170);wait(2);}
//    
//    else if (colorReco(F3) == WHITE) {RUN(140,-120);wait(2);}
//    else if (colorReco(F6) == WHITE) {RUN(-120,140);wait(2);}
//
//    else if (colorReco(F2) == WHITE) { RUN(170, -160);wait(2); }
//    else if (colorReco(F7) == WHITE) { RUN(-160, 170);wait(2); }
//
//    else if (colorReco(F1) == WHITE) { RUN(190, -180);wait(2); }
//    else if (colorReco(F8) == WHITE) { RUN(-180, 190);wait(2); }
//    
//    else RUN(60,60);
//}
//void lineWalkingRev()/**reverse line-tracking**/
//{
//    //FlagFB = 1;
//    if (colorReco(G4) == WHITE) {RUNB(140, 70);wait(2);}
//    else if (colorReco(G5) == WHITE) {RUNB(70, 140);wait(2);}
//    else if (colorReco(G5) == WHITE && colorReco(G4) == WHITE) RUNB(100, 100);
//    
//    else if (colorReco(G3) == WHITE&&colorReco(G4) == WHITE) {RUNB(170, 70);wait(2);}
//    else if (colorReco(G5) == WHITE&&colorReco(G6) == WHITE) {RUNB(70, 170);wait(2);}
//    
//    else if (colorReco(G3) == WHITE) {RUNB(140,-120);wait(2);}
//    else if (colorReco(G6) == WHITE) {RUNB(-120,140);wait(2);}
//
//    else if (colorReco(G2) == WHITE) { RUNB(170, -160);wait(2); }
//    else if (colorReco(G7) == WHITE) { RUNB(-160, 170);wait(2); }
//
//    else if (colorReco(G1) == WHITE) { RUNB(190, -180);wait(2); }
//    else if (colorReco(G8) == WHITE) { RUNB(-180, 190);wait(2); }
//    
//    else RUNB(100,100);
//}
//void bridgeWalking()
//{
//    if (FLDis() < 550 && FRDis() > 550) {RUN(140, 60);wait(20);}
//    else if (FLDis() > 550 && FRDis() < 550) {RUN(60, 140);wait(20);}
//    else RUN(60,60);
//}
////void bridgeWalking()/**Forward line-tracking **/
////{
////    //FlagFB = 0;
////    if (leftReco() >150) {RUN(150, 0);wait(20);}
////    else if (rightReco() >150) {RUN(0, 150);wait(20);}
////    else RUN(70,70);
////}
//void crossRun(int sec)//直走sec ms避开路口白线
//{
//    if(flagFB == 0)
//    {
//        RUN(100,100);
//        wait(sec);
//        RUN(0,0);
//        wait(200);
//    }
//    else
//    {
//        RUNB(100,100);
//        wait(sec);
//        RUNB(0,0);
//        wait(200);        
//    }
//}
//void lineToLeft()/**遇左路口停**/
//{
//  while(1)
//  {
//      if(flagFB == 0)
//      {
//          if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE) )
//          {
//              RUN(-150,-150);
//              wait(100);
////              crossRun(200);
//              break;
//          }
//          lineWalking();
//      }
//      else
//      {
//          if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
//          {
//              RUNB(-150,-150);
//              wait(100);
////              crossRun(200);
//              break;
//          }
//          lineWalkingRev();
//      }
//  }
//  RUN(0,0);
//  wait(300);
//  if(flagFB == 0)
//  {
//      if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE) )
//      {
//          crossRun(150);
//      }
//  }
//  else
//  {
//      if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
//      {
//          crossRun(150);
//      }        
//  }
//}
//void lineToLeftRev()/**反向遇左路口停**/
//{
//    while(1)
//    {
//        if(flagFB == 0)
//        {
//            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE) )
//            {
//                RUN(-150,-150);
//                wait(100);
//                break;
//            }
//            lineWalking();
//        }
//        else
//        {
//            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
//            {
//                RUNB(-150,-150);
//                wait(100);
//                break;
//            }   
//            lineWalkingRev();      
//        } 
//    }
//    RUN(0,0);
//    wait(300);
//}
//void lineToRight()/**遇右路口停**/
//{
//    while(1)
//    {
//        if(flagFB == 0)
//        {
//            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
//            {
//                RUN(-150,-150);
//                wait(100);
//                break;
//            }
//            lineWalking();
//        }
//        else
//        {
//            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE))
//            {
//                RUNB(-150,-150);
//                wait(100);
//                break;
//            }   
//            lineWalkingRev();      
//        } 
//    }
//    RUN(0,0);
//    wait(300);
//    if(flagFB == 0)
//    {
//        if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
//        {
//            crossRun(150);
//        }
//    }
//    else
//    {
//        if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE) )
//        {
//            crossRun(150);
//        }        
//    }
//}
//void lineToRightRev()/**反向遇右路口停**/
//{
//    while(1)
//    {
//        //directionDet();
//        if(flagFB == 0)
//        {
//            if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
//            {
//                RUN(-150,-150);
//                wait(100);
//                break;
//            }
//            lineWalking();
//        }
//        else
//        {
//            if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE))
//            {
//                RUNB(-150,-150);
//                wait(100);
//                break;
//            }   
//            lineWalkingRev();      
//        } 
//    }
//    RUN(0,0);
//    wait(300);
//}
//void lineToFlat(int dis)/**上平台遇挡板停**/
//{
//    if(flagFB == 0)
//    {
//        if(frontDis() > dis)
//        {
//            wait(5);
//            while(frontDis() > dis)
//            {
//                lineWalking();
//            }
//        }
//        RUN(-200,-200);
//        wait(100);
//        RUN(0,0);
//        wait(500);
//    }
//    else
//    {
//        while(postDis() > dis)
//        {
//            wait(1);
//            while(postDis() > dis)
//            {
//                lineWalkingRev();
//            }
//        }
//        RUNB(-200,-200);
//        wait(100);
//        RUNB(0,0);
//        wait(500);
//    }
//}
//void FillDirect()/**红外转向修正**/
//{   
//      disA = postDis();
//      RUN(240,-240);
//      wait(50);
//      RUN(0,0);
//      wait(100);
//      disB = postDis();
//      while(count < 20){
//        if( (disA - disB) > 0 )
//        {
//            RUN(240,-240);
//            wait(20);
//            RUN(0,0);
//            wait(100); 
//            disA = disB;
//            disB = postDis(); 
//            count ++;
//            if( (disA - disB) < 0 )
//            {
//                RUN(-240,240);
//                wait(20);
//                RUN(0,0);
//                wait(100); 
//                break;
//            }
//        }
//        else
//        {
//            RUN(-240,240);
//            wait(20);
//            RUN(0,0);
//            wait(100); 
//            disA = disB;
//            disB = postDis(); 
//            count ++;
//            if( (disA - disB) > 0 )
//            {
//                RUN(240,-240);
//                wait(10);
//                RUN(0,0);
//                wait(100); 
//                break;
//            }
//        }
//      }
//}
//void turnLeft()/**左转遇白线停**/
//{
//    if(flagFB == 0)
//    {
//        RUN(-240,210);
//        wait(80);
//        RUN(50,-50);
//        wait(100);
//        RUN(-240,210);
//        wait(80);
//        RUN(50,-50);
//        wait(100);
//        while(1)
//        {
////            RUN(-240,240);
//            RUN(-240,210);
//            wait(80);
//            RUN(50,-50);
//            wait(60);
//            if(colorReco(F4)==WHITE|| colorReco(F5)==WHITE) break;
//        }
////        RUN(100,-100);
////        wait(100);
//        RUN(-240,210);
//        wait(50);
//        RUN(50,-50);
//        wait(100);       
//    }
//    else
//    {
////        RUNB(-240, 240);
//        RUN(-240,210);
//        wait(80);
//        RUN(50,-50);
//        wait(100);
//        RUN(-240,210);
//        wait(80);
//        RUN(50,-50);
//        wait(100);
//        while(1)
//        {
////            RUNB(-240, 240);
//            RUN(-240,210);
//            wait(80);
//            RUN(50,-50);
//            wait(60);
//            if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
//        }
////        RUNB(100, -100);
////        wait(100); 
//        RUN(-240,210);
//        wait(50); 
//        RUN(50,-50);
//        wait(100);
//    }
//}
//void turnRight()/**右转遇白线停**/
//{
//    if(flagFB == 0)
//    {
////        RUN(240, -240);
////        wait(200);
//        RUN(220,-240);
//        wait(80);
//        RUN(-50,50);
//        wait(100);
//        RUN(220,-240);
//        wait(80);
//        RUN(-50,50);
//        wait(100);
//        while(1){
//            RUN(220,-240);
//            wait(80);
//            RUN(-50,50);
//            wait(60);
//          if(colorReco(F4) == WHITE || colorReco(F5) == WHITE) break;
//        }
//        RUN(220,-240);
//        wait(50);
//        RUN(-50,50);
//        wait(100);
//    }
//    else
//    {
////        RUNB(240, -240);
////        wait(200);
//        RUN(220,-240);
//        wait(80);
//        RUN(-50,50);
//        wait(100);
//        RUN(220,-240);
//        wait(80);
//        RUN(-50,50);
//        wait(100);
//        while(1)
//        {
//          RUN(220,-240);
//          wait(80);
//          RUN(-50,50);
//          wait(60);
//          if(colorReco(G4) == WHITE || colorReco(G5) == WHITE) break;
//        }   
//        RUN(220,-240);
//        wait(50); 
//        RUN(-50,50);
//        wait(100);
//    }
//}
//void turnBack()/**逆时针转180**/
//{
//    int initAngle = getAngle();
//    wait(150);
//    int d1 = initAngle;
//    int d2 = 0;
//    int d3 = 0;
//    int turnCount = 0;
//    do
//    {
//        if(d3 < 100)
//        {
////            RUN(-240, 240);
////            wait(100);
////            RUN(0, 0);
////            wait(150);
//            RUN(-240, 240);
//            wait(150);
//            RUN(50, -50);
//            wait(150);
//        }
//        else
//        {
//            RUN(-240, 240);
//            wait(100);
//            RUN(50, -50);
//            wait(120);
//        }
//        d2 = getAngle();
//        if( d1 >= (d2 - 5) )
//        {
//            d3 += abs(d1 - d2);
//        }
//        else if((abs(d1 - d2) > 200))
//        {
//            d3 += abs(d1 + 360 - d2);
//        }
//        d1 = getAngle();
//    }while(d3 < 183);
//    RUN(0,0);
//    wait(200);
//    int finalAngle = getAngle();
//    wait(150);
//    if(initAngle > finalAngle)
//    {
//        turnCount = (((abs(finalAngle - initAngle) - 183)) / 2) == 0 ? 1 : ((abs(finalAngle - initAngle) - 183)) / 2;
//    }else
//    {
//        turnCount = ((abs(360 + initAngle - finalAngle - 183)) / 2) == 0 ? 1 : (abs(360 + initAngle - finalAngle - 183)) / 2;
//    }
//    if(turnCount > 0){
//        while(turnCount--)
//        {
//            RUN(210, -210);
//            wait(30);
//            RUN(0, 0);
//            wait(100);
//        }
//    }else{
//        turnCount = - turnCount;
//        while(turnCount--)
//        {
//            RUN(-220, 200);
//            wait(30);
//            RUN(0, 0);
//            wait(100);
//        }
//    }
//    RUN(0, 0);
//    wait(200);
//}
//void turnLeftAngle(int degree)/*0<degree<=360*/
//{
//    int d1 = getAngle();
//    int d2 = 0;
//    int d3 = 0;
//    do
//    {
//        RUN(-240,210);
//        wait(80);
//        RUN(0,0);
//        wait(60);
//        d2 = getAngle();
//        if( d1 >= d2 )
//        {
//            d3 += abs(d1 - d2);
//        }
//        else
//        {
//            d3 += abs(d1 + 360 - d2);
//        }
//        d1 = getAngle();
//    }while(d3 < degree);
//    RUN(0,0);
//    wait(200);
//
//}
//void turnRightAngle(int degree)/*0<degree<=360*/
//{
//    int d1 = getAngle();
//    int d2 = 0;
//    int d3 = 0;
//    do
//    {
//        RUN(220,-240);
//        wait(60);
//        RUN(0,0);
//        wait(60);
//        d2 = getAngle();
//        if( d1 < d2 )
//        {
//            d3 += abs(d1 - d2);
//        }
//        else
//        {
//            d3 += abs( 360 -d1 + d2);
//        }
//        d1 = getAngle();
//    }while(d3 < degree);
//    RUN(0,0);
//    wait(200);
//}
//void turnBackDis()/**用红外转180**/
//{
//  for(int i=0;i<6;i++)
//  {
//    RUN(-240,240);
//    wait(120);
//    RUN(0,0);
//    wait(200);
//    if(postDis() <30 ) {
//      RUN(-240,240);
//      wait(150);
//      RUN(0,0);
//      wait(200);
//      break;
//    }
//  }
//}
//void Crash()/**碰撞景点挡板**/
//{
//    if(flagFB == 0)
//    {
//        while(frontDis() > 7) //>6
//        {
//            RUN(120,120);
//            wait(100);
//            RUN(0,0);
//            wait(100);
//        }
//        RUN(0,0);
//        wait(500);
//        RUN(-180,-180);
//        wait(80);
//        RUN(0,0);
//        wait(500);
//    }
//    else
//    {
//        while(postDis() > 7)
//        {
//            RUNB(120,120);
//            wait(100);
//            RUNB(0,0);
//            wait(100);
//        }
//        RUNB(0,0);
//        wait(500);
//        RUNB(-180,-180);
//        wait(80);
//        RUNB(0,0);
//        wait(500);     
//    }
//}
void setColor(int Blue)
{
    digitalWrite(B,Blue);
}
void ledYellow(int count)/**闪2次黄灯**/
{
    while(count--){
        setColor(1);
        delay(500);
        setColor(0);
        delay(500);
    }
}
void setup() 
{
    moto.initPin(IN3_1, IN3_2, IN4_1, IN4_2);
    pinMode(19,OUTPUT);
    pinMode(F1,INPUT);
    pinMode(F2,INPUT);
    pinMode(F3,INPUT);
    pinMode(F4,INPUT);
    pinMode(F5,INPUT);
    pinMode(F6,INPUT);
    pinMode(F7,INPUT);
    pinMode(F8,INPUT);
    
    Serial1.begin(9600); 
}
//id roundOne()
//{
///**出发**/
//    while(frontDis() < 15)
//    {
//         // Serial.println(frontDis());
//    }
//    RUN(0,0);
//    wait(1000);
//    //下平台
//    starttime = millis();
//    while (millis() - starttime < 1200)
//    {
//        RUN(50,50);
//    }
//    while(leftReco()>150 && rightReco()>150){
//        lineWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 700)
//    {
//        lineWalking();
//    }
//    //上桥过桥
//    while (leftReco()<150 || rightReco()<150)
//    {
//        bridgeWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 1300)
//    {
//        lineWalking();
//    }
//    //下桥上平台停
//    if(frontDis() > 15)
//    {
//        wait(5);
//        while(frontDis() > 15)
//        {
//            lineWalking();
//        }
//    }
//    RUN(-200,-200);
//    wait(100);
//    RUN(0,0);
//    wait(100);
//    //闪灯
//    ledGreen(2);
//    //转180
//    turnBack(); /*turnBackDis();*/
//
////            RUN(-240, 240);
////            wait(60);
////            RUN(50, -50);
////            wait(100);
//    /*FillDirect();*/
//
///**网咖回程**/
//    //下平台
//    starttime = millis();
//    while (millis() - starttime < 1200)
//    {
//        RUN(50,50);
//    }
//    while(leftReco()>150 && rightReco()>150){
//        lineWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 800)
//    {
//        lineWalking();
//    }
//    //上桥过桥
//    while (leftReco()<150 || rightReco()<150)
//    {
//        bridgeWalking();
//    }
//    //下桥右路口停
//    lineToRight();
//    RUN(0,0);
//    wait(500);
//    //右转
//    turnRight();
//    RUN(0,0);
//    wait(500);
//
///**02普陀山**/
//    //遇左路口停
//    lineToLeft();
//    //左转
//    turnLeft();
//    //遇右路口停
//    lineToRight();
//    //直走50ms避开路口白线
//    crossRun(50);
//    //左转30度
//    turnLeft();
//    //寻线遇平台停
//    lineToFlat(20);
//    //撞景点挡板得分
//    Crash();
//    
///**上04平台**/
//    //方向检测(1)
//    flagFB = 1;/*directionDet();*/
//    //反向遇左路口停
//    lineToLeft();
//    //方向检测 (正)
//    flagFB = 0;
//    //右转30度
//    turnRightAngle(30);/*turnRight();*/
//    //直走300ms避开路口白线
//    crossRun(300);
//    //上平台
//    lineToFlat(15);
//    //转180
//    turnBack();/*turnBackDis();*/
//    /*FillDirect();*/
//
///**第一个南湖(右)**/
//    starttime = millis();
//    while (millis() - starttime < 1300)
//    {
//        lineWalking();
//    }
//    //下04平台寻线遇左路口停
//    lineToLeft();
//    //左转
//    turnLeft();
//    //右路口停
//    lineToRight();
//    //右转
//    turnRight();
//    //右路口停
//    lineToRight();
//    //右转
//    turnRight(); 
//    //寻线遇平台停
//    starttime = millis();
//    while (millis() - starttime < 800)
//    {
//        lineWalking();
//    }    
//    lineToFlat(20);
//    //撞景点挡板得分
//    Crash();   
//
///**第二个南湖(左)**/
//    //方向检测 (1)
//    flagFB = 1;
//    //反向遇右路口停
//    lineToRight();
//    RUNB(100,100);
//    wait(150);
//    //方向检测 (0)
//    flagFB = 0;
//    //左转
//    turnLeft();
//    //右路口停
//    lineToRight();   
//    //直走300ms避开路口白线
//    crossRun(300);
//    //景点右路口停
//    lineToRight();
//    //右转
//    turnRight();
//    starttime = millis();
//    while (millis() - starttime < 800)
//    {
//        lineWalking();
//    }   
//    //寻线遇平台停
//    lineToFlat(20);
//    //撞景点挡板得分
//    Crash();       
//
///**03号平台**/
//    //方向检测
//    flagFB = 1;/*directionDet();*/
//    //反向遇右路口停
//    lineToRight();
//    RUNB(100,100);
//    wait(150);
//    //方向检测 (0正)
//    flagFB = 0;
//    //左转
//    turnLeft();
//    //右路口停
//    lineToRight();   
//    //右转
//    turnRight();    
//    //左路口停
//    lineToLeft();  
//    //左转
//    turnLeft();    
//    //右路口停
//    lineToRight(); 
//    //直走300ms避开路口白线
//    crossRun(300);
//    //上平台
//    lineToFlat(20);
//    //转180
//    turnBack();/*turnBackDis();*/
//    /*FillDirect();*/
//
///**01龙王山**/
//    starttime = millis();
//    while (millis() - starttime < 1700)
//    {
//        lineWalking();
//    }
//    while(1)
//    {
//        if( colorReco(G1)==WHITE ||colorReco(G2)==WHITE )
//        {
//            RUN(-200,-200);
//            wait(100);
//            break;
//        }
//        lineWalking();
//    }
//    crossRun(300);
//    flagFB = 1;
//    crossRun(300);
//    while(1)
//    {
//        if( colorReco(G1)==WHITE  )
//        {
//            RUNB(-150,-150);
//            wait(100);
//            break;
//        }
//        RUNB(80,80);
//    }    
//    turnRight();
//    
//    starttime = millis();
//    while (millis() - starttime < 2000)
//    {
//        lineWalkingRev();
//    }
//    //寻线遇平台停
//    lineToFlat(20);
//    //撞景点挡板得分
//    Crash();
///**回家**/
//    //方向检测
//    flagFB = 0;/*directionDet();*/
//    //右路口停
//    lineToRight();   
//    //右路口停
//    lineToLeft(); 
//    //左转
//    turnLeft(); 
//    //右路口停
//    lineToRight();
//    //左转
//    turnLeft(); 
//    while (1)
//    {
//        lineWalking();
//        if(leftReco()<100 || rightReco()<100)
//        {
//            RUN(-100,-100);
//            wait(100);
//            RUN(0,0);
//            wait(300);
//            break;  
//        }
//    }    
//    starttime = millis();
//    while (millis() - starttime < 1500)
//    {
//        lineWalkingRev();
//    }    
//    flagFB = 1; 
//    //寻线遇平台停
//    while(1)
//    {
//        if( colorReco(F1)==GRAY && colorReco(F2)==GRAY && colorReco(F3)==GRAY && colorReco(F4)==GRAY && colorReco(F5)==GRAY && colorReco(G6)==GRAY && colorReco(F7)==GRAY && colorReco(F8)==GRAY)
//        {            
//            wait(10);
//            if( colorReco(F1)==GRAY && colorReco(F2)==GRAY && colorReco(F3)==GRAY && colorReco(F4)==GRAY && colorReco(F5)==GRAY && colorReco(G6)==GRAY && colorReco(F7)==GRAY && colorReco(F8)==GRAY)
//            {
//                lineWalkingRev();
//                wait(200);
//                RUNB(-200,-200);
//                wait(100);
//                break;
//            }            
//        }
//        lineWalkingRev();
//    }
//}
void loop() {
//  while (colorReco(F1) == WHITE) {
//      ledYellow(2);
//      break;
//  }
//  Stop();
Serial1.print(colorReco(F1));
Serial1.print(colorReco(F2));
Serial1.print(colorReco(F3));
Serial1.print(colorReco(F4));
Serial1.print(colorReco(F5));
Serial1.print(colorReco(F6));
Serial1.print(colorReco(F7));
Serial1.println(colorReco(F8));
wait(500);
//ledYellow(2);
}
