int E1 = 6, M1 =8, E2 = 7, M2 = 9, R = 3, G = 4, B = 5, /**Moto IO**/
    F1 = 24, F2 =25, F3 =26, F4 = 27, F5 = 28, F6 = 29, F7 = 30, F8 = 31, /**Front gray level sensor IO**/
    G1 = 14, G2 =15, G3 =16, G4 = 17, G5 = 18, G6 = 19, G7 = 22, G8 = 23, /**Post gray level sensor IO**/
    FPin = A6, GPin = A7,/**Distance sensor IO**/
    LPin = A14, RPin = A15;
    
int flagFB = 0;/**0=forward 1=reverse**/
double disA = 0,disB = 0;
int count = 0;
//int i = 0;
//int redVal   = 255;  
//int greenVal = 1;   
//int blueVal  = 1;
#define WHITE 1
#define GREEN 0
#define GRAY  0

unsigned long starttime;
//unsigned long stoptime;
//unsigned long looptime;

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
double frontDis () /*front distance detection*/
{
    return analogRead(FPin)<60? 100:2076.0/(analogRead(FPin)-11.0);
//    return analogRead(FPin);
}
double postDis ()
{
    return analogRead(GPin)<40? 100:2076.0/(analogRead(GPin)-11.0);
}
double leftReco ()
{
    return analogRead(LPin);
}
double rightReco ()
{
    return analogRead(RPin);
}
void directionDet() /**directionDetection**/
{
  if(frontDis() < 30) flagFB = 1; //change direction to backward
  else if(postDis()<30) flagFB = 0; //change direction to forward
  else flagFB = 0;
}
void lineWalking()/**Forward line-tracking **/
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
void bridgeWalking()/**Forward line-tracking **/
{
    //FlagFB = 0;
    if (leftReco() >100) {RUN(150, -70);wait(50);}
    else if (rightReco() >100) {RUN(-70, 150);wait(50);}
    else RUN(60,60);
}
void lineToLeft()/**line-tracking stop at the left corner**/
{
  while(1)
  {
    //directionDet();
    if(flagFB == 0)
    {
        if( (colorReco(F8)==WHITE &&  colorReco(F6)==WHITE) || (colorReco(F7)==WHITE &&  colorReco(F5)==WHITE) || (colorReco(F8)==WHITE &&  colorReco(F4)==WHITE) )
        {
            RUN(-150,-150);
            wait(100);
            break;
        }
        lineWalking();
    }
    else
    {
        if( (colorReco(G8)==WHITE &&  colorReco(G6)==WHITE) || (colorReco(G7)==WHITE &&  colorReco(G5)==WHITE) || (colorReco(G8)==WHITE &&  colorReco(G4)==WHITE) )
        {
            RUNB(-140,-140);
            wait(80);
            break;
        }
        lineWalkingRev();
    }
  }
  RUN(0,0);
  wait(300);
}
void lineToRight()/**line-tracking stop at the right corner**/
{
    while(1)
    {
        //directionDet();
        if(flagFB == 0)
        {
            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F1)==WHITE &&  colorReco(F4)==WHITE))
            {
                RUN(-150,-150);
                wait(100);
                break;
            }
            lineWalking();
        }
        else
        {
            if( (colorReco(G1)==WHITE &&  colorReco(G3)==WHITE) || (colorReco(G2)==WHITE &&  colorReco(G4)==WHITE) || (colorReco(G1)==WHITE &&  colorReco(G4)==WHITE) )
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
void BridgeToRight()
{
    while(1)
    {
        //directionDet();
        if(flagFB == 0)
        {
            if( (colorReco(F1)==WHITE &&  colorReco(F3)==WHITE) || (colorReco(F2)==WHITE &&  colorReco(F4)==WHITE) || (colorReco(F1)==WHITE &&  colorReco(F4)==WHITE))
            {
                RUN(-100,-100);
                wait(100);
                break;
            }
                //FlagFB = 0;
        }
        else
        {
            if(colorReco(G1)==WHITE && colorReco(G2)==WHITE)
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
void FillDirect()
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
void turnLeft()
{
    RUN(-240,240);
    wait(200);
    while(1){
      RUN(-240,240);
      if(colorReco(F4)==WHITE|| colorReco(F5)==WHITE) break;
    }
    RUN(100,-100);
    wait(100);
}
void turnRight()
{
    RUN(240,-240);
    wait(200);
    while(1){
      RUN(240,-240);
      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
    }
    RUN(-100,100);
    wait(100);
}
void turnBack()/**180°转**/
{
//    RUN(180,-180);
//    wait(1000);
//    while(1){
//      RUN(-240,240);
//      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
//    }
  starttime = millis();
  for(int i=0;i<12;i++)
  {
    RUN(-240,240);
    wait(130);
    RUN(0,0);
    wait(200);
  }
}
void turnBackDis()
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
void turn_LB()/**反向左转**/
{
    RUNB(-240,240);
    wait(300);
    while(1){
      RUNB(-240,240);
      if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
    }
}
void turn_RB()/**反向右转**/
{
    RUNB(240,-240);
    wait(300);
    while(1){
      RUNB(240,-240);
      if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
    }
}
void Crash()
{
    while(frontDis()> 7)
    {
        RUN(60,60);
        wait(150);
        RUN(0,0);
        wait(100);
    }
    RUN(0,0);
    wait(500);
    RUN(-180,-180);
    wait(50);
    RUN(0,0);
    wait(500);
}
void setColor(int Red, int Green, int Blue)
{
    analogWrite(R , Red);
    analogWrite(G , Green);
    analogWrite(B , Blue);
}
void ledGreen(int count)
{
    while(count--){
        setColor(255,20,0);
        delay(500);
        setColor(0,0,0);
        delay(500);
    }
}
void ledYellow(int count)
{
    while(count--){
        setColor(255,45,0);
        delay(500);
        setColor(0,0,0);
        delay(500);
    }
}
void setup() 
{
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    pinMode(FPin,INPUT);
    pinMode(GPin,INPUT);
    Serial.begin(9600); 
    //pinMode(FPin,INPUT);
} 
void loop() 
{  
/**    路线一   **/
/**出发-网咖展示**/
//    //出发
//    while(frontDis() < 15)
//    {
//         // Serial.println(frontDis());
//    }
//
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
//    starttime = millis();
//    while (millis() - starttime < 1000)
//    {
//        lineWalking();
//    }
//    //下桥上平台停
//    while(frontDis() > 20)
//    {
//        lineWalking();
//    }
//    RUN(-200,-200);
//    wait(100);
//    RUN(0,0);
//    wait(100);
//    //闪灯
//    ledGreen(2);
//    //转180
//    turnBackDis();
//    FillDirect();
///*
//    while (frontDis() > 12)
//    {
//        RUN(100,100);
//        wait(50);
//        RUN(0,0);
//        wait(200);
//    }
//    while(frontDis()<15){
//        RUN(-100,-100);
//        wait(50);
//        RUN(0,0);
//        wait(200);
//    }
//*/

/**网咖回程**/
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
//    //跷跷板的右路口停
//
//    lineToRight();
//    RUN(0,0);
//    wait(500);
//    turnRight();
//    RUN(0,0);
//    wait(500);

/**02普陀山**/
//    //遇左路口停
//    lineToLeft();
//    //左转
//    turnLeft();
//    //遇右路口停
//    lineToRight();
//    //左转30度
//    turnLeft();
//    //寻线遇平台停
//    while(frontDis() > 20)
//    {
//        lineWalking();
//    }
//    RUN(-200,-200);
//    wait(100);
//    RUN(0,0);
//    wait(100);
//    //撞景点挡板得分
//    Crash();
    
/**上04平台**/
//    //方向检测
//    directionDet();
//    //反向遇左路口停
//    lineToLeft();
//    //方向检测
//    flagFB = 0;
//    //右转
//    turnRight();
//    //上平台
//    while(frontDis() > 20)
//    {
//        lineWalking();
//    }
//    RUN(-200,-200);
//    wait(100);
//    RUN(0,0);
//    wait(100);
//    //转180
//    turnBackDis();
//    FillDirect();
    //
    //
    //
    //
    //
    //
    //
/**测试红外传感器**/
//    Serial.println(rightReco());
//    Serial.println(frontDis());
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
//      
//  }
//  RUN(0,0);
//  wait(100);
//  ledYellow(2);`
  /**定时器使用**/
//    starttime = millis();
//    while (millis() - starttime < 6000)
//    {
//        bridgeWalking();
//        if((leftReco()>100 && rightReco()>100) ) break;
//    }

//  turn_B_Dis();
//  FillDirect();

//lineToRight();


//    lineWalking();
//   Stop();
//RUN(100,100);
}

