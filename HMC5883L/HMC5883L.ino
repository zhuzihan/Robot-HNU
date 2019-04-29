#include <Wire.h> //I2C Arduino Library

#define address 0x1E //001 1110b(0x3C>>1), I2C 7bit address of HMC5883
#define MagnetcDeclination 4.43 //Shanghai
#define CalThreshold 0

int offsetX,offsetY,offsetZ;

void setup()
{
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
  
 // calibrateMag();
}
void loop()
{
  int x,y,z; //triple axis data
  getRawData(&x,&y,&z);
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.print(z);
  Serial.print(" angle(x,y): ");
  Serial.println(calculateHeading(&x,&y,&z));

  delay(250);
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
  float headingRadians = atan2((double)((*y)-(-165)),(double)((*x)-22));
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

