 #include <Wire.h>

const int MPU_ADDR=0x68;
int16_t accelerometer_x,accelerometer_y,accelerometer_z;
int16_t gyro_x,gyro_y,gyro_z;
int16_t temperature;
char tmp_str[7];
int flag=0;
int ledPin=12;
int buttonPin=9;
//const int16_t AccelScaleFactor = 16384;
//const int16_t GyroScaleFactor = 131;
const int16_t AccelScaleFactorX = 1;
const int16_t AccelScaleFactorY = 1;
const int16_t AccelScaleFactorZ = 1;
const int16_t GyroScaleFactor = 1;
int16_t V_x,V_y,V_z;

char* convert_int16_to_str(int16_t i){
  sprintf(tmp_str,"%6d",i);
  return tmp_str;
  }
void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  V_x=0;
  V_y=0;
  V_z=0;
}
void loop() {
  if(digitalRead(buttonPin)==false){
    digitalWrite(ledPin,HIGH);
    delay(2000);
    digitalWrite(ledPin,LOW);
    flag=200;
    }
   if(flag<=0){
    //Wire.endTransmission(true);
    V_x=0;
    V_y=0;
    V_z=0;
    
  }else{
    // put your main code here, to run repeatedly:
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
  
    Serial.print("aX=");
    int aX=accelerometer_x/AccelScaleFactorX;
    Serial.print(convert_int16_to_str(aX));
    Serial.print(" aY=");
    int aY=accelerometer_y/AccelScaleFactorY;
    Serial.print(convert_int16_to_str(aY));
    Serial.print(" aZ=");
    int aZ=accelerometer_z/AccelScaleFactorZ;
    Serial.print(convert_int16_to_str(aZ));
  
    Serial.print(" gX=");
    int gX=gyro_x/GyroScaleFactor;
    Serial.print(convert_int16_to_str(gX));
    Serial.print(" gY=");
    int gY=gyro_y/GyroScaleFactor;
    Serial.print(convert_int16_to_str(gY));
    Serial.print(" gZ=");
    int gZ=gyro_z/GyroScaleFactor;
    Serial.print(convert_int16_to_str(gZ));
  
    //Serial.print(" tmp=");
    
    //Serial.print(temperature/340.00+36.53);
    Serial.print(" vX=");
    V_x=V_x+aX*0.610;
    Serial.print(V_x);
    Serial.print(" vY=");
    V_y=V_y+aY*0.610;
    Serial.print(V_y);
    Serial.print(" vZ=");
    V_z=V_z+aZ*0.610;
    Serial.print(V_z);

    Serial.println();
    delay(500);
    flag--;
  }


  delay(100);
  
}
