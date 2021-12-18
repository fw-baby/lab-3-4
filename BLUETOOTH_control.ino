#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(3,2);

char tmp;//basic definition
int i;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("ok_BT");
  BT.begin(9600);
}

void SPEED(int a) //speed setting
{
  Wire.beginTransmission(42);
  Wire.write("sa");
  for (i=0;i<=3;i++)
  {
    Wire.write(a); Wire.write(0);
  }
  Wire.endTransmission();
}

void STOP() //stop motor
{
  Wire.beginTransmission(42);
  Wire.write("ha");
  Wire.endTransmission();
}

void loop()
{
  if (Serial.available()) //AT
  {
     tmp = Serial.read();
     BT.print(tmp);
  }
  if (BT.available())
  {
  tmp = BT.read();
  
  switch(tmp) //set command to control through smartphone
  {
   case 'S': //stop motor
  {
   STOP();
   break;
 } 
  case 'F': //Forward
  {
    Wire.beginTransmission(42);
    Wire.write("daffff");
    Wire.endTransmission();
    Wire.beginTransmission(42);
    Wire.write(70);Wire.write(0);
    Wire.write(70);Wire.write(0);
    Wire.write(70);Wire.write(0);
    Wire.write(70);Wire.write(0);
    Wire.endTransmission();
    break;
  }
  case 'B': //Backward
  {
   Wire.beginTransmission(42);
   Wire.write("darrrr");
   Wire.endTransmission();
   Wire.beginTransmission(42);
   Wire.write(70);Wire.write(0);
   Wire.write(70);Wire.write(0);
   Wire.write(70);Wire.write(0);
   Wire.write(70);Wire.write(0);
   Wire.endTransmission();
   break;
  }
  case 'R': //Right
 {
   Wire.beginTransmission(42);
   Wire.write("daffrr");
   Wire.endTransmission();
   SPEED(70);
   STOP();
   break;
 }
  case 'L': //Left
 {
   Wire.beginTransmission(42);
   Wire.write("darrff");
   Wire.endTransmission();
   SPEED(70);
   STOP();
   break;
 }
}}}
