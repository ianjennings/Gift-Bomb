#include <Time.h>  
#include <Servo.h> 

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

int pos = 0;    // variable to store the servo position 


Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int daysLeft = 0;
int numToLight = 0;

int ledPins[] = {2,3,4,5,6,7,8,9};

// T1262347200  //noon Jan 1 2010

void setup()  {
  delay(2000);
  myservo.attach(12);  // attaches the servo on pin 9 to the servo object 
  myservo.write(180);
  Serial.begin(9600);
  for(int i = 0; i <= 7; i++){         //this is a loop and will repeat eight times
    pinMode(ledPins[i],OUTPUT); //we use this to set each LED pin to output
  }     
}

void loop(){     
  
  for(int i = 0; i < 7; i++){
    digitalWrite(ledPins[i], LOW);  //Turns on LED #i each time this runs i
  }        
  
  digitalClockDisplay();  

  // valentines - todays day
  daysLeft = day(1360872000) - day();
  
  Serial.println("hour is ");
  Serial.println(hour());
  
  if(hour() > 9 && hour() < 23) {
    numToLight = 7 - daysLeft;    
  } else {
    numToLight = 0;
    Serial.println("sleep");
    // delay(60 * 60 * 8);
  }
  
  Serial.println("days");
  Serial.println(daysLeft);
  
  Serial.println("tolight");
  Serial.println(numToLight);
  
  for(int i = 0; i < numToLight; i++){
    digitalWrite(ledPins[i], HIGH);  //Turns on LED #i each time this runs i
  }
  
  if(daysLeft == 0){
  
    myservo.write(90);
  
    while(true){  
      
      for(int i = 0; i < numToLight; i++){
        digitalWrite(ledPins[i], HIGH);  //Turns on LED #i each time this runs i
      }
      delay(80);  
      for(int i = 0; i < numToLight; i++){
        digitalWrite(ledPins[i], LOW);  //Turns on LED #i each time this runs i
      }   
      delay(100); 
      for(int i = 0; i < numToLight; i++){
        digitalWrite(ledPins[i], HIGH);  //Turns on LED #i each time this runs i
      }
      delay(80);  
      for(int i = 0; i < numToLight; i++){
        digitalWrite(ledPins[i], LOW);  //Turns on LED #i each time this runs i
      }   
      delay(3000); 

    }
    
  }
  
  delay(3000);

  if(Serial.available() ) 
  {
    processSyncMessage();
  }
  if(timeStatus() == timeNotSet) 
    Serial.println("waiting for sync message");
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consisnuts of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
