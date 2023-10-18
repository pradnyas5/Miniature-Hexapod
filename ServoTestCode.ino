#include <Servo.h>
#include <SPI.h>
#include <SD.h> 
#include "HX711.h"

File myfile ;
Servo servo ;
HX711 scale;

const int LOADCELL_DT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
const int chipSelect = 10 ;
float vtg ;
float CurrentVal ;
int AgVal ;
float vtg1;
float Weight ;
int StartTime ;
int EndTime ;

void setup() {
  Serial.begin(9600); 
  servo.attach(9);
  servo.write(180);
  scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(164.56);
  scale.tare();
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}
 
void loop() {
  for(int i=180; i>=0; i-=10){
  StartTime = millis(); //Process start time
  servo.write(i);
  Serial.print("Servo Position: ");
  Serial.print(i); 

  AgVal = analogRead(A0);
  CurrentVal = MeasureCurrent(AgVal);  
  Serial.print(" | Current: ");
  Serial.print(CurrentVal); //current = current + (.0264 * analogRead(A0) -13.51) / 1000;//this is for the 5A mode, if 20A or 30A mode, need to modify this formula to(.19 * analogRead(A0) -25) for 20A mode and(.044 * analogRead(A0) -3.78) for 30A mode
  Serial.print(" A");
  
  Weight = scale.get_units(10), 5 ; 
  Serial.print(" | LoadCell Reading: ");  
  Serial.print(Weight);
  Serial.println(" g");
  EndTime = millis(); //Process End Time
  
  LogData(i, CurrentVal, Weight, StartTime, EndTime);
  delay(5);
 }
 for(int i=0; i<=180; i+=10){
  StartTime = millis();
  servo.write(i);
  Serial.print("Servo Position: ");
  Serial.print(i); 
  
  AgVal = analogRead(A0);
  CurrentVal = MeasureCurrent(AgVal);
  Serial.print(" | Current: ");
  Serial.print(CurrentVal); //current = current + (.0264 * analogRead(A0) -13.51) / 1000;//this is for the 5A mode, if 20A or 30A mode, need to modify this formula to(.19 * analogRead(A0) -25) for 20A mode and(.044 * analogRead(A0) -3.78) for 30A mode/
  Serial.print(" A"); 

  Weight = scale.get_units(10), 5 ; 
  Serial.print(" | LoadCell Reading: ");  
  Serial.print(Weight);
  Serial.println(" g");
  EndTime = millis();
  
  LogData(i, CurrentVal, Weight, StartTime, EndTime);
  delay(5);
 }
}

float MeasureCurrent(int input){
    float avg;
    vtg = input*4.887 ; //in mV
    vtg1 = vtg/1000;
    //Serial.print(vtg1);
    avg = abs((vtg1 - 2.5)/0.185);    
    //Serial.println(avg);
    delay(1);
  return avg ;
}

void LogData(int pos, float current, float weight, int ST, int ET){
  myfile = SD.open("Test_2.txt", FILE_WRITE); 
  if(myfile){
    myfile.print("Servo Position: ");
    myfile.print(pos);
    myfile.print("| Observed Current: ");
    myfile.print(current) ;
    myfile.print(" A ");
    myfile.print("| Load Cell Reading: ");
    myfile.print(weight);
    myfile.print(" g ");
    myfile.print("| Process Start Time: ");
    myfile.print(ST);
    myfile.print(" ms ");
    myfile.print("| Process End Time: ");
    myfile.print(ET);
    myfile.println(" ms ");
  }
  else{
    Serial.println("Error opening the file");
  }
  myfile.close();
}
