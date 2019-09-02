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

bool isGuessing = false;
int lives = 5;
int goalNum = 0;
int guessNum = 0;

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

  for (int x = 1; x < 6; x++) {
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
  if(isGuessing == false) {
    while (xbee.available() > 0) {
      goalNum = xbee.read();
      isGuessing = true;
    }
  }
  else{
    if (digitalRead(enterButton) == HIGH) {
      if(guessNum == goalNum) {
        for (int i = 0; i < 32767; i ++) {
          if (i % 2 == 0) {
            digitalWrite(latchpinL, LOW);
            shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[5]+128);
            digitalWrite(latchpinL, HIGH);
          }
          else {
            digitalWrite(latchpinL, LOW);
            shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[0]);
            digitalWrite(latchpinL, HIGH);
          }
          delay(500);
        }
      }
      else {
        lives = lives - 1;
        int tenDigGuess = guessNum/10;
        int tenDigGoal = goalNum/10;
        int oneDigGuess = guessNum%10;
        int oneDigGoal = goalNum%10;
        if(tenDigGuess == tenDigGoal || oneDigGuess == oneDigGoal) {
          digitalWrite(latchpinL, LOW);
          shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[lives]+128);
          digitalWrite(latchpinL, HIGH);
        }
        else if(tenDigGuess == oneDigGoal || oneDigGuess == tenDigGoal) {
          digitalWrite(latchpinL, LOW);
          shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[lives]+64);
          digitalWrite(latchpinL, HIGH);
        }
        else {
          digitalWrite(latchpinL, LOW);
          shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[lives]+32);
          digitalWrite(latchpinL, HIGH);
        }
      }
    }
    else if (lives == 0) {
      //infinite for loop for flashing RED LED and display?
      for (int i = 0; i < 32767; i ++) {
        if (i % 2 == 0) {
          digitalWrite(latchpinL, LOW);
          shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[5]+32);
          digitalWrite(latchpinL, HIGH);
        }
        else {
          digitalWrite(latchpinL, LOW);
          shiftOut(datapinL, clockpinL, MSBFIRST, lifeArray[0]);
          digitalWrite(latchpinL, HIGH);
        }
        delay(500);
      }
    }
    else {
      if (digitalRead(oneUpButton) == HIGH) guessNum = (guessNum+1)%10;
      if (digitalRead(oneDownButton) == HIGH) guessNum = (guessNum-1)%10;
      if (digitalRead(tenUpButton) == HIGH) guessNum = (guessNum+10)%100;
      if (digitalRead(tenDownButton) == HIGH) guessNum = (guessNum-10)%100;
      digitalWrite(latchpinN, LOW);
      shiftOut(datapinN, clockpinN, MSBFIRST, numArray[guessNum%10]);
      shiftOut(datapinN, clockpinN, MSBFIRST, numArray[guessNum/10]);
      digitalWrite(latchpinN, HIGH);
    }
  }
}
