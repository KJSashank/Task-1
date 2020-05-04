#include <DFRobot_SSD1306_I2C.h>
#include <DFRobot_PAJ7620U2.h>
#include <EEPROM.h>  
#include <stdlib.h>
#include <DFRobot_IRremote.h>


#define EEPROM_HAVE_LEARNED 0   //the judge part address in EEPROM
#define EEPROM_UP_INDEX   4    //the up guesture part address in EEPROM
#define EEPROM_DOWN_INDEX   8 //the down guesture part address in EEPROM
#define EEPROM_RIGHT_INDEX   12 //the right guesture part address in EEPROM
#define EEPROM_LEFT_INDEX   16  //the left guesture part address in EEPROM
#define EEPROM_FORWARD_INDEX   20 //the forward guesture part address in EEPROM
#define EEPROM_BACKWARD_INDEX   24  //the backward guesture part address in EEPROM
#define EEPROM_CLOCKWISE_INDEX   28 //the clockwise guesture part address in EEPROM
#define EEPROM_ANTICLOCKWISE_INDEX   32 //the antiwise guesture part address in EEPROM
#define EEPROM_WAVE_INDEX   36  //the wave guesture part address in EEPROM


#define interval   400  //the submit time between two gesture
/*
   This table is describe the date of address and bgesture
     * ----------------------------------------------------------------
     * |    gesture   |   addr    |  gesture number | code(4 bites)|    
     * ----------------------------------------------------------------
     * |    right     |   12-15   |        1        | constumed    | 
     * ----------------------------------------------------------------
     * |    left      |   16-19   |        2        | constumed    | 
     * ----------------------------------------------------------------
   * |      up      |   4-7     |        4        | constumed    | 
     * ----------------------------------------------------------------
   * |     down     |   8-11    |        8        | constumed    | 
   * ----------------------------------------------------------------
     * |   forward    |   20-23   |       16        | constumed    | 
     * ----------------------------------------------------------------
     * |   backward   |   24-27   |       32        | constumed    | 
     * ----------------------------------------------------------------
     * |  clockwise   |   28-31   |       64        | constumed    | 
     * ----------------------------------------------------------------
     * |  anti-wise   |   32-36   |       128       | constumed    | 
     * ----------------------------------------------------------------
     *
  */
DFRobot_PAJ7620U2 paj;
uint32_t codeArry[8][3]={{paj.eGestureRight,  0x32A68877/*right+value=rightmove*/,  EEPROM_RIGHT_INDEX},
              {paj.eGestureLeft,  0x32A608F7/*leftt+value=leftmove*/,  EEPROM_LEFT_INDEX},
              {paj.eGestureUp,  0x32A6D02F/*up+value=upmove*/,  EEPROM_UP_INDEX},
              {paj.eGestureDown,  0x32A6708F/*down+value=downmove*/,  EEPROM_DOWN_INDEX},
              {paj.eGestureForward,0x32A6B04F/*forward+value=sure*/,EEPROM_FORWARD_INDEX},
              {paj.eGestureBackward,0x32A6B847/*backward+value=back*/,EEPROM_BACKWARD_INDEX},
              {paj.eGestureClockwise,0x32A6A857/*clockwise+value=voiceAdd*/,EEPROM_CLOCKWISE_INDEX},
              {paj.eGestureAntiClockwise,0x32A638C7/*antiwise+value=voiceMinus*/,EEPROM_ANTICLOCKWISE_INDEX}};

uint32_t timestamp=0,ret;/*to record the time of each step*/
String code;
IRremote_Send remoteSend;
DFRobot_SSD1306_I2C oled12864;
IRremote_Receive remoteReceive_2;
uint16_t gestureSupported =   paj.eGestureRight+    paj.eGestureLeft+       paj.eGestureUp+           paj.eGestureDown+
                paj.eGestureForward+  paj.eGestureBackward+   paj.eGestureClockwise+    paj.eGestureAntiClockwise;
void wave();
void clockwise();
void antiwise();
void forward();
void backward();
void right();
void left();
void down();
void up();
void cleardata();
uint32_t transfer( char * v);
void writeEEPROM(int index,uint32_t value);
void loadEEPROM(void);
uint32_t readEEPROM(int index);

void learnRemote(DFRobot_PAJ7620U2::eGesture_t ges);

uint32_t timeInterval;/*this means that the time between now from last step*/


void setup()
{
  Serial.begin(9600);
  remoteSend.begin(10);
  oled12864.begin(0x3c);
  remoteReceive_2.begin(0);
  delay(300);
  Serial.println("Gesture recognition system base on PAJ7620U2");
  while(paj.begin() != 0){
    Serial.println("initial PAJ7620U2 failure! Please check if all the connections are fine, or if the wire sequence is correct?");
    delay(500);
  }
  Serial.println("PAJ7620U2 init completed, start to test the gesture recognition function");
  /*to judge is there any data in EEPROM*/
  /*to register new data to the EEPROM and Delect the before date*/

  
  /*Set fast detection mode 
   *If the parameter is set to false, the module enters slow detection mode, and it detects one gesture every 2s. We have integrated
   *some gestures inside the module to make it convenient for beginners.
   *The slow mode can recognize 9  basic gestures and 4 expanded gestures: move left, right, up, down, forward, backward, clockwise, 
   *counter-clockwise, wave, slowly move left and right, slowly move up and down, slowly move forward and backward, 
   *wave slowly and randomly.
   *
   *
   *
   *If the parameter is set to true, the module enters fast detection mode. 
   *The fast mode can recognize 9 gestures: move left, right, up, down, forward, backward, clockwise, counter-clockwise, wave
   *To detect the combination of these gestures, like wave left, right and left quickly, users needs to design their own algorithms logic.
   *Since users only use limited gestures in this mode, we are not going to integrate too much expanded gestures in the library. 
   *If necessary, you can complete the algorithm logic in the ino file by yourself.
   */
  paj.setGestureHighRate(true);
  loadEEPROM();//to download the data if it is necessary after some judge procedure
}

void loop()
{
  /* Read gesture number（return eGesture_t enumerated type）
   * eGestureNone  eGestureRight  eGestureLeft  eGestureUp  eGestureDown  eGestureForward
   * eGestureBackward  eGestureClockwise  eGestureAntiClockwise  eGestureWave  
   */
  digitalWrite(13,LOW);
  timeInterval=millis()-timestamp;
  DFRobot_PAJ7620U2::eGesture_t gesture = paj.getGesture();
  if(gesture == paj.eGestureNone){
    return;
  }
  if(gesture==paj.eGestureWave){
    Serial.println("Wave begin");
    cleardata();
    remoteReceive_2.getIrCommand();
    learnRemote(paj.eGestureUp);
    learnRemote(paj.eGestureDown);
    learnRemote(paj.eGestureRight);
    learnRemote(paj.eGestureLeft);
    learnRemote(paj.eGestureForward);
    learnRemote(paj.eGestureBackward);
    learnRemote(paj.eGestureClockwise);
    learnRemote(paj.eGestureAntiClockwise);
    writeEEPROM(0,0xDFDFDFDF);
    loadEEPROM();
  } 
  while(timeInterval>10000){////这里要重新获取一下手势，否则退出不去
    DFRobot_PAJ7620U2::eGesture_t gesture = paj.getGesture();
    if(gesture==paj.eGestureForward){
       Serial.println("forward");
       digitalWrite(13,HIGH);
       delay(1000);//灯亮一秒，唤醒一秒后才开始工作
       timestamp=millis();
       timeInterval = millis()- timestamp;
       delay(interval);
       return;//这里break可以去掉，换成return
     }
    }
  if(gesture & gestureSupported ){
   /* Get the string descritpion corresponding to the gesture number.
    * The string description could be 
    * "None","Right","Left", "Up", "Down", "Forward", "Backward", "Clockwise", "Anti-Clockwise", "Wave",
    */
    //Serial.print("timeInterval=");Serial.println(timeInterval);
    String description  = paj.gestureDescription(gesture);//Convert gesture number into string description
    digitalWrite(13,HIGH);
    for(int i=0;i<9;i++){
        if(codeArry[i][0]==gesture){
        Serial.print("code  = ");Serial.println(codeArry[i][1],HEX);
        Serial.print("gesture  = ");Serial.println(description);
        remoteSend.sendNEC(codeArry[i][1],32);
          //Serial.println("111");
          oled12864.setCursorLine(1);
          //oled12864.setCursor(20,1);
          //Serial.println("222");
          //oled12864.printLine("hello");
          oled12864.printLine(description);
           //Serial.println("333");
          delay(interval);
          timestamp=millis();
          oled12864.fillScreen(0);
          break;
      }
    }
  }
}

uint32_t transfer( char * v)
{
  char *ptr;
  ret = strtoul(v,&ptr,16);
  delay(100);
  return ret;
  }

/*clean the EEPROM*/
void cleardata()
{
    for(int i=0;i<512;i++)
    {
        EEPROM.write(i,0);
    }  
}

  /**
   * @brief to read the data in EEPROM according index
   * @return the data of return is take up 4 bite; 
   * @param the "index" is defined in the head of the article;
   */
uint32_t readEEPROM(int index)
{
  uint32_t ret = 0;
  for(int i=0;i<4;i++){
    ret = (ret<<8) + EEPROM.read(index+3-i);}
  return ret;
}

/**
   * @brief to write the data which is download in IRreceive  
   * @return the data of return is take up 4 bite; 
   * @param the "index" is defined in the head of the article;
   * @param the "value" is download from IRreceive;
   */
void writeEEPROM(int index,uint32_t value)
{
  for(int i=0;i<4;i++){
    EEPROM.write(index+i,value&0xff);
    value>>=8;
  }
}

/**
   * @brief to save the data which is from writeEEPROM in the oldArry  
   * @return the data of return is take up 4 bite; 
   * @param the "index" is defined in the head of the article;
   * @param the "value" is download from IRreceive;
   */
// void saveEEPROM(void)
// {
//   writeEEPROM(0,0);
//   for(int i=0;i<sizeof(codeArry)/sizeof(codeArry[0]);i++){
//     writeEEPROM(codeArry[i][2],codeArry[i][1]);
//   }
//   writeEEPROM(0,0xDFDFDFDF);
// }

/**
   * @brief to judge whether the date in EEPROM is needed;  
   * if the date is needed,which will write in CodeArry;
   */
void loadEEPROM(void)
{
  uint32_t flag = readEEPROM(0);
  if(flag != 0xDFDFDFDF)
    return;
  else{
  for(int i=0;i<sizeof(codeArry)/sizeof(codeArry[0]);i++){
    codeArry[i][1] = readEEPROM(codeArry[i][2]);
  }}
}

void learnRemote(DFRobot_PAJ7620U2::eGesture_t ges)
{
  oled12864.setCursorLine(1);
  oled12864.printLine("set "+paj.gestureDescription(ges));
  oled12864.setCursorLine(2);
  oled12864.printLine("gesture:");
  do {
    code = (remoteReceive_2.getIrCommand());
    if(code != "0"){
    Serial.print("code=");Serial.println(code);}
  }while ((!(!(code == "0"))));
  char  a[8]={0};
  strcpy(a,code.c_str());
  uint32_t temp=transfer(a);
  for(int i=0;i<8;i++){
    if(ges==codeArry[i][0]){
      writeEEPROM(codeArry[i][2],temp);
      break;
      }
    }
  oled12864.setCursorLine(3);
  oled12864.printLine(code);
  delay(2000);
  oled12864.fillScreen(0);
  delay(300);
}
