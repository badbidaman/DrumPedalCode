#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

/*This initial setup is important because I have added 2 mixers. 
This means that playSdWav1 and playSDWav2 can be played at the same time*/

AudioPlaySdWav           playSdWav1 = rightPiezoSound;     
AudioPlaySdWav           playSdWav2 = leftPiezoSound;     
AudioMixer4              mixer1;         
AudioMixer4              mixer2;         
AudioOutputI2S           i2s1;           
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord3(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord4(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     

// lights
const int yellow = 2;  // led connected to pin 2.
const int white = 3;   // led connected to pin 3.
const int green = 4;   // led connected to pin 4.
const int blue = 5;    // led connected to pin 5.
const int killSwitch = 6;
const int rightPiezo = A1;  // right piezo on analog pin 1
const int leftPiezo = A2;   // left piezo on analog pin 2
int rightsensorReading = 0;
int leftsensorReading = 0;
int ledState = LOW;

const int threshold = 10;  /*do we need this if we're using the Peak capture method? */

bool shouldRunPattern1 = true;
bool shouldRunPattern2 = true;

bool shouldPlaySound1 = false;
bool shouldPlaySound2 = false;
long pattern1Start = 0;
long pattern2Start = 0;

const String rightPiezoSound = "RKIT.WAV";
const String leftPiezoSound = "LKIT.WAV";


void setup() {
  pinMode(yellow, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

}

void loop() {
  
  /* volume control, #s are somewhat subjective, 
  will need adjusting once pedal is built*/
  float volume = 0.0;
  if (piezo < 10) {
    volume = 0.0;
  } else if (piezo <= 320) {
    volume = 0.4; 
  } else if (piezo <= 700) {
    volume = 0.7; 
  } else {
    volume = 1.0; 
  } 
}

/*if (digitalRead(killSwitch) == LOW) {
    return; } */

  // read sensors and set variables
  rightsensorReading = analogRead(rightPiezo);
  leftsensorReading = analogRead(leftPiezo);
  
  // initialize the pattern1
  if (rightsensorReading >= threshold) {
    shouldRunPattern1 = true;
    shouldPlaySound1 = true;
    pattern1Start = millis();
  }
  // initialize the pattern2
  if (leftsensorReading >= threshold) {
    shouldRunPattern2 = true;
    shouldPlaySound2 = true;
    pattern2Start = millis();
  }
  // how long it's been since pattern1 was started
  long pattern1Duration = millis() - pattern1Start;
  // how long it's been since pattern2 was started
  long pattern2Duration = millis() - pattern2Start;
  // change this to change how long the LEDs stay on
  long patternPulseTime = 200;  // .2 seconds
  // run pattern 1
  if (shouldRunPattern1) {
    // green and blue on
    if (pattern1Duration < patternPulseTime) {
      digitalWrite(green, HIGH);
      digitalWrite(blue, HIGH);
    }
    // green and blue off for second half second
    else {
      digitalWrite(green, LOW);
      digitalWrite(blue, LOW);
      shouldRunPattern1 = false;
    }
  }
  // run pattern 2 (same as pattern1 just different leds)
  if (shouldRunPattern2) {
    if (pattern2Duration < patternPulseTime) {
      digitalWrite(yellow, HIGH);
      digitalWrite(white, HIGH);
    } else {
      digitalWrite(yellow, LOW);
      digitalWrite(white, LOW);
      shouldRunPattern2 = false;
    }
  }
  if (shouldPlaySound1) {
    DF1201S.playSpecFile(rightPiezoSound);
    Serial.print("Playing right sound: ");
    Serial.println(DF1201S.getFileName());
    shouldPlaySound1 = false;
  }
  if (shouldPlaySound2) {
    DF1201S.playSpecFile(leftPiezoSound);
    Serial.print("Playing left sound: ");
    Serial.println(DF1201S.getFileName());
    shouldPlaySound2 = false;
  }
}
