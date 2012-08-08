#include <Time.h>  

int pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 22, 23, 24, 25};
const int pinCount = 16;
const int startTime = 1344227822;
int hourPin = 0;
unsigned long currentMillis = millis();

void setup()  {
  Serial.begin(9600);
  setUpPins();
  setTime(startTime);
  updateTime();
}

void loop(){   
  unsigned long prevtime = minute();
  while(prevtime == minute()) {
     if((currentMillis  + 1000) < millis()) {
       Serial.print("Hour: ");
       Serial.println(hourFormat12());
       if(hourPin) {
         printHour(HIGH);
         hourPin = 0;
       } else {
         printHour(LOW);
         hourPin = 1;
       }
       currentMillis = millis();
     }
  } 
  updateTime();
}

void updateTime() {
  clearPins();
  printFiveMin(minute());
  printMin(minute());  
}

void setUpPins() {
    for(int i = 0; i <= pinCount; i++) {
       pinMode(pins[i], OUTPUT); 
    }
}

void printHour(int state) {
  int hour = hourFormat12();
  digitalWrite(2 + hour, state);  
}

void clearPins() {
    for(int i = 0; i <= pinCount; i++) {
       digitalWrite(pins[i], LOW);
    }
}


void printFiveMin(int curmin) {
  int led = curmin / 5;
  for(int i = 2; i < (2 + led); i++) {
      digitalWrite(i, HIGH);
  }
  Serial.println("Print 5 min");
  Serial.print("Minute: ");
  Serial.print(curmin);
  Serial.print(", Led: ");
  Serial.print(led);
  Serial.println();
}

void printMin(int curmin) {
  int led = curmin % 5;
  for(int i = 22; i < (22 + led); i++) {
      digitalWrite(i, HIGH);
  }  
  Serial.println("PrintMin");
  Serial.print("Minute: ");
  Serial.print(curmin);
  Serial.print(", Led: ");
  Serial.print(led);
  Serial.println();
}
