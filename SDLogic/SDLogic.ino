#include <SPI.h>
#include <SD.h>
const String fileName="data/test";
int count=0;
File file;
int fileCount=0;

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 53;

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
      file.close();
    }else{
      Serial.println("error opening test.txt");
    }
  }
void writeData(String data,bool isNewLine){
    if(!SD.begin(chipSelect)){
        Serial.println("Not able to begin SD card reader pin");
        delay(10);
        return;
        }
    file=SD.open(fileName+fileCount+".txt",FILE_WRITE);
    if(file){
      Serial.print("Writing to "+fileName+fileCount+".txt");
      if(isNewLine){
        file.println(data);
      }else{
        file.print(data);
        }
      
    // close the file:
      file.flush();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  }
void setup() {
  Serial.begin(9600);
  Serial.print("\nInitializing SD card...");
  pinMode(chipSelect, OUTPUT);
  delay(20);
  fileCount+=1;
  sdCardCheck();
  writeData("Test Data Added",true);
  //readData();
  count=0;
}

void loop() {
  if(count>=50){
    return;
    }
  count++;
  readData();
//  if(!SD.begin(pinSD)){
//    Serial.println("Not able to begin SD card reader pin");
//    delay(10);
//    return;
//    }
//  file=SD.open(fileName,FILE_WRITE);
//  if(file){
//    Serial.println("Writing data in the file.");
//    file.println("Hello world");
//    file.close();
//    }else{
//      Serial.println("Not able to open file");
//      delay(10);
//      }
//  // put your main code here, to run repeatedly:

}
