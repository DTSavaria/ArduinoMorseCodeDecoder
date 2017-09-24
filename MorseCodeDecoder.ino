/**
 * (c) 2017 Daniel Savaria
 * Released under the MIT license
 */

const int LED = LED_BUILTIN;
const int BUTTON = 7;

int inputVal = LOW;
int previousInputVal = LOW;
unsigned long holdStart = 0;
unsigned long currentHold = 0;
unsigned long previousHold = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {
  inputVal = (digitalRead(BUTTON) == HIGH) ? LOW : HIGH; //switch high and low because it's a pullup
  digitalWrite(LED, inputVal);

  if (inputVal == HIGH && previousInputVal == LOW) {
    holdStart = millis();
  }

  if (inputVal == HIGH) {
    currentHold = millis() - holdStart;
  } else {
    currentHold = 0;
  }

  if (currentHold < previousHold) {
    Serial.println(previousHold);
  }

  previousInputVal = inputVal;
  previousHold = currentHold;
}
