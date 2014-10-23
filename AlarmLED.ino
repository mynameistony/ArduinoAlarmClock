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

int mode = 0;

int pulseInterval = 50;

boolean pulseIncreasing = 0;

int pulseBrightness = 0;

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

int alarmInterval = 5000;
int alarmHour = 7;
int alarmMin = 0;

boolean alarmAM = true;

boolean alarmTriggered = false;

unsigned long lastPrint = 0;

unsigned long lastFade = 0;

unsigned long lastPulse = 0;

unsigned long lastAlarm = 0;

int fadeInterval = 50;

boolean isOn = true;

boolean colorFade = false;

boolean pulseMode = false;

void setup(){
  Serial.begin(9600);
  ser.begin(9600);

  lightOn();
  ser.write(22);
  printInfo();
  
  STRIP_PINOUT;
  reset_strip();

  pinMode(13,OUTPUT); 
  
  mySend(colors);  
}

void loop(){ 
  

  if(ser.available() > 0){
    input = ser.readStringUntil('\n');  
    
    if(input == "1234")
      alarmTriggered = false;
      
    Serial.println(input[0]);
    
    switch(input[0]){
      
    case 'j':
      mode = 3;
      setStrip(minute);
    break;
      
    case 'p':
      pulseInterval = 50;
      if(mode == 1)
        mode = 0;
      else
        mode = 1;
    break;
      
    case 'f':
      fadeInterval = 50;
      
      if(mode == 2)
        mode = 0;
      else
        mode = 2;
    break;
      
  
    case 'm':
      currLength += 1;
      
      if(currLength > 10)
        currLength = 10;
        
      if(mode != 2)
        updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'l':
      currLength -= 1;
      
      if(currLength < 0)
        currLength = 0;
      
      if(mode != 2)  
        updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'u':
    
      switch(mode){
        case 0:
          currBrightness += 10;
      
          if(currBrightness > 255)
            currBrightness = 255;
      
          updateStrip(colors, currColor, currBrightness, currLength);
        break; 
        
        case 1: 
          pulseInterval -= 5;
          if(pulseInterval < 0)
            pulseInterval = 0;
        break;
        
        case 2:
          fadeInterval -= 5;
          if(fadeInterval < 0)
            fadeInterval = 0;
        break;
        
        case 3:
          pulseInterval -= 5;
          if(pulseInterval < 0)
            pulseInterval = 0;
          
        break;
      }
    break;
    case 'd':
    
      switch(mode){
        case 0:
          currBrightness -= 10;
      
          if(currBrightness < 0)
            currBrightness = 0;
      
          updateStrip(colors, currColor, currBrightness, currLength);    
        break;
      
        case 1:
          pulseInterval += 5;
          if(pulseInterval > 100)
            pulseInterval = 100;
         
        break;
       
        case 2:
          fadeInterval += 5;
          if(fadeInterval > 100)
            fadeInterval = 100;
       
        break; 
      }
       
    break;    
      
    case 'R':
        mode = 0;
	currColor = 0x010000;       
	updateStrip(colors, currColor, currBrightness, currLength);
    
    break;
    
    case 'B':
      mode = 0;
      currColor = 0x000100;      
      updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'G':
      mode = 0;
      currColor = 0x000001;
      updateStrip(colors, currColor, currBrightness, currLength);
    break;

    case 'C':
      mode = 0;
      currColor = 0x000101;
      updateStrip(colors, currColor, currBrightness, currLength);
    break;

    case 'M':
      mode = 0;
      currColor = 0x010100;    
      updateStrip(colors, currColor, currBrightness, currLength);
    break;   
   
    case 'Y':
      mode = 0;
      currColor = 0x010001;
      updateStrip(colors, currColor, currBrightness, currLength);
    break;    

    case 'W':
      mode = 0;
      currColor = 0x010101;
      updateStrip(colors, currColor, currBrightness, currLength);
    break;
    
    case 'O':
      mode = 0;
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


  
  switch(mode){
    case 0:
    break;
    
    case 1:
    if(millis() - lastPulse > pulseInterval){
      lastPulse = millis();
      
      for(int i = 0; i < 10; i++)
        if(i < currLength)
          colors[i] = pulseColor(currColor);
        else
          colors[i] = 0;
      
      mySend(colors);
       
      
    }      
    break;
    
    case 2:
      if(millis() - lastFade > fadeInterval){
        lastFade = millis();  
        for(int i = 0; i < 10; i ++)
            colorFadeStrip[i] = newColorFade(colorFadeStrip[i]);                
        mySend(colorFadeStrip);   
      }
    break;
    
    case 3:
    if(millis() - lastPulse > pulseInterval){
      lastPulse = millis();
      setStrip(minute);      
      for(int i = 0; i < 10; i++)
        if(i < currLength)
          colors[i] = pulseColor(currColor);
        else
          colors[i] = 0;
          
          
      
      mySend(colors);
       
      
    }      
    break;
   
 
  
  }
  
  if(alarmTriggered)
    doAlarm();
  
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
  
  switch(mode){
    case 0:
      ser.print("B: ");
      ser.print(currBrightness);
      ser.print(" L: ");
      ser.print(currLength);
    break;
    
    case 1:
      ser.print("Pulse Rate: ");
      ser.print(100 - pulseInterval);
    break;
    
    case 2:
      ser.print("Fade Rate: ");
      ser.print(100 - fadeInterval);
    break; 
    
    case 3:
      ser.print("T-Pulse Rate: ");
      ser.print(100 - pulseInterval);
    break;
    
  }
  
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
  
  if(hour == 4){
    if(minute > 19 && minute < 30){
      pulseInterval = 100 - (10 * (minute % 10));
      currColor = 0x000001;
      mode = 1; 
    }
    
    if(minute == 30){
      pulseInterval = 50;
      currColor = 0x000000;
      mode = 0;
    
      updateStrip(colors,0,0,0);
    }
    
  }
  
  if(mode == 3)
    setStrip(minute);          
      
    
  if(alarmHour == hour)
    if(alarmMin == minute)
      if(alarmAM ==isAM)
        alarmTriggered = true;
}

void doAlarm(){
  
  if(millis() - lastAlarm > 5000){
    lastAlarm = millis();
    
    ser.write(211);
    ser.write(220);  
    ser.write(220);  
    ser.write(220);  
    ser.write(220);  
  }
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

void setStrip(int thisMinute){
    switch(thisMinute / 10){
      
     case 0:
       currColor = 0x010000;
     break;   

     case 1:
       currColor = 0x000100;
     break;   

     case 2:
       currColor = 0x000001;
     break;   

     case 3:
       currColor = 0x010100;
     break;   

     case 4:
       currColor = 0x010001;
     break;   

     case 5:
       currColor = 0x000101;
     break;        
    }
    
    currLength = thisMinute % 10;
    
    if(currLength == 0)
      currLength = 10;
    
    
//    updateStrip(colors,currColor,125,currLength + 1);
  
  
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

unsigned long pulseColor(unsigned long thisColor){
  
  if(pulseIncreasing){
    pulseBrightness++;
    
    if(pulseBrightness > 254)
      pulseIncreasing = false;
  }
  
  else{
    pulseBrightness--;
    
    if(pulseBrightness < 1)
      pulseIncreasing = true;    
  }
    
  thisColor *= pulseBrightness;
  
  return thisColor;
}
