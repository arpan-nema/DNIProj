// Designing Novel Interactions (INFO90003_2021_SM1)
// Assignment 2
// Group Members- Arpan Nema, Saman Akbaryan
// Date- 10 June 2021

//****************************************Thank you**************************************
//Santos, R. (2016). Guide for 8x8 Dot Matrix MAX7219 with Arduino | Random Nerd Tutorials. Randomnerdtutorials.com. Retrieved 10 June 2021, 
//from https://randomnerdtutorials.com/guide-for-8x8-dot-matrix-max7219-with-arduino-pong-game/.

//Santos, S. (2017). Guide to SD Card Module with Arduino | Random Nerd Tutorials. Randomnerdtutorials.com. Retrieved 10 June 2021,
//from https://randomnerdtutorials.com/guide-to-sd-card-module-with-arduino/.

//Schoeffler, M. (2017). Tutorial: How to use the GY-521 module (MPU-6050 breakout board) with the Arduino Uno. Michael Schoeffler. Retrieved 10 June 2021,
//from https://mschoeffler.com/2017/10/05/tutorial-how-to-use-the-gy-521-module-mpu-6050-breakout-board-with-the-arduino-uno/.
//*******************************************End*****************************************

// Libraries
#include <LedControl.h>
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoQueue.h>

//Declaring the global variables
const int MPU_ADDR=0x68;
int16_t accelerometer_x,accelerometer_y,accelerometer_z;
int16_t old_accelerometer_x,old_accelerometer_y,old_accelerometer_z;
int16_t gyro_x,gyro_y,gyro_z;
int16_t temperature;
char tmp_str[7];
int flag=0;
int ledPin=12;
int redLedPin=8;
int blueLedPin=7;
int greenLedPin=6;
int buttonPin=9;
int firstRound=0;
LedControl lc=LedControl(11,13,10,1);
//const int16_t AccelScaleFactor = 16384;
//const int16_t GyroScaleFactor = 131;
//int16_t AccelScaleFactorX = 1620;
//int16_t AccelScaleFactorY = 1130;
//int16_t AccelScaleFactorZ = 15550;
int16_t AccelWalkLimitX = 900;
int16_t AccelWalkLimitY = 900;
int16_t AccelWalkLimitZ = 900;
int16_t AccelRunLimitX = 5000;
int16_t AccelRunLimitY = 4000;
int16_t AccelRunLimitZ = 8000;
const int16_t GyroScaleFactor = 1;
ArduinoQueue<int> xQueue(5);
ArduinoQueue<int> yQueue(5);
ArduinoQueue<int> zQueue(5);
ArduinoQueue<int> tempQueue(5);
byte w[8]={B10000001,B10000001,B10000001,B10000001,B10011001,B10100101,B11000011,B10000001};
byte r[8]={B11111110,B10000001,B10000001,B10000010,B11111100,B10000100,B10000010,B10000001};
byte s[8]={B01111110,B10000001,B10000000,B01110000,B00001110,B00000001,B10000001,B01111110};
int training[5]={1,2,3,2,1};
int cycleIndex=0; 
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 53;
const String fileName="data/test";
File file;
int fileCount=0;
bool fileRead=true;

//Takes an integer and convert it into the string
char* convert_int16_to_str(int16_t i){
  sprintf(tmp_str,"%6d",i);
  return tmp_str;
  }
//To identify if SD card shield is working.
void sdCardCheck(){
  
   if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }
   // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }
  
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  // put your setup code here, to run once:
  }
//Read data from the created file
void readData(){
  if(!SD.begin(chipSelect)){
        Serial.println("Not able to begin SD card reader pin");
        delay(10);
        return;
        }
    file=SD.open(fileName+fileCount+".txt",FILE_READ);
    if(file){
      Serial.println(fileName+fileCount+".txt:");
      // read from the file until there's nothing else in it:
      while (file.available()) {
        Serial.write(file.read());
      }
      // close the file:
      //file.close();
       file.flush();
    }else{
      Serial.println("error opening "+fileName+fileCount+".txt:");
    }
  }
// Wirte data in the created file
void writeData(String data,bool isNewLine){
     if(!SD.begin(chipSelect)){
      Serial.println("Not able to begin SD card reader pin");
      delay(10);
      return;
      }
      Serial.println("Writelogic1");
      file.flush();
    file=SD.open(fileName+fileCount+".txt",FILE_WRITE);
    if(file){
      Serial.println("Writelogic2");
      Serial.print("Writing to "+fileName+fileCount+".txt");
      if(isNewLine){
        file.println(data);
      }else{
        file.print(data);
        }
      Serial.println("Writelogic3");
    // close the file:
      file.flush();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening "+fileName+fileCount+".txt");
    }
    

  }
//To collect accelerometer output in different files. Used for analysis
void collectData(String xdata,String ydata,String zdata,bool isNewLine){
  File fileX;
  File fileY;
  File fileZ;
     if(!SD.begin(chipSelect)){
      Serial.println("Not able to begin SD card reader pin");
      delay(10);
      return;
      }
    fileX.flush();
    fileX=SD.open("XaccData.txt",FILE_WRITE);
    if(fileX){
      Serial.print("Writing to XaccData.txt");
      if(isNewLine){
        fileX.println(xdata);
      }else{
        fileX.print(xdata);
        }
      fileX.flush();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening XaccData.txt");
    }

    fileY.flush();
    fileY=SD.open("YaccData.txt",FILE_WRITE);
    if(fileY){
      Serial.print("Writing to YaccData.txt");
      if(isNewLine){
        fileY.println(ydata);
      }else{
        fileY.print(ydata);
        }
      fileY.flush();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening YaccData.txt");
    }

        fileZ.flush();
    fileZ=SD.open("ZaccData.txt",FILE_WRITE);
    if(fileZ){
      Serial.print("Writing to ZaccData.txt");
      if(isNewLine){
        fileZ.println(zdata);
      }else{
        fileZ.print(zdata);
        }
      fileZ.flush();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening ZaccData.txt");
    }
  }
//Glow tri-color lead.
void glowLed(int leadGlow){
  if(leadGlow==1){
      digitalWrite(redLedPin,HIGH);
      delay(10);
      digitalWrite(redLedPin,LOW);
    }
    if(leadGlow==2){
      digitalWrite(blueLedPin,HIGH);
      delay(10);
      digitalWrite(blueLedPin,LOW);
    }
    if(leadGlow==3){
      digitalWrite(greenLedPin,HIGH);
      delay(10);
      digitalWrite(greenLedPin,LOW);
    }
    delay(100);
  }
// Logic using queue data structure to indentify the state of user.  
void stateIdentifier(int x,int y,int z){
  if(x<0){
    x=x*-1;
  }
  if(y<0){
    y=y*-1;
  }
  if(x<0){
    z=z*-1;
  }
  if(xQueue.isFull()){
    xQueue.dequeue();
  }
  if(yQueue.isFull()){
    yQueue.dequeue();
  }
  if(zQueue.isFull()){
    zQueue.dequeue();
  }
  if(x<old_accelerometer_x){
    xQueue.enqueue(old_accelerometer_x-x);
  }else{
    xQueue.enqueue(x-old_accelerometer_x);
  }
  old_accelerometer_x=x;
  if(y<old_accelerometer_y){
    yQueue.enqueue(old_accelerometer_y-y);
  }else{
    yQueue.enqueue(y-old_accelerometer_y);
  }
  old_accelerometer_y=y;
  if(z<old_accelerometer_z){
    zQueue.enqueue(old_accelerometer_z-z);
  }else{
    zQueue.enqueue(z-old_accelerometer_z);
  }
  old_accelerometer_z=z;
  int runCount=0;
  int walkCount=0;
  int stayCount=0;

  while(!xQueue.isEmpty()){
    int tempVal=xQueue.dequeue();
   if(tempVal>AccelRunLimitX){
     runCount+=1;
   }else if(tempVal>AccelWalkLimitX){
    walkCount+=1;
   }else{
    stayCount+=1;
   }
   tempQueue.enqueue(tempVal);
  }
  while(!tempQueue.isEmpty()){
    xQueue.enqueue(tempQueue.dequeue());
  }
  while(!yQueue.isEmpty()){
    int tempVal=yQueue.dequeue();
   if(tempVal>AccelRunLimitY){
     runCount+=1;
   }else if(tempVal>AccelWalkLimitY){
    walkCount+=1;
   }else{
    stayCount+=1;
   }
   tempQueue.enqueue(tempVal);
  }
  while(!tempQueue.isEmpty()){
    yQueue.enqueue(tempQueue.dequeue());
  }
  while(!zQueue.isEmpty()){
    int tempVal=zQueue.dequeue();
   if(tempVal>AccelRunLimitZ){
     runCount+=1;
   }else if(tempVal>AccelWalkLimitZ){
    walkCount+=1;
   }else{
    stayCount+=1;
   }
   tempQueue.enqueue(tempVal);
  }
  while(!tempQueue.isEmpty()){
    zQueue.enqueue(tempQueue.dequeue());
  }
  Serial.println("Total score:"+String(runCount)+":"+String(walkCount)+":"+String(stayCount));
  int currentCycle=training[cycleIndex];
  if(runCount>=6){
    glowLed(3);
    if(currentCycle!=3){
      display(currentCycle);
    }else{
      display(0);
    }
  }else if(walkCount>=9){
    glowLed(2);
    if(currentCycle!=2){
      display(currentCycle);
    }else{
      display(0);
    }
  }else{
    glowLed(1);
    if(currentCycle!=1){
      display(currentCycle);
    }else{
      display(0);
    }
  }
}
//Display differect characters in LED display(MAX7219)
void display(int x){
    if(x==1){
      lc.setRow(0,0,s[0]);
      lc.setRow(0,1,s[1]);
      lc.setRow(0,2,s[2]);
      lc.setRow(0,3,s[3]);
      lc.setRow(0,4,s[4]);
      lc.setRow(0,5,s[5]);
      lc.setRow(0,6,s[6]);
      lc.setRow(0,7,s[7]);
    }else if(x==2){
      lc.setRow(0,0,w[0]);
      lc.setRow(0,1,w[1]);
      lc.setRow(0,2,w[2]);
      lc.setRow(0,3,w[3]);
      lc.setRow(0,4,w[4]);
      lc.setRow(0,5,w[5]);
      lc.setRow(0,6,w[6]);
      lc.setRow(0,7,w[7]);
    }else if(x==3){
      lc.setRow(0,0,r[0]);
      lc.setRow(0,1,r[1]);
      lc.setRow(0,2,r[2]);
      lc.setRow(0,3,r[3]);
      lc.setRow(0,4,r[4]);
      lc.setRow(0,5,r[5]);
      lc.setRow(0,6,r[6]);
      lc.setRow(0,7,r[7]);
    }else{
      lc.setRow(0,0,0);
      lc.setRow(0,1,0);
      lc.setRow(0,2,0);
      lc.setRow(0,3,0);
      lc.setRow(0,4,0);
      lc.setRow(0,5,0);
      lc.setRow(0,6,0);
      lc.setRow(0,7,0);
    }

  }
// Setting us the different pins and testing SD card sheild
void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  
  // startPlayback(sample, sizeof(sample));
  pinMode(ledPin,OUTPUT);
  pinMode(redLedPin,OUTPUT);
  pinMode(blueLedPin,OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(chipSelect, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  sdCardCheck();
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  accelerometer_x=0;
  accelerometer_y=0;
  accelerometer_z=0;
  glowLed(1);
  glowLed(2);
  glowLed(3);
}
//Loop logic which record GY-521 output and analyse it.
void loop() {
  if(digitalRead(buttonPin)==false){
    digitalWrite(ledPin,HIGH);
    delay(2000);
    digitalWrite(ledPin,LOW);
    fileRead=false;
    flag=100;
    accelerometer_x=0;
    accelerometer_y=0;
    accelerometer_z=0;
    if(cycleIndex<4){
        cycleIndex+=1;
      }else{
        cycleIndex=0;
      }
    }
 if(flag>=0){
    String sdData="";
    digitalWrite(ledPin,HIGH);
    delay(10);
    digitalWrite(ledPin,LOW);
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR,7*2,true);
    accelerometer_x=Wire.read()<<8|Wire.read();
    accelerometer_y=Wire.read()<<8|Wire.read();
    accelerometer_z=Wire.read()<<8|Wire.read();
    temperature=Wire.read()<<8|Wire.read();
    gyro_x=Wire.read()<<8|Wire.read();
    gyro_y=Wire.read()<<8|Wire.read();
    gyro_z=Wire.read()<<8|Wire.read();
    stateIdentifier(accelerometer_x,accelerometer_y,accelerometer_z);
    sdData+="aX=";
    int aX=accelerometer_x;
    sdData+=convert_int16_to_str(aX);
    Serial.print(convert_int16_to_str(aX));
    sdData+=" aY=";
    //Serial.print(" aY=");
    int aY=accelerometer_y;
    sdData+=convert_int16_to_str(aY);
    Serial.print(convert_int16_to_str(aY));
    sdData+=" aZ=";
    //Serial.print(" aZ=");
    int aZ=accelerometer_z;
    sdData+=convert_int16_to_str(aZ);
    Serial.print(convert_int16_to_str(aZ));
    //collectData(convert_int16_to_str(aX),convert_int16_to_str(aY),convert_int16_to_str(aZ),true);
    sdData+=" gX=";
    //Serial.print(" gX=");
    int gX=gyro_x/GyroScaleFactor;
    sdData+=convert_int16_to_str(gX);
    //Serial.print(convert_int16_to_str(gX));
    sdData+=" gY=";
    //Serial.print(" gY=");
    int gY=gyro_y/GyroScaleFactor;
    sdData+=convert_int16_to_str(gY);
    //Serial.print(convert_int16_to_str(gY));
    sdData+=" gZ=";
    //Serial.print(" gZ=");
    int gZ=gyro_z/GyroScaleFactor;
    sdData+=convert_int16_to_str(gZ);
    //Serial.print(convert_int16_to_str(gZ));
    //Serial.print(" tmp=");
    //Serial.print(temperature/340.00+36.53);
    //sdData+=" vX=";
    //Serial.print(" vX=");
    //V_x=V_x+aX*0.610;
    //sdData+=V_x;
    //Serial.print(V_x);
    //sdData+=" vY=";
    //Serial.print(" vY=");
    //V_y=V_y+aY*0.610;
    //sdData+=V_y;
    //Serial.print(V_y);
    //sdData+=" vZ=";
    //Serial.print(" vZ=");
    //V_z=V_z+aZ*0.610;
    //sdData+=V_z;
    //Serial.print(V_z);
    Serial.println(sdData);
    //writeData(sdData,true);
    delay(400);
    flag--;
  }
  delay(100);
}
