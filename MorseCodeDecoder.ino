/**
 * (c) 2017 Daniel Savaria
 * Released under the MIT license
 */

const int LED = LED_BUILTIN;
const int BUTTON = 7;

const int NOTHING = 0; //represents no input
const int DOT = 1; //represent dot input
const int DASH = 2; //represent dash input

unsigned long dotMillisMin = 50; //minimum length of a button press for a dot in milliseconds
unsigned long dashMillisMin = 200; //minimum length of a button press for a dash in milliseconds

int inputVal = LOW; //the current input
int previousInputVal = LOW; //the previous input
unsigned long holdStart = 0; //the time the input started
unsigned long currentHold = 0; //the length of time of the current button press in milliseconds
unsigned long previousHold = 0; //the length of time of the previous button press in milliseconds

int determineDotDash(unsigned long holdMillis);
String getDotDashString(int input);

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  inputVal = (digitalRead(BUTTON) == HIGH) ? LOW : HIGH; //switch high and low because it's a pullup
  digitalWrite(LED, inputVal);

  if (inputVal == HIGH && previousInputVal == LOW) {
    holdStart = millis(); //capture the time the button started being pressed
  }

  if (inputVal == HIGH) {
    currentHold = millis() - holdStart; //if the button is being pressed, see for how long
  } else {
    currentHold = 0; //the button is not being pressed, reset the hold
  }

  if (currentHold < previousHold) {

    //if the button stopped being pressed, determine if the previous press was a dot or dash
    int result = determineDotDash(previousHold);

    //print the results for now
    if (result == DOT || DASH) {
      Serial.println(getDotDashString(result) + "(" + previousHold + ")");
    }
  }

  //set the previous values
  previousInputVal = inputVal;
  previousHold = currentHold;
}

/**
 * Given the length of a button press in milliseconds, return DOT, DASH, or NOTHING
 */
int determineDotDash(unsigned long holdMillis) {
  int toReturn = NOTHING;
  if (previousHold >= dashMillisMin) {
    toReturn = DASH;
  } else if (previousHold >= dotMillisMin) {
    toReturn = DOT;
  }
  return toReturn;
}

/**
 * Given DOT, DASH, or NOTHING, return a string representing it
 */
String getDotDashString(int input) {
  String toReturn = "nothing";
  if (input == DOT) {
    toReturn = "dot";
  } else if (input == DASH) {
    toReturn = "dash";
  }
  return toReturn;
}

