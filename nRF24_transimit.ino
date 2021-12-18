#include<nRF24L01.h>
#include<RF24.h>
#include<SPI.h>
#include<LiquidCrystal.h>

RF24 rf24(9,10); // CE, CSN
LiquidCrystal lcd(7,8,3,4,5,6);
const int channel = 115;
const int xst = A0, yst = A1;
const byte addr[2][6] = {"NOVA1","NOVA2"};
const char display[2][5]={"Stop", "Run"};

int num=0;
int msg[2];  //[0]status: 0:forward; 1:backward; 2:turn; 3:stop; [1]speed
double info[3];  //message receive array
int posXvalue, posYvalue;

void setup() 
{
  Serial.begin(9600);
  lcd.begin(2,16);  //setup for LCD
  rf24.begin();  //basic setup for nRF24L01
  rf24.setChannel(channel);       
  rf24.openWritingPipe(addr[0]); 
  rf24.openReadingPipe(1,addr[1]);
  rf24.setPALevel(RF24_PA_MAX);   
  rf24.setDataRate(RF24_2MBPS);      
}

void LCD_print()  //function of displaying the message on the LCD
{
  num++;
  if (num%4!=0) return;
  lcd.clear();
  lcd.print("Distance:"); lcd.print(info[2]); lcd.print("m");
  lcd.setCursor(0,1);
  lcd.print("Speed: "); lcd.print(abs(info[1])); 
  lcd.print(" "); lcd.print(display[int(info[0])]);
}

void loop() 
{
  posXvalue=analogRead(xst);  //533 mid
  posYvalue=analogRead(yst);  //490 mid
  rf24.stopListening();
  if (abs(533-posXvalue)>abs(490-posYvalue))
  {
    if (posXvalue<533) msg[1]=map(posXvalue,0,528,40,0); //forward
    if (posXvalue>533) msg[1]=map(posXvalue,538,1023,0,-40);  //backward
    if (msg[1]>0) msg[0]=0;
    else msg[0]=1;
  }
  if (abs(533-posXvalue)<abs(490-posYvalue))
  {
    if (posYvalue>490) msg[1]=map(posYvalue,495,1023,-40,-60); //turn right
    if (posYvalue<490) msg[1]=map(posYvalue,0,485,60,40);  //turn left
    msg[0]=2;
  }
  if ((posXvalue==533&&posYvalue==490)||msg[1]==0) 
  {
    msg[0]=3; msg[1]=0;  //stop
  }
  rf24.stopListening();
  rf24.write(&msg,sizeof(msg));  //sent command
  delay(25);
  rf24.startListening(); //receive message
  if (rf24.available()) 
  {
    rf24.read(&info,sizeof(info));
  }
  LCD_print();  //print data
  delay(25);
}
