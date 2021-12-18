#include<nRF24L01.h>
#include<Wire.h>
#include<RF24.h>
#include<SPI.h>

RF24 rf24(9,10); // CE, CSN
const int channel = 115;
const byte addr[2][6] = {"NOVA1","NOVA2"}; //handle: NOVA1; vehicle: NOVA2

int i,encodervalue;
int command[2];  //the array of command from transmitter
int distance;
double info[3]; //info[0]: run/stop status; info[1]: speed; info[2]: distance

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  rf24.begin();  //initial setting for nRF24L01
  rf24.setChannel(channel);       
  rf24.openWritingPipe(addr[1]); 
  rf24.openReadingPipe(1,addr[0]);
  rf24.setPALevel(RF24_PA_MAX);   
  rf24.setDataRate(RF24_2MBPS);       
}

void motor_straight()  //funtion to let the vehicle go forward and backward
{
  Wire.beginTransmission(42);
  if (command[1]<0) Wire.write("darrrr");
  else Wire.write("daffff");
  Wire.endTransmission();
  Wire.beginTransmission(42);
  Wire.write("sa");
  for (i=0;i<=3;i++)
  {
    Wire.write(abs(command[1]));
    Wire.write(0);
  }
  Wire.endTransmission();
}

void motor_turn()  //function to let the vehicle turn left and right
{
  Wire.beginTransmission(42); 
  if (command[1]<0) Wire.write("darrff"); 
  else Wire.write("daffrr");
  Wire.endTransmission();
  Wire.beginTransmission(42);
  Wire.write("sa");
  for(int i=0;i<=3;i++)
  {
    Wire.write(abs(command[1]));
    Wire.write(0);
  }
  Wire.endTransmission();
}

void read_encoder()  //funtion that read encoder
{
  Wire.beginTransmission(42);
  Wire.write("i");
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(42,4);
  delay(10);
  encodervalue = (long int) Wire.read();
  encodervalue += ((long int) Wire.read() <<8);
  encodervalue += ((long int) Wire.read() <<16);
  encodervalue += ((long int) Wire.read() <<24);
}

void stopmotor()  //stop motor command
{
  Wire.beginTransmission(42);
  Wire.write("ha");
  Wire.endTransmission();
}

void loop() 
{
  rf24.startListening();
  if (rf24.available())   //get the commands from transmitter
  {
    rf24.read(&command, sizeof(command));
  }
  if (command[1]!=0&&command[0]!=3)
  {
    if (command[0]==0||command[0]==1) 
    {
      motor_straight();  //execute command
      read_encoder();  
      distance=encodervalue-0x80000000;
      info[2]=double(distance)/232.0*0.246;
    }
    if (command[0]==2) motor_turn();
    info[0]=1; info[1]=abs(command[1]);  //prepare for the information back
  }
  else 
  {
    info[0]=0; info[1]=0; stopmotor();
  }
  delay(25);
  rf24.stopListening();
  rf24.write(&info,sizeof(info));  //transmit information array
  delay(25);
}
