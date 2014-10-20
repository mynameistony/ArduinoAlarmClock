#include <SoftwareSerial.h>

SoftwareSerial ser(6,7);
String input = "NO-INPUT";

int minute = 0;

int hour = 12;

boolean isAM = true;

int alarmHour = 7;
int alarmMin = 0;

boolean alarmAM = true;

unsigned long lastPrint = 0;
void setup(){

  Serial.begin(9600);
  ser.begin(9600);

  ser.write(17); 
  printInfo();

  pinMode(13,OUTPUT); 
}

void loop(){ 

  if(ser.available() > 0){
    input = ser.readStringUntil('\n');  

    Serial.println(input[0]);

    switch(input[0]){

    case 't':
      minute++;
      updateTime();
    break;

    case 'T':
      Serial.println("Setting time...");
      for(int i = 1; i <= 4; i++)
        input[i] -= 48;
 
      hour = input[1] * 10 + input[2];
      minute = input[3] * 10 + input[4];
      
      switch(input[5]){
        case 'a':
          isAM = true;
        break;
        
        case 'A':
          isAM = true;
        break;
        
        case 'p':
          isAM = false;
        break;
        
        case 'P':
          isAM = false;
        break;
        
        default:
        break;
      }

      printInfo();
      break;

    case 'A':
        Serial.println("Setting alarm...");
        for(int i = 1; i <= 4; i++)
          input[i] -= 48;

        alarmHour = input[1] * 10 + input[2];
        alarmMin = input[3] * 10 + input[4];

        switch(input[5]){
          case 'a':
            alarmAM = true;
          break;
        
          case 'A':
            alarmAM = true;
          break;
        
          case 'p':
            alarmAM = false;
          break;
        
          case 'P':
            alarmAM = false;
          break;
        
          default:
          break;
        }
      
        printInfo();
      break;


    }

    printInfo();
  }  


}

void printInfo(){

  ser.write(12);

  setPos(1,0);

  ser.print(hour);

  ser.print(':');

  if(minute < 9)
    ser.print('0');
  ser.print(minute);

  if(isAM)
    ser.print("am");
  else
    ser.print("pm");

  setPos(1,9);

  ser.print(alarmHour);

  ser.print(':');

  if(alarmMin < 9)
    ser.print('0');

  ser.print(alarmMin);

  if(alarmAM)
    ser.print("am");
  else
    ser.print("pm");

}

void setPos(unsigned int r,unsigned int c){
  if(r == 0)
    ser.write(128 + c);
  else
    if(r == 1)
      ser.write(148 + c);
  else{
  } 

}

void updateTime(){
  if(minute > 59){
    minute = 0;
    hour++;
    
    if(hour > 11)
      if(isAM)
        isAM = false;
      else
        isAM = true;

    if(hour > 12){
      hour = 1;
    }
    
  }
  
  if(alarmHour == hour)
    if(alarmMin == minute)
      if(alarmAM ==isAM)
        doAlarm();  

}

void doAlarm(){
  
  ser.write(211);
  ser.write(220);  
  ser.write(220);  
  ser.write(220);  
  ser.write(220);  
}

