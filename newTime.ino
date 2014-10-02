int dataPin = 22;
int latchPin = 24;
int clockPin = 26;


int button1 = 30;
int button2 = 32;
int button3 = 34;
int button4 = 36;

int digits[4] = {
  31,33,35,37  
};

int numbers[] = {
  136,237,195,161,228,176,144,233,128,160
};

int s = 0;
int m = 0;
int h = 0;
unsigned long currSec = 0;
unsigned long lastSec = 0;

unsigned long currMin = 0;
unsigned long lastMin = 0;

unsigned long currHour = 0;
unsigned long lastHour = 0;

unsigned long currMillis = 0;
void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  Serial.begin(9600);

  setTime(1,25);  
}

void loop(){
//  updateTime();
  updateTime();
  printTime();
  displayNumber(1234);
}


void printTime(){
  
  Serial.print(h);
  Serial.print(':');
  Serial.print(m);
  Serial.print(':');
  Serial.println(s);  
  
}

void updateTime(){
  currMillis = millis();
  currSec = currMin = currHour = currMillis;
  if(currSec - lastSec > 1000){
    s++; 
    lastSec = currSec;
  }
  if(currMin - lastMin > 60000){
    m++; 
    lastMin = currMin;
  }

  if(currHour - lastHour > 3600000){
    h++; 
    lastHour = currHour;
  }  
  
  if(s > 59)
    s = 0;
  if(m > 59)
    m = 0;
  if(h > 12)
    h = 1;
}

void setTime(int hour, int minute){
  h = hour;
 
  m = minute;  
}

void displayTime(int hour, int minute){
  
}
void displayNumber(int number){
  int d1 = number / 1000;
  int d2 = number % 1000 / 100;
  int d3 = number % 100 / 10;
  int d4 = number % 10;
  
  Serial.print(d1);
  Serial.print(d2);
  Serial.print(d3);
  Serial.print(d4); 
  
  for(int i = 0; i < 4; i++){

    digitalWrite(digits[i],1);
    
    digitalWrite(latchPin,0);
    
    switch(i){
    
     case 0:
       shiftOut(dataPin, clockPin, LSBFIRST, numbers[d1]);
     break;
     case 1:
       shiftOut(dataPin, clockPin, LSBFIRST, numbers[d2]);
     break;
     case 2:
       shiftOut(dataPin, clockPin, LSBFIRST, numbers[d3]);     
     break;
     case 3:
       shiftOut(dataPin, clockPin, LSBFIRST, numbers[d4]);    
     break;     
     
     
    }
   
     digitalWrite(latchPin,1); 
           
     delay(500);
     
     digitalWrite(digits[i],0);
  }
  
}
