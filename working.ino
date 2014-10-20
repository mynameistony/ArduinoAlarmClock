#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

#define DATA_1 (PORTC |=  0X01)    // DATA 1    // for UNO
#define DATA_0 (PORTC &=  0XFE)    // DATA 0    // for UNO
#define STRIP_PINOUT (DDRC=0xFF)    // for UNO

#define rr  0xff0000
#define ro  0x6f002f 
#define ry  0x8f008f
#define rg  0x0000ff
#define rb  0x00ff00
#define rp  0x8f8f00
#define oo  0x000000
#define lightblue  0x008f8f

SoftwareSerial ser(6,7);

unsigned long colors[10] = {
  rr,ro,ry,ry,rg,rg,rb,rb,rp,rp
};


int currLength = 10;

int currRate = 100;

int currMode = 11;

int currBrightness = 125;

unsigned long currColor = 0x010101; 

String input = "NO-INPUT";


int minute = 0;

int hour = 12;

boolean isAM = true;

int alarmHour = 7;
int alarmMin = 0;

boolean alarmAM = true;

unsigned long lastPrint = 0;

boolean isOn = true;
void setup(){
  Serial.begin(9600);
  ser.begin(9600);

  lightOn();
  printInfo();
  
  STRIP_PINOUT;
  reset_strip();

  pinMode(13,OUTPUT); 
  
  mySend(colors);  
}

void loop(){ 
  

  if(ser.available() > 0){
    input = ser.readStringUntil('\n');  

    Serial.println(input[0]);

    switch(input[0]){
      
    case 'm':
      currLength += 1;
      
      if(currLength > 10)
        currLength = 10;
        
      updateStrip();
    break;
    
    case 'l':
      currLength -= 1;
      
      if(currLength < 0)
        currLength = 0;
        
      updateStrip();
    break;
    
    case 'u':
      currBrightness += 10;
      
      if(currBrightness > 255)
        currBrightness = 255;
      
      updateStrip();
    break;
    case 'd':
      currBrightness -= 10;
      
      if(currBrightness < 0)
        currBrightness = 0;
      
      updateStrip();    
    break;    
      
    case 'R':
	currColor = 0x010000;       
	updateStrip();
    
    break;
    
    case 'B':
			currColor = 0x000100;      
			updateStrip();
    break;
    
    case 'G':
			currColor = 0x000001;
			updateStrip();
    break;

    case 'C':
			currColor = 0x000101;
			updateStrip();
    break;

    case 'M':
			currColor = 0x010100;    
			updateStrip();
    break;   
   
    case 'Y':
			currColor = 0x010001;
			updateStrip();
    break;    

    case 'W':
			currColor = 0x010101;
			updateStrip();
    break;
    
    case 'O':
			currColor = 0x000000;
			updateStrip();
    break; 
    
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

//      printInfo();
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
      
//        printInfo();
      break;

    }

    printInfo();
  }  

}

void printInfo(){
//  lastPrint = millis();
  clearScreen();  
  
  setPos(0,0);

  ser.print(hour);

  ser.print(':');

  if(minute < 9)
    ser.print('0');
    
  ser.print(minute);

  if(isAM)
    ser.print("am");
  else
    ser.print("pm");

  setPos(0,9);

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

void lightOn(){
  
  ser.write(17);
  isOn = true;
}

void lightOff(){
  ser.write(18); 
  isOn = false;  
}

void clearScreen(){
  ser.write(12);  
}


void send_strip(uint32_t data){
	int i;
	unsigned long j=0x800000;
 
	for (i=0;i<24;i++){
		if (data & j){
			DATA_1;
			__asm__("nop\n\t");    
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");    
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			    
			/*----------------------------*/
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");  
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");  
			__asm__("nop\n\t");  
			__asm__("nop\n\t");        
			/*---------------------------*/      
			DATA_0;
		}
		else{
			DATA_1;
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");    
			DATA_0;
			/*----------------------------*/      
			__asm__("nop\n\t");
			__asm__("nop\n\t");
			__asm__("nop\n\t");      
			/*----------------------------*/         
		}

	j>>=1;
	}  
}

void reset_strip(){
	DATA_0;
	delayMicroseconds(20);
}

void mySend(unsigned long data[10]){
	
	// 32 bit temporary variable
	uint32_t temp_data;

	// Disable Interupts for time-critical operation
	noInterrupts();
	
	// Loop thru input array
	// Sending each element to the LED strip
	for(int i = 0; i < 10; i++){
		temp_data=data[i];
		send_strip(temp_data);
	}
	
	// Re-enable interrupts
	interrupts();

}

void updateStrip(){

	// Loop through strip segments
	for(int i = 0; i < 10; i++)
		// Limit to current length
		if(i < currLength)
			// Set segment according to current settings
			colors[i] = currColor * currBrightness;
		else
			// Set others off
			colors[i] = 0x000000;

	// Refresh Strip
	mySend(colors);
}

