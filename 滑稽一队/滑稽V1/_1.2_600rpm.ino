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

//#include <MagneticSensorLsm303.h>

void wait(double sec)
{
  delay(sec);
}
void SetMoto(int side, int v){ //0=L 1=R
     if(side && v>=0){     //right side
          digitalWrite(M2,HIGH);
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
void RUN( int LeftV, int RightV ){ //v= 0~255
     SetMoto(0, LeftV);
     SetMoto(1, RightV);
}//V 0~255
void RUNB( int LeftV, int RightV ){ //v= 0~255
     SetMoto(0, -LeftV);
     SetMoto(1, -RightV);
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
void lineWalking()/**Forward line-tracking **/
{
    //FlagFB = 0;
    if (colorReco(F5) == WHITE || colorReco(F4) == WHITE) RUN(250, 250);

    else if (colorReco(F6) == WHITE) RUN(-100, 250);
    else if (colorReco(F3) == WHITE) RUN(250, -100);

    else if (colorReco(F7) == WHITE) { RUN(-250, 250); wait(50); }
    else if (colorReco(F2) == WHITE) { RUN(250, -250); wait(50); }
    else if (colorReco(F8) == WHITE) { RUN(-250, 250); wait(50); }
    else if (colorReco(F1) == WHITE) { RUN(250, -250); wait(50); }
    else if (colorReco(F5) == WHITE&&colorReco(F4) == WHITE) RUN(-50, 250);
    else if (colorReco(F3) == WHITE&&colorReco(F6) == WHITE) RUN(250, -50);
    else if (colorReco(F6) == WHITE&&colorReco(F3) == WHITE) RUN(-50, 250);
    else if (colorReco(F2) == WHITE&&colorReco(F7) == WHITE) RUN(250, -50);
    else RUN(250,250);
}
void lineWalkingRev()/**reverse line-tracking**/
{
    //FlagFB = 0;
    if (colorReco(G5) == WHITE || colorReco(G4) == WHITE) RUNB(250, 250);
    else if (colorReco(G6) == WHITE) RUNB(-100, 250);
    else if (colorReco(G3) == WHITE) RUNB(250, -100);
    else if (colorReco(G7) == WHITE) { RUNB(-250, 250); wait(50); }
    else if (colorReco(G2) == WHITE) { RUNB(250, -250); wait(50); }
    else if (colorReco(G8) == WHITE) { RUNB(-250, 250); wait(50); }
    else if (colorReco(G1) == WHITE) { RUNB(250, -250); wait(50); }
    else if (colorReco(G5) == WHITE&&colorReco(G4) == WHITE) RUNB(-50, 250);
    else if (colorReco(G3) == WHITE&&colorReco(G6) == WHITE) RUNB(250, -50);
    else if (colorReco(G6) == WHITE&&colorReco(G3) == WHITE) RUNB(-50, 250);
    else if (colorReco(G2) == WHITE&&colorReco(G7) == WHITE) RUNB(250, -50);
    else RUNB(250,250);
}
void Line_To_Left()/**line-tracking stop at the left corner**/
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
void Line_To_Right()/**line-tracking stop at the right corner**/
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
void Turn_L()
{
    RUN(-240,240);
    wait(300);
    while(1){
      RUN(-240,240);
      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
    }
}
void Turn_R()
{
    RUN(240,-240);
    wait(300);
    while(1){
      RUN(240,-240);
      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
    }
}
void Turn_B()/**180°转**/
{
    RUN(-240,240);
    wait(300);
    while(1){
      RUN(-240,240);
      if(colorReco(F4)==WHITE || colorReco(F5)==WHITE) break;
    }
}
void Turn_LB()/**反向左转**/
{
    RUNB(-240,240);
    wait(300);
    while(1){
      RUNB(-240,240);
      if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
    }
}
void Turn_RB()/**反向右转**/
{
    RUNB(240,-240);
    wait(300);
    while(1){
      RUNB(240,-240);
      if(colorReco(G4)==WHITE || colorReco(G5)==WHITE) break;
    }
}
void Circle()
{
   Line_To_Left();
   Turn_L();
}
void setup() 
{
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    pinMode(FPin,INPUT);
    pinMode(GPin,INPUT);
    //Serial.begin(9600); 
    //pinMode(FPin,INPUT);
} 
void loop() 
{  
/**无平台路线**/

//starttime = millis();
//while (millis() - starttime < 3000)
//{
//    lineWalking();
//}
//Turn_B();

//Circle(){
//   Line_To_Left();
//   Turn_L();
//}

//Line_To_Left();
RUN(240,240);
//wait(500);
//Line_To_Left();
//Turn_L_Light();
//Line_To_Left();
//Turn_R_Light();
//Line_To_Left();
//Turn_R_Light();
//Line_To_Left();
//Turn_R_Light();
//Line_To_Left();
//RUN(240,240);
//wait(100);
//Line_To_Left();
//Stop();

//  /**定时器使用**/
//  unsigned long starttime;
//  //void loop()
//  starttime = millis();
//  while (millis() - starttime < 5000)
//  {
//    lineWalking();
//  }
//  RUN(240,240);
//  wait(100);
//  Turn_B();
//  starttime = millis();
//  while (millis() - starttime < 2000)
//  {
//    lineWalking();
//  }
//  for(int i =1 ; i<6;i++){
//    RUN(240,240);
//    wait(550);
//    RUN(-100,250);
//    wait(30);
//  }
//  Stop();
}

