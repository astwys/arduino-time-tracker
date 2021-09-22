#include <Servo.h>

int const butPin = 13;
int const ledPin = 12;

int buttonState = LOW;
int previousTrackState = LOW;
int currentTrackState = LOW;

Servo servo;
int const potPin = A0;
int previousAngle;
int currentAngle;

unsigned long startTime = 0;

// work, study, play
unsigned long times[] = {0, 0, 0};
enum Category {
  WORK = 0,
  STUDY = 1,
  PLAY = 2
};
Category previousCategory;
Category currentCategory;

void setup() {
  // put your setup code here, to run once:
  pinMode(butPin, INPUT);
  pinMode(ledPin, OUTPUT);

  servo.attach(3);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // check if time should be tracked
  previousTrackState = currentTrackState;
  currentTrackState = startStop();
  setLED(currentTrackState);

  // reset start time as soon as tracking starts
  if (previousTrackState != currentTrackState && currentTrackState == HIGH) {
    resetStartTime();
  }

  // set visual hardware arrow
  previousAngle = currentAngle;
  currentAngle = getPotAngle();
  int diff = abs(previousAngle - currentAngle);
  if (diff > 2) {
    setServo(currentAngle);
  }

  logTimes();

  // print result when tracking has stopped
  if (previousTrackState != currentTrackState && currentTrackState == LOW) {
    Serial.print("{\"work\":");
    Serial.print((unsigned long) times[0]);
    Serial.print(",\"study\":");
    Serial.print((unsigned long) times[1]);
    Serial.print(",\"play\":");
    Serial.print((unsigned long) times[2]);
    Serial.println("}");
  }

  delay(15);
}

int startStop() {
  int previousButtonState = buttonState;
  buttonState = digitalRead(butPin);

  int newState = currentTrackState;

  if (previousButtonState != buttonState && buttonState == HIGH) {
    if (currentTrackState == LOW) {
      newState = HIGH;
    } else {
      newState = LOW;
    }
  }
  return newState;
}

/**
 * log times when:
 * a. tracking is running and the category changes
 * b. tracking has stopped and the last category time needs to be logged
 */
void logTimes() {
  previousCategory = currentCategory;
  currentCategory = getCategory(currentAngle);
  if ((currentTrackState == HIGH && previousCategory != currentCategory) || (currentTrackState == LOW && previousTrackState != currentTrackState)) {
    setCategoryTimes(previousCategory);
    resetStartTime();
  }
}

void setCategoryTimes(Category category) {
  unsigned long timeSpent = millis() - startTime;
  int currCat = (int) category;
  times[currCat] = times[currCat] + timeSpent;
}



Category getCategory(int angle) {
  Category cat = WORK;
  if (0 <= angle && angle < 70) {
    cat = PLAY;
  } else if (70 <= angle && angle < 110) {
    cat = STUDY;
  } else if (110 <= angle && angle < 180) {
    cat = WORK;
  }

  return cat;
}

int getPotAngle() {
  int potVal = analogRead(potPin);

  int angle = map(potVal, 0, 1023, 0, 179);
  return angle;
}

void setServo(int angle) {
  servo.write(angle);
}

void setLED(int state) {
  digitalWrite(ledPin, state);
}

void resetStartTime() {
  startTime = millis();
}
