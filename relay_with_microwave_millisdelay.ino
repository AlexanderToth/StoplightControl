//#include <MsTimer2.h> //Timer interrupt function
#include <millisDelay.h>

//unsigned long lastDetected = 0;
volatile int count = 0;
//volatile int state = LOW; //Define ledOut, default is off


const unsigned long microwaveWarmupPeriod = 1000; //milliseconds
const unsigned long durationFromLastDetected = 20000; //milliseconds

const int ledOut = 13;
const int microwaveIn = 2; // Define the interrupt PIN is 0, that is, digital pins 2
const int green = 5;
const int yellow = 6;
const int red = 7;
const int brakes = 8;

millisDelay microwaveWarmup;
millisDelay microwaveActive;
millisDelay greenActive;
millisDelay yellowActive;
millisDelay redActive;
//millisDelay pollCount;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup:");
  pinMode(ledOut, OUTPUT);
//  microwaveWarmup.start(microwaveWarmupPeriod);
  microwaveActive.start(durationFromLastDetected);
//  microwaveActive.stop();
  pinMode(microwaveIn, INPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(brakes, OUTPUT);


  //pollCount.start(3000);
  attachInterrupt(digitalPinToInterrupt(microwaveIn), stateChange, FALLING); // Sets the interrupt function, falling edge triggered interrupts.
}

void loop() {
//  if (pollCount.justFinished()) {
//    Serial.println(count);
//    Serial.println(digitalRead(microwaveIn));
//    pollCount.repeat();
//  }
//    digitalWrite(ledOut, HIGH);

  if (microwaveActive.justFinished()) {
    Serial.println("Microwave just finished");
    reset();
  }
  else if (microwaveActive.isRunning()) {
    if (redActive.justFinished()) {
      Serial.println("Red Finished");
      reset();
      startGreen();
    } else if (!lightsOn()) {
      Serial.println("No lights on, start green");
      startGreen();
    }
    else if (greenActive.justFinished()) {
      Serial.println("Green done, start yellow");
      digitalWrite(green, LOW);
      startYellow();
    }
    else if (yellowActive.justFinished()) {
      digitalWrite(yellow, LOW);
      startRed();
    }
  }
  //Serial.println(digitalRead(microwaveIn));
}

void startGreen() {
    Serial.println("green HIGH:");
    digitalWrite(green, HIGH);
    greenActive.start(10000);
}

void startYellow() {
    Serial.println("yellow HIGH:");
    digitalWrite(yellow, HIGH);
    yellowActive.start(2000);
}

void startRed() {
    Serial.println("yellow HIGH:");
    digitalWrite(red, HIGH);
    digitalWrite(brakes, HIGH);
    redActive.start(5000);
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

void stateChange() //Interrupt function
{
  ///count++;
  Serial.println("interrupt:");
  //if (!microwaveWarmup.isRunning()) {
  //  Serial.println("restart:");
  microwaveActive.start(durationFromLastDetected);
  //}
}
