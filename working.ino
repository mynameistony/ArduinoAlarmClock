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

unsigned long colorFadeStrip[10] = {
  rr,rr,rr,rr,rr,rr,rr,rr,rr,rr
};

int colorFadeMode = 0;

int currLength = 10;

int currRate = 100;

int currMode = 11;

int currBrightness = 125;

unsigned long currColor = 0x010101; 

String input = "NO-INPUT";

int hourUntil = 0;

int minUntil = 0;

int minute = 0;

int hour = 12;

boolean isAM = true;

int alarmHour = 7;
int alarmMin = 0;

boolean alarmAM = true;

unsigned long lastPrint = 0;

unsigned long lastFade = 0;

int fadeInterval = 50;

boolean isOn = true;

boolean colorFade = false;

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
      
    case 'f':
      if(colorFade)
        colorFade = false;
      else
        colorFade = true;
    break;
      
  
    case 'm':
      currLength += 1;
      
      if(currLength > 10)
        currLength = 10;
        
      updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'l':
      currLength -= 1;
      
      if(currLength < 0)
        currLength = 0;
        
            updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'u':
      currBrightness += 10;
      
      if(currBrightness > 255)
        currBrightness = 255;
      
      updateStrip(colors, currColor, currBrightness, currLength);
    break;
    case 'd':
      currBrightness -= 10;
      
      if(currBrightness < 0)
        currBrightness = 0;
      
      updateStrip(colors, currColor, currBrightness, currLength);    
    break;    
      
    case 'R':
	currColor = 0x010000;       
	updateStrip(colors, currColor, currBrightness, currLength);
    
    break;
    
    case 'B':
			currColor = 0x000100;      
			updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'G':
			currColor = 0x000001;
			updateStrip(colors, currColor, currBrightness, currLength);
    break;

    case 'C':
			currColor = 0x000101;
			updateStrip(colors, currColor, currBrightness, currLength);
    break;

    case 'M':
			currColor = 0x010100;    
			updateStrip(colors, currColor, currBrightness, currLength);
    break;   
   
    case 'Y':
			currColor = 0x010001;
			updateStrip(colors, currColor, currBrightness, currLength);
    break;    

    case 'W':
			currColor = 0x010101;
			updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'O':
			currColor = 0x000000;
			updateStrip(colors, currColor, currBrightness, currLength);
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

  if(colorFade){
    if(millis() - lastFade > fadeInterval){
      lastFade = millis();  
      for(int i = 0; i < 10; i ++)
        colorFadeStrip[i] = newColorFade(colorFadeStrip[i]);  
      mySend(colorFadeStrip);   
    }
  }
  
  if(isOn)
    if(millis() - lastPrint > 5000)
      lightOff();

}

void printInfo(){
  lastPrint = millis();
  
  lightOn();
  
  clearScreen();  
  
  
  setPos(0,0);

  ser.print(hour);

  ser.print(':');

  if(minute < 10)
    ser.print('0');
    
  ser.print(minute);

  if(isAM)
    ser.print("am");
  else
    ser.print("pm");

  setPos(0,9);

  ser.print(alarmHour);

  ser.print(':');

  if(alarmMin < 10)
    ser.print('0');

  ser.print(alarmMin);

  if(alarmAM)
    ser.print("am");
  else
    ser.print("pm");
  
  setPos(1,0);
  
//  ser.print(hourUntil);
//  ser.print(':');
//  if(minUntil < 10);
//    ser.print('0');
//  ser.print(minUntil);
//  ser.print(" left");
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

void updateStrip(unsigned long thisStrip[10], unsigned long newColor, int newBrightness, int newLength){

	// Loop through strip segments
	for(int i = 0; i < 10; i++)
		// Limit to current length
		if(i < newLength)
			// Set segment according to current settings
			thisStrip[i] = newColor * newBrightness;
		else
			// Set others off
			thisStrip[i] = 0x000000;

	// Refresh Strip
	mySend(thisStrip);
}

unsigned long newColorFade(unsigned long data){

	// Break down value into Red, Green and Blue values
	int r =  data / 0x010000;
	int b = (data / 0x000100) % 0x000100 ;
	int g = data % 0x000100;  
	  
	//  Serial.print("Data: ");
	//  Serial.print(data, HEX);
	//  Serial.print("\nR: ");
	//  Serial.print(r, HEX);
	//  Serial.print("\tB: ");
	//  Serial.print(b, HEX);
	//  Serial.print("\tG: ");
	//  Serial.print(g, HEX);
	//  Serial.print("\tMode: ");
	//  Serial.println(colorFadeMode);
	
	switch(colorFadeMode){
	
	// Red -> Blue  
	case 0:
		if(r > 0){
			data -= 0x010000;
			data += 0x000100;
		}
		else
			colorFadeMode = 1;
	break;

	// Blue -> Green
	case 1:
		if(b > 0){
			data -= 0x000100;
			data += 0x000001; 
		}
		else
			colorFadeMode = 2;
	break;

	// Green -> Red
	case 2:
		if(g > 0){
			data += 0x010000;
			data -= 0x000001; 
		}
		else
			colorFadeMode = 0;        
	break;

	default:
		Serial.print("\nShits fucked\n");     
	break;
	}    

	return data;
}
