//#include <Wire.h>

const int MPU_ADDR=0x68;
int16_t accelerometer_x,accelerometer_y,accelerometer_z;
int16_t gyro_x,gyro_y,gyro_z;
int16_t temperature;
char tmp_str[7];
int flag=0;
int ledPin=12;
int buttonPin=9;

char* convert_int16_to_str(int16_t i){
  sprintf(tmp_str,"%6d",i);
  return tmp_str;
  }
void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Wire.begin();
//  Wire.beginTransmission(MPU_ADDR);
//  Wire.write(0x68);
//  Wire.write(0);
//  Wire.endTransmission(true);
  flag=100;
}
void loop() {
  if(digitalRead(buttonPin)==false){
    digitalWrite(ledPin,HIGH);
    delay(2000);
    digitalWrite(ledPin,LOW);
    flag=100;
    }
   if(flag<=0){
    //Wire.endTransmission(true);
    
  }else{
    // put your main code here, to run repeatedly:
    digitalWrite(12,HIGH);
    delay(10);
    digitalWrite(12,LOW);
    flag--;
  }
//  Wire.beginTransmission(MPU_ADDR);
//  Wire.write(0x3B);
//  Wire.endTransmission(false);
//  Wire.requestFrom(MPU_ADDR,7*2,true);
//  accelerometer_x=Wire.read()<<8|Wire.read();
//  accelerometer_y=Wire.read()<<8|Wire.read();
//  accelerometer_z=Wire.read()<<8|Wire.read();
//  temperature=Wire.read()<<8|Wire.read();
//  gyro_x=Wire.read()<<8|Wire.read();
//  gyro_y=Wire.read()<<8|Wire.read();
//  gyro_z=Wire.read()<<8|Wire.read();
//
//  Serial.print("aX= ");
//  Serial.print(convert_int16_to_str(accelerometer_x));
//  Serial.print("aY= ");
//  Serial.print(convert_int16_to_str(accelerometer_y));
//  Serial.print("aZ= ");
//  Serial.print(convert_int16_to_str(accelerometer_z));
//
//  Serial.print("gX= ");
//  Serial.print(convert_int16_to_str(gyro_x));
//  Serial.print("gY= ");
//  Serial.print(convert_int16_to_str(gyro_y));
//  Serial.print("gZ= ");
//  Serial.print(convert_int16_to_str(gyro_z));
//
//  Serial.print("tmp= ");
//  Serial.print(temperature/340.00+36.53);
//  Serial.println();

  delay(100);
  
}
