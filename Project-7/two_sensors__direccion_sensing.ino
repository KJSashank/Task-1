//code rearranged by Javier Muñoz 10/11/2016 ask me at javimusama@hotmail.com
#include <SoftwareSerial.h>

#define ARDUINO_RX 5//should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 6//connect to RX of the module

#define trigPin 13//for the FIRST distance module 
#define echoPin 12 
#define trigPin2 10//for the SECOND  distance module 
#define echoPin2 9

#define trigPin 13//for the distance module
#define echoPin 12


SoftwareSerial mySerial(ARDUINO_RX, ARDUINO_TX);//init the serial protocol, tell to myserial wich pins are TX and RX

////////////////////////////////////////////////////////////////////////////////////
//all the commands needed in the datasheet(http://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf)
static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string
                                 //0X7E FF 06 command 00 00 00 EF;(if command =01 next song order) 
#define NEXT_SONG 0X01 
#define PREV_SONG 0X02 

#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song)

#define VOLUME_UP_ONE 0X04
#define VOLUME_DOWN_ONE 0X05
#define CMD_SET_VOLUME 0X06//DATA IS REQUIRED (number of volume from 0 up to 30(0x1E))
#define SET_DAC 0X17
#define CMD_PLAY_WITHVOLUME 0X22 //data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song)

#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED
                #define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED
                
#define SLEEP_MODE_START 0X0A
#define SLEEP_MODE_WAKEUP 0X0B

#define CMD_RESET 0X0C//CHIP RESET
#define CMD_PLAY 0X0D //RESUME PLAYBACK
#define CMD_PAUSE 0X0E //PLAYBACK IS PAUSED

#define CMD_PLAY_WITHFOLDER 0X0F//DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3

#define STOP_PLAY 0X16

#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care)

#define SET_CYCLEPLAY 0X19//data is needed 00 start; 01 close

#define SET_DAC 0X17//data is needed 00 start DAC OUTPUT;01 DAC no output
////////////////////////////////////////////////////////////////////////////////////
int firstTime=0;//we need to declare firstTime outside the loop 
long Distance,auxDistance,gap=0;
long Distance2,auxDistance2,gap2=0;

void setup()
{
  Serial.begin(9600);//Start our Serial coms for serial monitor in our pc
mySerial.begin(9600);//Start our Serial coms for THE MP3
delay(500);//Wait chip initialization is complete
   sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
delay(200);//wait for 200ms
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
pinMode(trigPin2, OUTPUT); 
pinMode(echoPin2, INPUT); 

}

void loop() 
{ 


Distance=measureDistance(trigPin,echoPin);//measure distance1 and store
Distance2=measureDistance(trigPin2,echoPin2);//measure distance2 and store
gap=abs(Distance-auxDistance);// calculate the difference between now and last reading
gap2=abs(Distance2-auxDistance2);// calculate the difference between now and last reading

if(firstTime==0){//necesary for stability things
auxDistance=Distance;
auxDistance2=Distance2;
gap=0;
gap2=0;
//does it only the first time after play a song to avoid first loop malfuntcion
firstTime++; 
delay(2000);
}


 if(gap>20 and gap2<20 ){ //if distance variation is 20cm
sendCommand(CMD_PLAY_WITHFOLDER, 0X0201);//play the first song of the second folder 
firstTime=0;//avoid errors!!we dont like errors
Serial.println("RIGHT MOVEMENT DETECTED");
delay(2000); 
} 
 if(gap2>20 and gap<20){ //if distance variation is 20cm
sendCommand(CMD_PLAY_WITHFOLDER, 0X0202);//play the second song of the second folder 
firstTime=0;//avoid errors!!we dont like errors
Serial.println("LEFT MOVEMENT DETECTED");
delay(2000); 
} 
Serial.println("\\\\\\\\\\\\\\\\\\\\\\");//debugggggg
Serial.print(" New Distace:");//debugggggg
Serial.print(Distance);
Serial.print("   Old Distance: ");
Serial.print(auxDistance);
Serial.print("   GAP ");
Serial.println(gap);

Serial.print("New Distace2:");//debugggggg
Serial.print(Distance);
Serial.print("  Old Distance2: ");
Serial.print(auxDistance);
Serial.print("  GAP2 ");
Serial.println(gap);
Serial.println("\\\\\\\\\\\\\\\\\\\\\\");//debugggggg

delay(300); 
auxDistance=Distance;//store the value for the if() in the next loop
auxDistance2=Distance2;//store the value for the if() in the next loop

}


 

void sendCommand(int8_t command, int16_t dat)
{
     if (command==CMD_PLAY_WITHFOLDER or command==CMD_PLAY_WITHVOLUME){Serial.print("PLAYING SONG, SLIGHTLY MOVE YOUR HEAD FOR GREAT EFFECT");}
 delay(20);
 Send_buf[0] = 0x7e; //starting byte
 Send_buf[1] = 0xff; //version
 Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
 Send_buf[3] = command; //
 Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
 Send_buf[5] = (int8_t)(dat >> 8);//datah
 Send_buf[6] = (int8_t)(dat); //datal
 Send_buf[7] = 0xef; //ending byte
 for(uint8_t i=0; i<8; i++)//
 {
   mySerial.write(Send_buf[i]) ;//send bit to serial mp3

 }
 Serial.println();
}

long measureDistance(int trigger,int echo){
   long duration, distance;
  
  digitalWrite(trigger, LOW);  //PULSE ___|---|___
  delayMicroseconds(2); 
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger, LOW);
  
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
//   Serial.println("distance:");
//   Serial.println(distance);
  return distance;

}
