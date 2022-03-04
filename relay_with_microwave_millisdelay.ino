#include <millisDelay.h>

const unsigned long motionSenseDurationSeconds = 180;
const int greenMinSeconds = 90;
const int greenMaxSeconds = 160;
const int redMinSeconds = 45;
const int redMaxSeconds = 80;
const int yellowSeconds = 4;

const int microwaveIn = 2; // Define the interrupt PIN is 0, that is, digital pins 2
const int green = 3;
const int yellow = 4;
const int red = 5;
const int brakes = 6;

volatile bool restartMicrowave = false;

millisDelay microwaveActive;
millisDelay greenActive;
millisDelay yellowActive;
millisDelay redActive;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup:");
  
  pinMode(microwaveIn, INPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(brakes, OUTPUT);  

  randomSeed(analogRead(0));

  microwaveActive.start(motionSenseDurationSeconds * 1000);  

  attachInterrupt(digitalPinToInterrupt(microwaveIn), stateChange, FALLING); // Sets the interrupt function, falling edge triggered interrupts.
}

void loop() {
  if (restartMicrowave) {
      restartMicrowave = false;
      Serial.println("interrupt:");
      Serial.println(microwaveActive.remaining());
      microwaveActive.restart();
  }
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

void stateChange() //Interrupt function
{
  restartMicrowave = true;
}
