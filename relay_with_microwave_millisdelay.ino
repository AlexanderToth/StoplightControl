#include <millisDelay.h>

const int greenMinSeconds = 60;
const int greenMaxSeconds = 130;
const int redMinSeconds = 25;
const int redMaxSeconds = 60;
const int yellowSeconds = 6;

const int microwaveIn = 2;
const int green = 3;
const int yellow = 4;
const int red = 5;
const int brakes = 6;

volatile bool motionDetected = false;
volatile bool change = false;

millisDelay greenActive;
millisDelay yellowActive;
millisDelay redActive;

//millisDelay stateCheck;


void setup() {
  Serial.begin(9600);
  Serial.println("Setup:");
  
  pinMode(microwaveIn, INPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(brakes, OUTPUT);  

  randomSeed(analogRead(0));

  //stateCheck.start(4000);
  attachInterrupt(digitalPinToInterrupt(microwaveIn), microwaveChangeInterrupt, CHANGE);  
}

void loop() {
  if (change) {
    change = false;
    motionDetected = digitalRead(microwaveIn);    
    if (!motionDetected) {
      reset();
    }
  }
  if (motionDetected) {
    if (redActive.justFinished()) {
      Serial.println("Red Finished");
      //Serial.println(digitalRead(microwaveIn));
      //Serial.println(motionDetected);
      reset();
      startGreen();
    }
    else if (greenActive.justFinished()) {
      Serial.println("Green done, start yellow");
      //Serial.println(digitalRead(microwaveIn));
      //Serial.println(motionDetected);

      digitalWrite(green, LOW);
      startYellow();
    }
    else if (yellowActive.justFinished()) {
      //Serial.println(digitalRead(microwaveIn));
      //Serial.println(motionDetected);
      digitalWrite(yellow, LOW);
      startRed();
    }
    else if (!lightsOn()) {
      Serial.println("No lights on, start green");
      //Serial.println(digitalRead(microwaveIn));      
      //Serial.println(motionDetected);
      startGreen();
    }
    //if (stateCheck.justFinished()) {
    //  Serial.println(digitalRead(microwaveIn));      
    //  Serial.println(motionDetected);
    //  stateCheck.restart();
    //}
  }
}

void startGreen() {
    Serial.println("green HIGH:");
    digitalWrite(green, HIGH);
    greenActive.start(random(greenMinSeconds, greenMaxSeconds) * 1000);
}

void startYellow() {
    Serial.println("yellow HIGH:");
    digitalWrite(yellow, HIGH);
    yellowActive.start(yellowSeconds * 1000);
}

void startRed() {
    Serial.println("red HIGH:");
    digitalWrite(red, HIGH);
    digitalWrite(brakes, HIGH);
    redActive.start(random(redMinSeconds, redMaxSeconds) * 1000);
}

bool lightsOn() {
  return greenActive.isRunning() || yellowActive.isRunning() || redActive.isRunning();
}

void reset() {
   Serial.println("Reset:");
 
    digitalWrite(green, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(brakes, LOW);

    greenActive.stop();
    yellowActive.stop();
    redActive.stop();
}

void microwaveChangeInterrupt() {
  Serial.println("Change");
  Serial.println(digitalRead(microwaveIn));
  change = true;
}
