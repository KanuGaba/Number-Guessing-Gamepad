#include <SoftwareSerial.h>
SoftwareSerial xbee(10, 9); // RX, TX

//Number Registers
int latchpinN = 11; // connect to pin 12 on the '595
int clockpinN = 12; // connect to pin 11 on the '595
int datapinN = 13; // connect to pin 14 on the '595
//Life Register
int latchpinL = 5; // connect to pin 12 on the '595
int clockpinL = 6; // connect to pin 11 on the '595
int datapinL = 7; // connect to pin 14 on the '595
//Buttons
int tenUpButton = 0;
int tenDownButton = 1;
int oneUpButton = 2;
int oneDownButton = 3;
int enterButton = 4;

int lifeArray[] = {0, 1, 3, 7, 15, 31};
int numArray[] = {126, 12, 182, 158, 204, 218, 250, 14, 254, 206};

int goalNum = 0;
bool numSent = false;

void setup() {
  xbee.begin(9600);
  
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  
  pinMode(latchpinN, OUTPUT);
  pinMode(clockpinN, OUTPUT);
  pinMode(datapinN, OUTPUT);
  
  pinMode(latchpinL, OUTPUT);
  pinMode(clockpinL, OUTPUT);
  pinMode(datapinL, OUTPUT);

  for (int x = 5; x >= 0; x--) {
    digitalWrite(latchpinL, LOW);
    shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[x]);
    digitalWrite(latchpinL, HIGH);
    delay(20);
  }
  
  digitalWrite(latchpinN, LOW);
  shiftOut(datapinN, clockpinN, MSBFIRST, numArray[0]);
  shiftOut(datapinN, clockpinN, MSBFIRST, numArray[0]);
  digitalWrite(latchpinN, HIGH);  

  while (!Serial) {}
}

void loop() {
  if(numSent) {
    for (int i = 0; i < 32767; i ++) {
        if (i % 2 == 0) {
          digitalWrite(latchpinN, LOW);
          shiftOut(datapinN, clockpinN, MSBFIRST, numArray[goalNum%10]);
          shiftOut(datapinN, clockpinN, MSBFIRST, numArray[goalNum/10]);
          digitalWrite(latchpinN, HIGH);
        }
        else {
          digitalWrite(latchpinN, LOW);
          shiftOut(datapinN, clockpinN, MSBFIRST, 0);
          shiftOut(datapinN, clockpinN, MSBFIRST, 0);
          digitalWrite(latchpinN, HIGH);
        }
        delay(500);
      }
  }
  else {
    if (digitalRead(enterButton) == HIGH) {
        xbee.write(goalNum);
        numSent = true;
    }
    else {
        if (digitalRead(oneUpButton) == HIGH) goalNum = (goalNum+1)%10;
        if (digitalRead(oneDownButton) == HIGH) goalNum = (goalNum-1)%10;
        if (digitalRead(tenUpButton) == HIGH) goalNum = (goalNum+10)%100;
        if (digitalRead(tenDownButton) == HIGH) goalNum = (goalNum-10)%100;
        digitalWrite(latchpinN, LOW);
        shiftOut(datapinN, clockpinN, MSBFIRST, numArray[goalNum%10]);
        shiftOut(datapinN, clockpinN, MSBFIRST, numArray[goalNum/10]);
        digitalWrite(latchpinN, HIGH);
    }
  }
}
