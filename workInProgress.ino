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

unsigned long rainbow[7] = {
  rr,ro,ry,rg,lightblue,rb,rp
};


unsigned long colors[10] = {
  rr,ro,ry,ry,rg,rg,rb,rb,rp,rp
};

unsigned long colorFadeStrip[10] = {
  rr,rr,rr,rr,rr,rr,rr,rr,rr,rr
};
int rainbowPos = 0;

int intAns = 0;

String strAns = "1234";

int randNum1 = 0;

int randNum2 = 0;

int mode = 0;

int pulseMax = 254;

int pulseMin = 1;

int pulseInterval = 50;

boolean pulseIncreasing = 0;

int pulseBrightness = 0;

int colorFadeMode = 0;

int currLength = 10;

int currRate = 100;

int currMode = 11;

int currBrightness = 125;

unsigned long currColor = 0x010101; 

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

String currFact[10] = {

  "","","","","","","","","","",
};

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

  randomSeed(A5);

}

void loop(){  
	if(ser.available() > 0){
		String newInput = ser.readStringUntil('\n');  
		doStuff(newInput);
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
				colors[i] = newColorFade(colors[i]);                
			mySend(colors);   
		}
	break;
    
	case 3:
		if(millis() - lastPulse > pulseInterval){
			lastPulse = millis();
			setStrip(hour,minute);  


                        if(currLength < 11){
                          for(int i = 0; i < 10; i++)
                            if(i < currLength)
                              colors[i] = pulseColor(currColor);
                            else
                              colors[i] = 0;   
                        }
                        else{
                            Serial.print("test\n");
//                            currLength = currLength % 10;
                            for(int i = 0; i < 10; i++)                                  
                              if(i < currLength - 10)
                                colors[i] = pulseColor(currColor);
                              else
                                colors[i] = currColor * 125;
                                
                               
                        }
                        
                        mySend(colors);
		}      
	break;

        case 4:
		if(millis() - lastFade > fadeInterval){
			lastFade = millis();  
                        for(int i = 0; i < 10; i++)
          			colors[i] = fadeToColor(colors[i],rainbow[random(0,6)]);                
			mySend(colors);   
		}              
        break;
  
	}

	if(alarmTriggered){
		doAlarm();
        }
	else{
		randNum1 = random(0,99);
		randNum2 = random(0,99);
		if(isOn)
			if(millis() - lastPrint > 5000)
				lightOff();
	}

}

void doStuff(String input){
	if(input == strAns)
		alarmTriggered = false;
      
	//   Serial.println(input[0]);
    
	switch(input[0]){
  
                case 'v':
                  fadeInterval = 50;  
  
                  if(mode != 4)
                    mode = 4;
                  else
                    mode = 0;                
                break;
                
                case 'Z':
                  
                  currFact[0] = input;
//                  currFact[0] = '*';

                  Serial.println(input);
                  Serial.println(currFact[0]);
                break;
                
                case 'q':
                        pulseMin -= 5;
                        if(pulseMin < 1)
                          pulseMin = 1;
                            
                break;
                
                case 'w':
                        pulseMin += 5;
                        if(pulseMin > 254)
                          pulseMin = 254;
                        if(pulseMin > pulseMax)
                          pulseMin = pulseMax;
                          
                break;
                
                case 'e':
                        pulseMax -= 5;
                        if(pulseMax < 1)
                          pulseMax = 1;
                          
                        if(pulseMax < pulseMin)
                          pulseMax = pulseMin;
                break;
               
                case 'r':
                        pulseMax += 5;
                        if(pulseMax > 254)
                          pulseMax = 254;
                break; 
      
		case 'j':
			pulseInterval = 50;
			setStrip(hour,minute);
	
			if(mode != 3)
				mode = 3;
			else
				mode = 0;
		break;
      
		case 'P':
			pulseInterval = 50;
			
			if(mode != 1)
				mode = 1;
			else
				mode = 0;
		break;
      
		case 'F':
                        fadeInterval = 50;
                              
			if(mode != 2)
				mode = 2;
			else
				mode = 0;
		break;
      
  
		case 'm':
			currLength += 1;
      
			if(currLength > 10)
				currLength = 10;
        
			if(mode == 0)
				updateStrip(colors, currColor, currBrightness, currLength);

		break;
    
		case 'l':
			currLength -= 1;
      
			if(currLength < 0)
				currLength = 0;
      
			if(mode == 0)  
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

				case 3:
					pulseInterval += 5;

					if(pulseInterval > 100)
						pulseInterval = 100;
				break;
			}
       
		break;    
      
		case 'R':
			mode = 1;
			currColor = 0x010000;       
			updateStrip(colors, currColor, currBrightness, currLength);
    
			break;
    
		case 'B':
			mode = 1;
			currColor = 0x000100;      
			updateStrip(colors, currColor, currBrightness, currLength);
		break;
    
		case 'G':
			mode = 1;
			currColor = 0x000001;
			updateStrip(colors, currColor, currBrightness, currLength);
		break;

		case 'C':
			mode = 1;
			currColor = 0x000101;
			updateStrip(colors, currColor, currBrightness, currLength);
		break;

		case 'M':
			mode = 1;
			currColor = 0x010100;    
			updateStrip(colors, currColor, currBrightness, currLength);
		break;   
   
		case 'Y':
			mode = 1;
			currColor = 0x010001;
			updateStrip(colors, currColor, currBrightness, currLength);
		break;    

		case 'W':
			mode = 1;
			currColor = 0x010101;
			updateStrip(colors, currColor, currBrightness, currLength);
		break;
    
		case 'O':
			mode = 1;
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
 
			if(input[5] == 'a' || input[5] == 'A')
				isAM = true;
			if(input[5] == 'p' || input[5] == 'P')
				isAM = false;
    
			//printInfo();
		break;

		case 'A':
			Serial.println("Setting alarm...");
			for(int i = 1; i <= 4; i++)
				input[i] -= 48;
			
			alarmHour = input[1] * 10 + input[2];
			alarmMin = input[3] * 10 + input[4];
			
			if(input[5] == 'a' || input[5] == 'A')
				alarmAM = true;
			if(input[5] == 'p' || input[5] == 'P')
				alarmAM = false;

			//        printInfo();
		break;

	}
}

void printInfo(){
	lastPrint = millis();
	  
	lightOn();
	  
	clearScreen();  
	  
	if(alarmTriggered){
		ser.print("What is ");
		ser.print(randNum1);
		ser.print(" * ");
		ser.print(randNum2);
		ser.print('?');
                
                setPos(1,0);
//                ser.print(currFact[0]);
//		ser.print(strAns);
	}
	  
	else{
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
                                ser.print(pulseMin);
                                ser.print('-');
                                ser.print(pulseMax);                                
				ser.print(" Rate:");
				ser.print(100 - pulseInterval);
			break;
		    
			case 2:
				ser.print("Fade Rate: ");
				ser.print(100 - fadeInterval);
			break; 
		    
			case 3:
                                ser.print(pulseMin);
                                ser.print('-');
                                ser.print(pulseMax);   
                                ser.print("T-Rate:");
				ser.print(100 - pulseInterval);
			break;
	    
		}
	}  
	//  ser.print(hourUntil);
	//  ser.print(':');
	//  if(minUntil < 10);
	//    ser.print('0');
	//  ser.print(minUntil);
	//  ser.print(" left");
}

void setPos(unsigned int r,unsigned int c){
	if(r == 0 )
		ser.write(128 + c);
	else
	if(r == 1)
		ser.write(148 + c);
	else{} 
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

		if(hour > 12)

			hour = 1;
		lightOn();
                beepTime(hour);
	}
  
	if(hour == 4){
		if(minute > 19 && minute < 30){
                        currLength = 10 - (minute % 10);
			pulseInterval = 50 - map(minute % 10,0, 9, 5, 45);
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
		setStrip(hour,minute);          
      
    	if(alarmHour == hour)
		if(alarmMin == minute)
			if(alarmAM ==isAM){
				alarmTriggered = true;
				intAns = randNum1 * randNum2;
				strAns = intToString(intAns);
			}
}

String intToString(int thisInt){
	
	String temp = "xxxx";
		for(int i = 0; i < 4; i++){
			switch(i){
				case 0:
					temp[i] = (thisInt / 1000) + 48; 
				break;

				case 1:
					temp[i] = ((thisInt % 1000) / 100) + 48;
				break;
			
				case 2:
					temp[i] = ((thisInt % 100) / 10) + 48;
				break;
				
				case 3:
					temp[i] = (thisInt % 10) + 48;
				break;    
			}  
		}
  
	return temp; 
}

void setCode(int newCode){
  
	for(int i = 0; i < 4; i++){

		switch(i){
			case 0:
				strAns[i] = (newCode / 1000) + 48; 
			break;

			case 1:
				strAns[i] = ((newCode % 1000) / 100) + 48;
			break;
			
			case 2:
				strAns[i] = ((newCode % 100) / 10) + 48;
			break;
		
			case 3:
				strAns[i] = (newCode % 10) + 48;
			break;    
		}  
	}  
}

void beepTime(int thisTime){
  ser.write(211);
  for(int i = 0; i < thisTime; i++)
          ser.write(220);        
  
}
void doAlarm(){

	if(millis() - lastAlarm > 5000){
		lastAlarm = millis();
		lightOn();    
		ser.write(211);
		ser.write(220);  
		ser.write(220);  
		ser.write(220);  
		ser.write(220); 
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

void setStrip(int thisHour, int thisMinute){
//	if(thisHour <= 10)
        currLength = thisMinute % 10;
        
        if(currLength == 0)
          currLength = 10;
          
        pulseInterval = 50 - map(minute / 10,0,5,0,45);        
        
//	else
//		currLength = 10;
		     
	switch(thisHour ){
	      
		case 0:
			//currColor = 0x010000;
		break;   

		case 1:
			currColor = 0x010000;
		break;   

		case 2:
			currColor = 0x010100;
		break;   

		case 3:
			currColor = 0x000100;
		break;   

		case 4:
			currColor = 0x000101;
		break;   

		case 5:
			currColor = 0x000001;
		break;        

                case 6:
                        currColor = 0x010001;
                break;

		case 7:
			currColor = 0x010000;
		break;   

		case 8:
			currColor = 0x010100;
		break;   

		case 9:
			currColor = 0x000100;
		break;   

		case 10:
			currColor = 0x000101;
		break;   

		case 11:
			currColor = 0x000001;
		break;        

                case 12:
                        currColor = 0x010001;
                break;
             
	}

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
    
		if(pulseBrightness > pulseMax)
			pulseIncreasing = false;
	}
  
	else{
	pulseBrightness--;
    
		if(pulseBrightness < pulseMin)
			pulseIncreasing = true;    
	}
    
	thisColor *= pulseBrightness;
  
	return thisColor;
}

unsigned long fadeToColor(unsigned long thisColor, unsigned long nextColor){
	int r1 =  thisColor / 0x010000;
	int b1 = (thisColor / 0x000100) % 0x000100 ;
	int g1 = thisColor % 0x000100;  

	int r2 =  nextColor / 0x010000;
	int b2 = (nextColor / 0x000100) % 0x000100 ;
	int g2 = nextColor % 0x000100;  
        
        unsigned long temp = thisColor;
        
        if(r1 < r2)
          temp += 0x010000;
        else
        if(r1 > r2)
          temp -= 0x010000;
  
        if(b1 < b2)
          temp += 0x000100;
        else
        if(b1 > b2)
          temp -= 0x000100;
  
        if(g1 < g2)
          temp += 0x000001;
        else
        if(g1 > g2)
          temp -= 0x000001;        
          
        return temp;
}
