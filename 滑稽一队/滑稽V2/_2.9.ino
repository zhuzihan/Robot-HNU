int E1 = 6, M1 =8, E2 = 7, M2 = 9, R = 3, G = 4, B = 5, /**Moto IO**/
    F1 = 24, F2 =25, F3 =26, F4 = 27, F5 = 28, F6 = 29, F7 = 30, F8 = 31, /**Front gray level sensor IO**/
    G1 = 14, G2 =15, G3 =16, G4 = 17, G5 = 18, G6 = 19, G7 = 22, G8 = 23, /**Post gray level sensor IO**/
    FPin = A6, GPin = A7,/**Distance sensor IO**/
    FLPin = A12, FRPin = A13,
    BLPin = A10, BRPin = A11,
    LPin = A14, RPin = A15;
int flagFB = 0;/**0=forward 1=reverse**/
int flagFix = 0;
int offsetX,offsetY,offsetZ;
unsigned long starttime;
#include <Wire.h> //I2C Arduino Library
#define address 0x1E //001 1110b(0x3C>>1), I2C 7bit address of HMC5883
#define MagnetcDeclination 2.233 //Shanghai 5.43
#define CalThreshold 0
#define WHITE 1
#define GREEN 0
#define GRAY  0
double disA = 0,disB = 0;//FillDirect
int count = 0;//FillDirect

void wait(double sec)
{
  delay(sec);
}
void SetMoto(int side, int v)//0=L 1=R
{ 
     if(side && v>=0){     //right side
          digitalWrite(M1,HIGH);
          analogWrite(E1, v); //PWM control velocity
     }
     if(side && v<0){
          digitalWrite(M1, LOW);
          analogWrite(E1, -v); //PWM control velocity
     }
     if(side==0 && v>=0){ //left side
          digitalWrite(M2, LOW);
          analogWrite(E2, v);
     }
     if(side==0 && v<0){
          digitalWrite(M2, HIGH);
          analogWrite(E2, -v);
     }
}
void RUN( int LeftV, int RightV )//v= 0~255
{ 
     SetMoto(0, LeftV);
     SetMoto(1, RightV);
}//V 0~255
void RUNB( int LeftV, int RightV )//v= 0~255
{ 
     SetMoto(0, -RightV);
     SetMoto(1, -LeftV);
}//V 0~255
void Stop()
{
    while(1)
    {
      RUN(0,0);
    }
}
int colorReco(int sensor)/**color recognition（white 1 green 0）**/
{
    return digitalRead(sensor);  
}
//return distance (cm)
double frontDis () /**车前红外**/
{
    return analogRead(FPin)<60? 100:2076.0/(analogRead(FPin)-11.0);
}
double postDis () /**车后红外**/
{
    return analogRead(GPin)<40? 100:2076.0/(analogRead(GPin)-11.0);
}
double FLDis ()/**车底左红外**/
{
    return analogRead(FLPin);
}
double FRDis ()/**车底右红外**/
{
    return analogRead(FRPin);
}
double BLDis ()/**车中左灰度**/
{
    return analogRead(BLPin);
}
double BRDis ()/**车中右灰度**/
{
    return analogRead(BRPin);
}
double leftReco ()/**车前左灰度**/
{
    return analogRead(LPin);
}
double rightReco ()/**车前右灰度**/
{
    return analogRead(RPin);
}
void directionDet() /**方向检测**/
{
  if(frontDis() < 30) flagFB = 1; //change direction to backward
  else if(postDis()<30) flagFB = 0; //change direction to forward
  else flagFB = 0;
}
void lineWalking()/**正向寻线函数**/
{ 
    //FlagFB = 0;
    if (colorReco(F4) == WHITE) {RUN(140, 70);wait(2);}
    else if (colorReco(F5) == WHITE) {RUN(70, 140);wait(2);}
    else if (colorReco(F5) == WHITE && colorReco(F4) == WHITE) RUN(100, 100);
    
    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) {RUN(170, 70);wait(2);}
    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) {RUN(70, 170);wait(2);}
    
    else if (colorReco(F3) == WHITE) {RUN(140,-120);wait(2);}
    else if (colorReco(F6) == WHITE) {RUN(-120,140);wait(2);}

    else if (colorReco(F2) == WHITE) { RUN(170, -160);wait(2); }
    else if (colorReco(F7) == WHITE) { RUN(-160, 170);wait(2); }

    else if (colorReco(F1) == WHITE) { RUN(190, -180);wait(2); }
    else if (colorReco(F8) == WHITE) { RUN(-180, 190);wait(2); }
    
    else RUN(100,100);
}
void lineWalkingLow()
{
    //FlagFB = 0;
    if (colorReco(F4) == WHITE) {RUN(140, 70);wait(2);}
    else if (colorReco(F5) == WHITE) {RUN(70, 140);wait(2);}
    else if (colorReco(F5) == WHITE && colorReco(F4) == WHITE) RUN(60, 60);
    
    else if (colorReco(F3) == WHITE&&colorReco(F4) == WHITE) {RUN(170, 70);wait(2);}
    else if (colorReco(F5) == WHITE&&colorReco(F6) == WHITE) {RUN(70, 170);wait(2);}
    
    else if (colorReco(F3) == WHITE) {RUN(140,-120);wait(2);}
    else if (colorReco(F6) == WHITE) {RUN(-120,140);wait(2);}

    else if (colorReco(F2) == WHITE) { RUN(170, -160);wait(2); }
    else if (colorReco(F7) == WHITE) { RUN(-160, 170);wait(2); }

    else if (colorReco(F1) == WHITE) { RUN(190, -180);wait(2); }
    else if (colorReco(F8) == WHITE) { RUN(-180, 190);wait(2); }
    
    else RUN(60,60);
}
void lineWalkingRev()/**reverse line-tracking**/
{
    //FlagFB = 1;
    if (colorReco(G4) == WHITE) {RUNB(140, 70);wait(2);}
    else if (colorReco(G5) == WHITE) {RUNB(70, 140);wait(2);}
    else if (colorReco(G5) == WHITE && colorReco(G4) == WHITE) RUNB(100, 100);
    
    else if (colorReco(G3) == WHITE&&colorReco(G4) == WHITE) {RUNB(170, 70);wait(2);}
    else if (colorReco(G5) == WHITE&&colorReco(G6) == WHITE) {RUNB(70, 170);wait(2);}
    
    else if (colorReco(G3) == WHITE) {RUNB(140,-120);wait(2);}
    else if (colorReco(G6) == WHITE) {RUNB(-120,140);wait(2);}

    else if (colorReco(G2) == WHITE) { RUNB(170, -160);wait(2); }
    else if (colorReco(G7) == WHITE) { RUNB(-160, 170);wait(2); }

    else if (colorReco(G1) == WHITE) { RUNB(190, -180);wait(2); }
    else if (colorReco(G8) == WHITE) { RUNB(-180, 190);wait(2); }
    
    else RUNB(100,100);
}
void bridgeWalking()
{
    if (FLDis() < 550 && FRDis() > 550) {RUN(140, 60);wait(20);}
    else if (FLDis() > 550 && FRDis() < 550) {RUN(60, 140);wait(20);}
    else RUN(60,60);
}
//void bridgeWalking()/**Forward line-tracking **/
//{
//    //FlagFB = 0;
//    if (leftReco() >120) {RUN(150, 0);wait(20);}
//    else if (rightReco() >120) {RUN(0, 150);wait(20);}
//    else RUN(70,70);
//}
void crossRun(int sec)//直走sec ms避开路口白线
{
    if(flagFB == 0)
    {
        RUN(100,100);
        wait(sec);
        RUN(0,0);
        wait(200);
    }
    else
    {
        RUNB(100,100);
        wait(sec);
        RUNB(0,0);
        wait(200);        
    }
}
void lineToLeft()/**遇左路口停**/
{
  while(1)
  {
      if(flagFB == 0)
      {
          if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE) )
          {
              RUN(-150,-150);
              wait(100);
//              crossRun(200);
              break;
          }
          lineWalking();
      }
      else
      {
          if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
          {
              RUNB(-150,-150);
              wait(100);
//              crossRun(200);
              break;
          }
          lineWalkingRev();
      }
  }
  RUN(0,0);
  wait(300);
  if(flagFB == 0)
  {
      if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE) )
      {
          crossRun(150);
      }
  }
  else
  {
      if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
      {
          crossRun(150);
      }        
  }
}
void lineToLeftRev()/**反向遇左路口停**/
{
    while(1)
    {
        if(flagFB == 0)
        {
            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE) )
            {
                RUN(-150,-150);
                wait(100);
                break;
            }
            lineWalking();
        }
        else
        {
            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
            {
                RUNB(-150,-150);
                wait(100);
                break;
            }   
            lineWalkingRev();      
        } 
    }
    RUN(0,0);
    wait(300);
}
void lineToRight()/**遇右路口停**/
{
    while(1)
    {
        if(flagFB == 0)
        {
            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
            {
                RUN(-150,-150);
                wait(100);
                break;
            }
            lineWalking();
        }
        else
        {
            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE))
            {
                RUNB(-150,-150);
                wait(100);
                break;
            }   
            lineWalkingRev();      
        } 
    }
    RUN(0,0);
    wait(300);
    if(flagFB == 0)
    {
        if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F3)==WHITE) )
        {
            crossRun(150);
        }
    }
    else
    {
        if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G3)==WHITE) )
        {
            crossRun(150);
        }        
    }
}
void lineToRightRev()/**反向遇右路口停**/
{
    while(1)
    {
        //directionDet();
        if(flagFB == 0)
        {
            if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G6)==WHITE) )
            {
                RUN(-150,-150);
                wait(100);
                break;
            }
            lineWalking();
        }
        else
        {
            if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F6)==WHITE))
            {
                RUNB(-150,-150);
                wait(100);
                break;
            }   
            lineWalkingRev();      
        } 
    }
    RUN(0,0);
    wait(300);
}
void BridgeToRight()/**下桥遇右路口停**/
{
    while(1)
    {
        //directionDet();
        if(flagFB == 0)
        {
            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) )
            {
                RUN(-100,-100);
                wait(100);
                break;
            }
                //FlagFB = 0;
        }
        else
        {
            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) )
            {
                RUN(-100,-100);
                wait(100);
                break;
            }         
        }
        lineWalkingLow();
    }
    RUN(0,0);
    wait(300);
}
void lineToFlat(int dis)/**上平台遇挡板停**/
{
    if(flagFB == 0)
    {
        if(frontDis() > dis)
        {
            wait(5);
            while(frontDis() > dis)
            {
                lineWalking();
            }
        }
        RUN(-200,-200);
        wait(100);
        RUN(0,0);
        wait(100);
    }
    else
    {
        while(postDis() > dis)
        {
            wait(1);
            while(postDis() > dis)
            {
                lineWalkingRev();
            }
        }
        RUNB(-200,-200);
        wait(100);
        RUNB(0,0);
        wait(100);
    }
}
void FillDirect()/**红外转向修正**/
{   
      disA = postDis();
      RUN(240,-240);
      wait(50);
      RUN(0,0);
      wait(100);
      disB = postDis();
      while(count < 20){
        if( (disA - disB) > 0 )
        {
            RUN(240,-240);
            wait(20);
            RUN(0,0);
            wait(100); 
            disA = disB;
            disB = postDis(); 
            count ++;
            if( (disA - disB) < 0 )
            {
                RUN(-240,240);
                wait(20);
                RUN(0,0);
                wait(100); 
                break;
            }
        }
        else
        {
            RUN(-240,240);
            wait(20);
            RUN(0,0);
            wait(100); 
            disA = disB;
            disB = postDis(); 
            count ++;
            if( (disA - disB) > 0 )
            {
                RUN(240,-240);
                wait(10);
                RUN(0,0);
                wait(100); 
                break;
            }
        }
      }
}
void turnLeft()/**左转遇白线停**/
{
    if(flagFB == 0)
    {
        RUN(-240,210);
        wait(80);
        RUN(50,-50);
        wait(100);
        RUN(-240,210);
        wait(80);
        RUN(50,-50);
        wait(100);
        while(1)
        {
//            RUN(-240,240);
            RUN(-240,210);
            wait(80);
            RUN(50,-50);
            wait(60);
            if(colorReco(F4)==WHITE|| colorReco(F5)==WHITE) break;
        }
//        RUN(100,-100);
//        wait(100);
        RUN(-240,210);
        wait(50);
        RUN(50,-50);
        wait(100);       
    }
    else
    {
//        RUNB(-240, 240);
        RUN(-240,210);
        wait(80);
        RUN(50,-50);
        wait(100);
        RUN(-240,210);
        wait(80);
        RUN(50,-50);
        wait(100);
        while(1)
        {
//            RUNB(-240, 240);
            RUN(-240,210);
            wait(80);
            RUN(50,-50);
            wait(60);
            if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
        }
//        RUNB(100, -100);
//        wait(100); 
        RUN(-240,210);
        wait(50); 
        RUN(50,-50);
        wait(100);
    }
}
void turnRight()/**右转遇白线停**/
{
    if(flagFB == 0)
    {
//        RUN(240, -240);
//        wait(200);
        RUN(220,-240);
        wait(80);
        RUN(-50,50);
        wait(100);
        RUN(220,-240);
        wait(80);
        RUN(-50,50);
        wait(100);
        while(1){
            RUN(220,-240);
            wait(80);
            RUN(-50,50);
            wait(60);
          if(colorReco(F4) == WHITE || colorReco(F5) == WHITE) break;
        }
        RUN(220,-240);
        wait(50);
        RUN(-50,50);
        wait(100);
    }
    else
    {
//        RUNB(240, -240);
//        wait(200);
        RUN(220,-240);
        wait(80);
        RUN(-50,50);
        wait(100);
        RUN(220,-240);
        wait(80);
        RUN(-50,50);
        wait(100);
        while(1)
        {
          RUN(220,-240);
          wait(80);
          RUN(-50,50);
          wait(60);
          if(colorReco(G4) == WHITE || colorReco(G5) == WHITE) break;
        }   
        RUN(220,-240);
        wait(50); 
        RUN(-50,50);
        wait(100);
    }
}
void turnBack()/**逆时针转180**/
{
    int initAngle = getAngle();
    wait(150);
    int d1 = initAngle;
    int d2 = 0;
    int d3 = 0;
    int turnCount = 0;
    do
    {
        if(d3 < 100)
        {
//            RUN(-240, 240);
//            wait(100);
//            RUN(0, 0);
//            wait(150);
            RUN(-240, 240);
            wait(150);
            RUN(50, -50);
            wait(150);
        }
        else
        {
            RUN(-240, 240);
            wait(100);
            RUN(50, -50);
            wait(120);
        }
        d2 = getAngle();
        if( d1 >= (d2 - 5) )
        {
            d3 += abs(d1 - d2);
        }
        else if((abs(d1 - d2) > 200))
        {
            d3 += abs(d1 + 360 - d2);
        }
        d1 = getAngle();
    }while(d3 < 183);
    RUN(0,0);
    wait(200);
    int finalAngle = getAngle();
    wait(150);
    if(initAngle > finalAngle)
    {
        turnCount = (((abs(finalAngle - initAngle) - 183)) / 2) == 0 ? 1 : ((abs(finalAngle - initAngle) - 183)) / 2;
    }else
    {
        turnCount = ((abs(360 + initAngle - finalAngle - 183)) / 2) == 0 ? 1 : (abs(360 + initAngle - finalAngle - 183)) / 2;
    }
    if(turnCount > 0){
        while(turnCount--)
        {
            RUN(210, -210);
            wait(30);
            RUN(0, 0);
            wait(100);
        }
    }else{
        turnCount = - turnCount;
        while(turnCount--)
        {
            RUN(-220, 200);
            wait(30);
            RUN(0, 0);
            wait(100);
        }
    }
    RUN(0, 0);
    wait(200);
}
void turnDegrees(int degree)
{
    int initAngle = getAngle();
    wait(150);
    int d1 = initAngle;
    int d2 = 0;
    int d3 = 0;
    int turnCount = 0;
    do
    {
        if(d3 < degree * 0.5583)
        {
            RUN(-240, 240);
            wait(180);
            RUN(0, 0);
            wait(150);
        }
        else
        {
            RUN(-240, 240);
            wait(60);
            RUN(0, 0);
            wait(100);
        }
        d2 = getAngle();
        if( d1 >= (d2 - 5) )
        {
            d3 += abs(d1 - d2);
        }
        else if((abs(d1 - d2) > 200))
        {
            d3 += abs(d1 + 360 - d2);
        }
        d1 = getAngle();
    }while(d3 < degree);
    RUN(0, 0);
    wait(200);
    int finalAngle = getAngle();
    wait(150);
    if(initAngle > finalAngle)
    {
        turnCount = (((abs(finalAngle - initAngle) - degree)) / 2) == 0 ? 1 : ((abs(finalAngle - initAngle) - degree)) / 2;
    }else
    {
        turnCount = ((abs(360 + initAngle - finalAngle - degree)) / 2) == 0 ? 1 : (abs(360 + initAngle - finalAngle - degree)) / 2;
    }
    if(turnCount > 0){
        while(turnCount--)
        {
            RUN(210, -210);
            wait(30);
            RUN(0, 0);
            wait(100);
        }
    }else{
        turnCount = - turnCount;
        while(turnCount--)
        {
            RUN(-220, 220);
            wait(30);
            RUN(0, 0);
            wait(100);
        }
    }
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
void turnRightAngle(int degree)/*0<degree<=360*/
{
    int d1 = getAngle();
    int d2 = 0;
    int d3 = 0;
    do
    {
        RUN(220,-240);
        wait(60);
        RUN(0,0);
        wait(60);
        d2 = getAngle();
        if( d1 < d2 )
        {
            d3 += abs(d1 - d2);
        }
        else
        {
            d3 += abs( 360 -d1 + d2);
        }
        d1 = getAngle();
    }while(d3 < degree);
    RUN(0,0);
    wait(200);
}
void turnBackDis()/**用红外转180**/
{
  for(int i=0;i<6;i++)
  {
    RUN(-240,240);
    wait(120);
    RUN(0,0);
    wait(200);
    if(postDis() <30 ) {
      RUN(-240,240);
      wait(150);
      RUN(0,0);
      wait(200);
      break;
    }
  }
}
void Crash()/**碰撞景点挡板**/
{
    if(flagFB == 0)
    {
        while(frontDis() > 7) //>6
        {
            RUN(120,120);
            wait(100);
            RUN(0,0);
            wait(100);
        }
        RUN(0,0);
        wait(500);
        RUN(-180,-180);
        wait(80);
        RUN(0,0);
        wait(500);
    }
    else
    {
        while(postDis() > 7)
        {
            RUNB(120,120);
            wait(100);
            RUNB(0,0);
            wait(100);
        }
        RUNB(0,0);
        wait(500);
        RUNB(-180,-180);
        wait(80);
        RUNB(0,0);
        wait(500);     
    }
}
void setColor(int Red, int Green, int Blue)
{
    analogWrite(R , Red);
    analogWrite(G , Green);
    analogWrite(B , Blue);
}
void ledGreen(int count)/**闪2次绿灯**/
{
    while(count--){
        setColor(255,20,0);
        delay(500);
        setColor(0,0,0);
        delay(500);
    }
}
void ledYellow(int count)/**闪2次黄灯**/
{
    while(count--){
        setColor(255,45,0);
        delay(500);
        setColor(0,0,0);
        delay(500);
    }
}
void getRawData(int* x ,int* y,int* z)
{
    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(address, 6);
    if(6<=Wire.available()){
      *x = Wire.read()<<8; //X msb
      *x |= Wire.read(); //X lsb
      *z = Wire.read()<<8; //Z msb
      *z |= Wire.read(); //Z lsb
      *y = Wire.read()<<8; //Y msb
      *y |= Wire.read(); //Y lsb
    }
}
int calculateHeading(int* x ,int* y,int* z)
{
  float headingRadians = atan2((double)((*y)-(-125)),(double)((*x)-(-17)));//-140 -28
  // Correct for when signs are reversed.
  if(headingRadians < 0)
    headingRadians += 2*PI;
 
  int headingDegrees = headingRadians * 180/M_PI;
  headingDegrees += MagnetcDeclination; //the magnetc-declination angle 
  
  // Check for wrap due to addition of declination.
  if(headingDegrees > 360)
    headingDegrees -= 360;
  
  return headingDegrees;
}
void calibrateMag()
{
    int x,y,z; //triple axis data
    int xMax, xMin, yMax, yMin, zMax, zMin;
    //initialize the variables
    getRawData(&x,&y,&z);  
    xMax=xMin=x;
    yMax=yMin=y;
    zMax=zMin=z;
    offsetX = offsetY = offsetZ = 0;
    
    Serial.println("Starting Calibration......");
    Serial.println("Please turn your device around in 20 seconds");
    
    for(int i=0;i<200;i++)
    {
      getRawData(&x,&y,&z);
      //get Max and Min
      // this routine will capture the max and min values of the mag X, Y, and Z data while the
      // compass is being rotated 360 degrees through the level plane and the upright plane.
      // i.e. horizontal and vertical circles.
      // This function should be invoked while making continuous measurements on the magnetometers
      if (x > xMax)
        xMax = x;
      if (x < xMin )
        xMin = x;
      if(y > yMax )
        yMax = y;
      if(y < yMin )
        yMin = y;
      if(z > zMax )
        zMax = z;
      if(z < zMin )
        zMin = z;
        
      delay(100);
      
      if(i%10 == 0)
      {
        Serial.print(xMax);
        Serial.print(" ");
        Serial.println(xMin);
      }
    }
    //compute offsets
    if(abs(xMax - xMin) > CalThreshold )
      offsetX = (xMax + xMin)/2;
    if(abs(yMax - yMin) > CalThreshold )
      offsetY = (yMax + yMin)/2;
    if(abs(zMax - zMin) > CalThreshold )
      offsetZ = (zMax +zMin)/2;
    Serial.print("offsetX:");
    Serial.print("");
    Serial.print(offsetX);
    Serial.print(" offsetY:");
    Serial.print("");
    Serial.print(offsetY);
    Serial.print(" offsetZ:");
    Serial.print("");
    Serial.println(offsetZ);
    delay(5000);  
}
int getAngle()
{
    int x,y,z; //triple axis data
    getRawData(&x,&y,&z);
    return calculateHeading(&x,&y,&z);
}
void setup() 
{
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    pinMode(FPin,INPUT);
    pinMode(GPin,INPUT);
    //Initialize Serial and I2C communications
    Serial.begin(9600); 
    Wire.begin();
    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883r
    Wire.write(0x00); //select configuration register A
    Wire.write(0x70); //0111 0000b configuration
    Wire.endTransmission(); 
    
    Wire.beginTransmission(address);
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //set continuous measurement mode:0x00,single-measurement mode:0x01
    Wire.endTransmission();
//    RUN(200,-200);
//    calibrateMag();
}
void roundOne()
{
/**出发**/
    while(frontDis() < 15)
    {
         // Serial.println(frontDis());
    }
    RUN(0,0);
    wait(1000);
    //下平台
    starttime = millis();
    while (millis() - starttime < 1200)
    {
        RUN(50,50);
    }
    while(leftReco()>150 && rightReco()>150){
        lineWalking();
    }
    starttime = millis();
    while (millis() - starttime < 700)
    {
        lineWalking();
    }
    //上桥过桥
    while (leftReco()<150 || rightReco()<150)
    {
        bridgeWalking();
    }
    starttime = millis();
    while (millis() - starttime < 1300)
    {
        lineWalking();
    }
    //下桥上平台停
    if(frontDis() > 15)
    {
        wait(5);
        while(frontDis() > 15)
        {
            lineWalking();
        }
    }
    RUN(-200,-200);
    wait(100);
    RUN(0,0);
    wait(100);
    //闪灯
    ledGreen(2);
    //转180
    turnBack(); /*turnBackDis();*/

//            RUN(-240, 240);
//            wait(60);
//            RUN(50, -50);
//            wait(100);
    /*FillDirect();*/

/**网咖回程**/
    //下平台
    starttime = millis();
    while (millis() - starttime < 1200)
    {
        RUN(50,50);
    }
    while(leftReco()>150 && rightReco()>150){
        lineWalking();
    }
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalking();
    }
    //上桥过桥
    while (leftReco()<150 || rightReco()<150)
    {
        bridgeWalking();
    }
    //下桥右路口停
    lineToRight();
    RUN(0,0);
    wait(500);
    //右转
    turnRight();
    RUN(0,0);
    wait(500);

/**02普陀山**/
    //遇左路口停
    lineToLeft();
    //左转
    turnLeft();
    //遇右路口停
    lineToRight();
    //直走50ms避开路口白线
    crossRun(50);
    //左转30度
    turnLeft();
    //寻线遇平台停
    lineToFlat(20);
    //撞景点挡板得分
    Crash();
    
/**上04平台**/
    //方向检测(1)
    flagFB = 1;/*directionDet();*/
    //反向遇左路口停
    lineToLeft();
    //方向检测 (正)
    flagFB = 0;
    //右转30度
    turnRightAngle(30);/*turnRight();*/
    //直走300ms避开路口白线
    crossRun(300);
    //上平台
    lineToFlat(15);
    //转180
    turnBack();/*turnBackDis();*/
    /*FillDirect();*/

/**第一个南湖(右)**/
    starttime = millis();
    while (millis() - starttime < 1300)
    {
        lineWalking();
    }
    //下04平台寻线遇左路口停
    lineToLeft();
    //左转
    turnLeft();
    //右路口停
    lineToRight();
    //右转
    turnRight();
    //右路口停
    lineToRight();
    //右转
    turnRight(); 
    //寻线遇平台停
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalking();
    }    
    lineToFlat(20);
    //撞景点挡板得分
    Crash();   

/**第二个南湖(左)**/
    //方向检测 (1)
    flagFB = 1;
    //反向遇右路口停
    lineToRight();
    RUNB(100,100);
    wait(150);
    //方向检测 (0)
    flagFB = 0;
    //左转
    turnLeft();
    //右路口停
    lineToRight();   
    //直走300ms避开路口白线
    crossRun(300);
    //景点右路口停
    lineToRight();
    //右转
    turnRight();
    starttime = millis();
    while (millis() - starttime < 800)
    {
        lineWalking();
    }   
    //寻线遇平台停
    lineToFlat(20);
    //撞景点挡板得分
    Crash();       

/**03号平台**/
    //方向检测
    flagFB = 1;/*directionDet();*/
    //反向遇右路口停
    lineToRight();
    RUNB(100,100);
    wait(150);
    //方向检测 (0正)
    flagFB = 0;
    //左转
    turnLeft();
    //右路口停
    lineToRight();   
    //右转
    turnRight();    
    //左路口停
    lineToLeft();  
    //左转
    turnLeft();    
    //右路口停
    lineToRight(); 
    //直走200ms避开路口白线
    crossRun(200);
    //上平台
    lineToFlat(20);
    //转180
    turnBack();/*turnBackDis();*/
    /*FillDirect();*/

/**01龙王山**/
    starttime = millis();
    while (millis() - starttime < 2000)
    {
        lineWalking();
    }
    while(1)
    {
        if( colorReco(G1)==WHITE ||colorReco(G2)==WHITE )
        {
            RUN(-200,-200);
            wait(100);
            break;
        }
        lineWalking();
    }
    crossRun(300);
    flagFB = 1;
    crossRun(300);
    while(1)
    {
        if( colorReco(G1)==WHITE  )
        {
            RUNB(-150,-150);
            wait(100);
            break;
        }
        RUNB(80,80);
    }    
    turnRight();
    
    starttime = millis();
    while (millis() - starttime < 2000)
    {
        lineWalkingRev();
    }
    //寻线遇平台停
    lineToFlat(20);
    //撞景点挡板得分
    Crash();
/**回家**/
    //方向检测
    flagFB = 0;/*directionDet();*/
    //右路口停
    lineToRight();   
    //右路口停
    lineToLeft(); 
    //左转
    turnLeft(); 
    //右路口停
    lineToRight();
    //左转
    turnLeft(); 
    while (1)
    {
        lineWalking();
        if(leftReco()<100 || rightReco()<100)
        {
            RUN(-100,-100);
            wait(100);
            RUN(0,0);
            wait(300);
            break;  
        }
    }    
    starttime = millis();
    while (millis() - starttime < 1500)
    {
        lineWalkingRev();
    }    
    flagFB = 1; 
    //寻线遇平台停
    while(1)
    {
        if( colorReco(F1)==GRAY && colorReco(F2)==GRAY && colorReco(F3)==GRAY && colorReco(F4)==GRAY && colorReco(F5)==GRAY && colorReco(G6)==GRAY && colorReco(F7)==GRAY && colorReco(F8)==GRAY)
        {            
            wait(10);
            if( colorReco(F1)==GRAY && colorReco(F2)==GRAY && colorReco(F3)==GRAY && colorReco(F4)==GRAY && colorReco(F5)==GRAY && colorReco(G6)==GRAY && colorReco(F7)==GRAY && colorReco(F8)==GRAY)
            {
                lineWalkingRev();
                wait(200);
                RUNB(-200,-200);
                wait(100);
                break;
            }            
        }
        lineWalkingRev();
    }
}
void roundTwo()
{
/**出发**/
//    while(frontDis() < 15)
//    {
//         // Serial.println(frontDis());
//    }
//    RUN(0,0);
//    wait(1000);
//    //下平台
//    starttime = millis();
//    while (millis() - starttime < 1500)
//    {
//        RUN(50,50);
//    }
//    while(leftReco()>150 && rightReco()>150){
//        lineWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 2000)
//    {
//        lineWalking();
//    }
//    //上桥过桥
//    while (leftReco()<150 || rightReco()<150)
//    {
//        bridgeWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 1000)
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
//    /*FillDirect();*/

///**网咖回程**/
//    //下平台
//    starttime = millis();
//    while (millis() - starttime < 1200)
//    {
//        RUN(50,50);
//    }
//    while(leftReco()>100 && rightReco()>100){
//        lineWalking();
//    }
//    starttime = millis();
//    while (millis() - starttime < 1000)
//    {
//        lineWalking();
//    }
//    //上桥过桥
//    while (leftReco()<100 || rightReco()<100)
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
///**回家**/
//    starttime = millis();
//    while (millis() - starttime < 2000)
//    {
//        lineWalking();
//    }
//    //方向检测
//    flagFB = 0;
//    //左路口停
//    lineToLeft();    
//    //直走500ms避开路口白线
//    crossRun(500);
//    //右路口停
//    lineToLeft(); 
//    //右转
//    turnRight(); 
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
}
void loop() 
{  
//   roundOne();
//   lineToRight();
//   turnRight();
/**测试罗盘转角**/
//    turnBack();
//turnLeftAngle(90);
//turnRightAngle(90);
//ledYellow(2);
//lineToFlat(20);
//lineToLeft();
//turnLeftAngle(90);
//turnLeft();
//    while (leftReco()<150 || rightReco()<150)
//    {
//        bridgeWalking();
//    }
//    Stop();
RUN(-100,-100);
/**测试红外传感器**/
//    Serial.println(leftReco());/**车前左灰度**/
//    Serial.println(rightReco());/**车前右灰度**/
//    Serial.println(frontDis());
//    Serial.print("L:");
//    Serial.println(BLDis());
//    Serial.print("R:");
//    Serial.println(BRDis());
//    wait(300);
/**测试灰度传感器**/
//  while (colorReco(G3) == WHITE) {
//      ledYellow(2);
//      break;
//  }
//  Stop();
/**测试寻线函数**/
//  if( colorReco(F1) == WHITE ){
//      while(! (colorReco(F4) == WHITE && colorReco(F5) == WHITE) ){
//          RUN(190, -180);wait(2);
//      }     
//  }
//  RUN(0,0);
//  wait(100);
//  ledYellow(2);
  /**定时器使用**/
//    starttime = millis();
//    while (millis() - starttime < 6000)
//    {
//        bridgeWalking();
//        if((leftReco()>100 && rightReco()>100) ) break;
//    }
//    lineWalking();
}

