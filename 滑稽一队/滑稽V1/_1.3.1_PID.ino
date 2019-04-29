#include <PID_v1.h>
int E1 = 6, M1 = 8, E2 = 7, M2 =9;/**Moto IO**/
double Setpoint , Input, Output;/**Define Variables we'll be connecting to**/

const byte encoder0pinLA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinLB = 44;//B pin -> the digital pin 4
const byte encoder0pinRA = 3;//A pin -> the interrupt pin 0
const byte encoder0pinRB = 42;//B pin -> the digital pin 4

byte encoder0PinALast_left;
byte encoder0PinALast_right;
int duration_left;//the number of the pulses
boolean Direction_left;//the rotation direction 
int duration_right;//the number of the pulses
boolean Direction_right;//the rotation direction 
int flag;
double aggKp=20, aggKi=0, aggKd=0;
double consKp=20, consKi=0, consKd=0;
//0.264<p<0.308
//PID myPID(&Input, &Output, &Setpoint,0.28,16,0, DIRECT);
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
//PID myPID(&Input, &Output, &Setpoint,0.05,50,0, DIRECT);

void wait(double sec)
{
  delay(sec);
}
void SetMoto(int side, int v){ //0=L 1=R
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
void RUN( int LeftV, int RightV ){ //v= 0~255
     SetMoto(0, LeftV);
     SetMoto(1, RightV);
}//V 0~255
void RUNB( int LeftV, int RightV ){ //v= 0~255
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
void EncoderInit()
{
  Direction_left = true;//default -> Forward  
  Direction_right = true;//default -> Forward  
  pinMode(encoder0pinLB,INPUT);
  pinMode(encoder0pinRB,INPUT);   

  pinMode(encoder0pinLA,INPUT);
  pinMode(encoder0pinRA,INPUT);   
  
  attachInterrupt(0, wheelSpeed_left, CHANGE);
  attachInterrupt(1, wheelSpeed_right, CHANGE);
}
void setup() 
{
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    pinMode(E1, OUTPUT);   
    pinMode(E2, OUTPUT); 

    Serial.begin(9600);
    EncoderInit();//Initialize the module
    //initialize the variables we're linked to
    
    //turn the PID on
    myPID.SetMode(AUTOMATIC);
    Setpoint = 6;
    //analogWrite(7,70);
    //RUN(70,70);
    flag = 1;
} 
void loop() 
{  
//    //Input = analogRead(0);
//    if(flag == 1){
//      delay(4000);
//      flag = ~flag;
//      duration_left = 0;
//      duration_right = 0;
//      delay(200);
////      Serial.print("L:");
////      Serial.println(duration_left/(11*20.4)*30);
////      Serial.print("R:");
////      Serial.println(duration_right/(11*20.4)*30);
//    }
    duration_left = 0;
    duration_right = 0; 
    
    Input = -duration_left/(11*20.4)*30;
    //Setpoint = duration_right/(11*20.4)*30; //keep output ~ 100
    double gap = abs(Setpoint-Input);
    if(gap<1)
    {   
        //we're close to setpoint, use conservative tuning parameters
        myPID.SetTunings(consKp, consKi, consKd);
    }
    else
    {
        //we're far from setpoint, use aggressive tuning parameters
        myPID.SetTunings(aggKp, aggKi, aggKd);
    }
    
    myPID.Compute();
    analogWrite(7,Output);

    Serial.print("Out:");
    Serial.println(Output);
    Serial.print("L:");
    Serial.println(duration_left/(11*20.4)*30);
//    Serial.print("R:");
//    Serial.println(duration_right/(11*20.4)*30);

}
void wheelSpeed_left()
{
  int Lstate = digitalRead(encoder0pinLA);
  if((encoder0PinALast_left == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinLB);
    if(val == LOW && Direction_left)
    {
      Direction_left = false; //Reverse
    }
    else if(val == HIGH && !Direction_left)
    {
      Direction_left = true;  //Forward
    }
  }
  encoder0PinALast_left = Lstate;
 
  if(!Direction_left)  duration_left++;
  else  duration_left--;
}
void wheelSpeed_right()
{
  int Lstate = digitalRead(encoder0pinRA);
  if((encoder0PinALast_right == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinRB);
    if(val == LOW && Direction_right)
    {
      Direction_right = false; //Reverse
    }
    else if(val == HIGH && !Direction_right)
    {
      Direction_right = true;  //Forward
    }
  }
  encoder0PinALast_right = Lstate;
 
  if(!Direction_right)  duration_right++;
  else  duration_right--;
}
