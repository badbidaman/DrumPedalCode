const int thresholdMin = 12;  // minimum reading, avoid "noise"
const int aftershockMillis = 60; // time of aftershocks & vibration
int state=0; // 0=idle, 1=looking for peak, 2=ignore aftershocks
int peak;    // remember the highest reading
elapsedMillis msec; // timer to end states 1 and 2
void setup() {
  Serial.begin(115200);
  //pinMode(A0, INPUT_DISABLE);
  while (!Serial && millis() < 2500) /* wait for serial monitor */ ;
  Serial.println("Piezo Peak Capture");
}
void loop() {
  int piezo = analogRead(A0);
  if (state == 0) {
    // IDLE state: if any reading is above a threshold, begin peak
    if (piezo > thresholdMin) {
      //Serial.println("begin state 1");
      state = 1;
      peak = piezo;
      msec = 0;
    }
  } else if (state == 1) {
    // Peak Tracking state: for 10 ms, capture largest reading
    if (piezo > peak) {
      peak = piezo;
    }
    if (msec >= 10) {
      Serial.print("peak = ");
      Serial.println(peak);
      //Serial.println("begin state 2");
      state = 2;
      msec = 0;
    }
  } else {
    // Ignore Aftershock state: wait for things to be quiet again
    if (piezo > thresholdMin) {
      msec = 0; // keep resetting timer if above threshold
    } else if (msec > 30) {
      //Serial.println("begin state 0");
      state = 0; // go back to idle after 30 ms below threshold
    }
  }
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
