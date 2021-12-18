#include <Wire.h>

const int pinri = 9, pinle = 10;//basic set and definition
const int pinILd = 6, pinIRd = 7;
const int pinILa = A1, pinIRa = A0;
int IR_A1le = 0, IR_A1ri = 0;
int IR_D1ri = 0,IR_D1le = 0;

void setup()//define pin for input and output
{
  Serial.begin(9600);
  pinMode(pinle,OUTPUT);
  pinMode(pinri,OUTPUT);
  pinMode(pinILd,INPUT);
  pinMode(pinIRd,INPUT);
  pinMode(pinILa,INPUT);
  pinMode(pinIRa,INPUT);
  Wire.begin();
}

void loop()
{
  Serial.print("right:");            //serial monitor
  Serial.println(IR_D1ri);
  Serial.print("left:");
  Serial.println(IR_D1le);
  
  IR_A1ri = analogRead(pinIRa);
  IR_D1ri = digitalRead(pinIRd);
  IR_A1le = analogRead(pinILa);
  IR_D1le = digitalRead(pinILd);

  if((IR_D1le = 0)&&(IR_D1ri = 0))//forward
  {
    Wire.beginTransmission(42);
    Wire.write("baffff");
    Wire.endTransmission();
    Wire.beginTransmission(42);
    Wire.write(17);Wire.write(0);
    Wire.write(17);Wire.write(0);
    Wire.write(17);Wire.write(0);
    Wire.write(17);Wire.write(0);
    Wire.endTransmission();
  }
  
   else if((IR_D1le = 1)&&(IR_D1ri = 1) )//stop
  {
    Wire.beginTransmission(42);
    Wire.write("ha");
    Wire.endTransmission();
  }
  
else if((IR_D1le = 0)&&(IR_D1ri = 1))//right
  {
   Wire.beginTransmission(42);
   Wire.write("baffrr");
   Wire.endTransmission();
   Wire.beginTransmission(42);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
  Wire.endTransmission();
  }
  
  else if((IR_D1le = 1)&&(IR_D1ri = 0))//left
  {
   Wire.beginTransmission(42);
   Wire.write("barrff");
   Wire.endTransmission();
   Wire.beginTransmission(42);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
   Wire.write(99);Wire.write(0);
  Wire.endTransmission();
  }
}
