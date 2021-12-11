#include<Wire.h>
#include<MsTimer2.h>

double P,I,D,Kp,Ki,Kd;
double OriginalDis, Dis, value;
int Speed, Ri, Le;

void setup() 
{
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  Wire.begin();
  Kp=1; Ki=0; Kd=0;
  MsTimer2::set(5,MsTimer2ISR);
  MsTimer2::start();
}

void MsTimer2ISR()
{
  Le=analogRead(A0);
  Ri=analogRead(A1);
  Dis=double(Le*-30+Ri*30)/(Le*Ri);
  P=Kp*Dis;
  I=Ki*(OriginalDis+Dis)*5/2;
  D=Kd*(Dis-OriginalDis)/5;
  value=P+I+D;
  Speed=map(value,0,100,10,30);
  if (value>2)
  {
    Wire.beginTransmission(42);
    Wire.write("daffrr");
    Wire.endTransmission();
  }
  if (value<-2)
  {
    Wire.beginTransmission(42);
    Wire.write("darrff");
    Wire.endTransmission();
  }
  if (-2<value&&value<=4)
  {
    Wire.beginTransmission(42);
    Wire.write("daffff");
    Wire.endTransmission();
  }
  Wire.beginTransmission(42);
  Wire.write("sa");
  for (int i=0;i<=3;i++)
  {
    Wire.write(Speed);
    Wire.write(0);
  }
  Wire.endTransmission();
  OriginalDis=Dis;
}

void loop() 
{
  
}
