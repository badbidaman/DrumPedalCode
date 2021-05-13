//This code controls the Beata lights.  
const int yellow = 2; // led connected to pin 2.
const int white = 3;  // led connected to pin 3.
const int green = 4;  // led connected to pin 4.
const int blue = 5;  // led connected to pin 5.
const int killSwitch = 6; //+ wire of on/off switch to pin 6.
const int rightPiezo = A10;  // right piezo on analog pin 24(A10)
const int leftPiezo = A11;  // left piezo on analog pin 25(A11)

int rightsensorReading = 0;
int leftsensorReading = 0;
int ledState = LOW;

const int threshold = 10; //changes the sensitivity of the piezos (1-100)

bool shouldRunPattern1 = true;
bool shouldRunPattern2 = true;
long pattern1Start = 0;
long pattern2Start = 0;

void setup() {
  pinMode(yellow, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}
void loop () 

/* Code for On/Off switch to be worked on in Future 
 *  { if (digitalRead(killSwitch) == LOW) 
  return;*/

  // read sensors and set variables
   {
   rightsensorReading = analogRead(rightPiezo);
   leftsensorReading = analogRead(leftPiezo);
  
  // initialize the pattern1
  if (rightsensorReading >= threshold) {
    shouldRunPattern1 = true;
    pattern1Start = millis();
  }
  
  // initialize the pattern2
  if (leftsensorReading >= threshold) {
    shouldRunPattern2 = true;
    pattern2Start = millis();
  }
  // how long it's been since pattern1 was started
  long pattern1Duration = millis() - pattern1Start;
  
  // how long it's been since pattern2 was started
  long pattern2Duration = millis() - pattern2Start;
  // change this to change how long the LEDs stay on
  long patternPulseTime = 200; // .2 seconds
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
}
