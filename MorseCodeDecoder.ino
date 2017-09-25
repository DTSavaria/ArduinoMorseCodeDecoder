/**
 * (c) 2017 Daniel Savaria
 * Released under the MIT license
 */

const int LED = LED_BUILTIN;
const int BUTTON = 7;

const int NOTHING = 0; //represents no input
const int DOT = 1; //represent dot input
const int DASH = 2; //represent dash input
const int LETTER_SPACE = 3; //represent letter space
const int WORD_SPACE = 4; //represent word space

unsigned long dotMillisMin = 50; //minimum length of a button press for a dot in milliseconds
unsigned long dashMillisMin = 200; //minimum length of a button press for a dash in milliseconds

unsigned long letterSpaceMillisMin = 500; //minimum length of no button press for a letter in milliseconds
unsigned long wordSpaceMillisMin = 1700; //minimum length of no button press for a word space in milliseconds

int inputVal = LOW; //the current input
int previousInputVal = LOW; //the previous input

unsigned long holdStart = 0; //the time the input started
unsigned long currentHold = 0; //the length of time of the current button press in milliseconds
unsigned long previousHold = 0; //the length of time of the previous button press in milliseconds

unsigned long spaceStart = 0; //the time the space started
unsigned long currentSpace = 0; //the length of time of the current space in milliseconds
unsigned long previousSpace = 0; //the length of time of the previous space in milliseconds

int determineDotDash(unsigned long holdMillis);
int determineLetterWordSpace(unsigned long spaceMillis);
String getDotDashOrSpaceString(int input);
String getDotDashOrSpaceSymbolString(int input);

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

  if (inputVal == LOW && previousInputVal == HIGH) {
    spaceStart = millis();
  }

  if (inputVal == HIGH) {
    currentHold = millis() - holdStart; //if the button is being pressed, see for how long
    currentSpace = 0; //the button is being pressed, reset the space
  } else {
    currentHold = 0; //the button is not being pressed, reset the hold
    currentSpace = millis() - spaceStart; //the button is not being pressed, see for how long
  }

  int result = NOTHING;

  if (currentHold < previousHold) {

    //if the button stopped being pressed, determine if the previous press was a dot or dash
    result = determineDotDash(previousHold);
  } else if (currentSpace < previousSpace) {

    //if the space is over, determine if the previous space was a letter or word space
    result = determineLetterWordSpace(previousSpace);
  }

  if (result != NOTHING) {
    //print the results for now
    Serial.print(getDotDashOrSpaceSymbolString(result));
    //Serial.println(getDotDashOrSpaceString(result) + "(" + previousHold + ", " + previousSpace + ")");
  }

  //set the previous values
  previousInputVal = inputVal;
  previousHold = currentHold;
  previousSpace = currentSpace;
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
 * Given the length of a space in milliseconds, return LETTER_SPACE, WORD_SPACE, or NOTHING
 */
int determineLetterWordSpace(unsigned long spaceMillis) {
  int toReturn = NOTHING;
  if (previousSpace >= wordSpaceMillisMin) {
    toReturn = WORD_SPACE;
  } else if (previousSpace >= letterSpaceMillisMin) {
    toReturn = LETTER_SPACE;
  }
  return toReturn;
}

/**
 * Given DOT, DASH, LETTER_SPACE, WORD_SPACE, or NOTHING, return a string representing it
 */
String getDotDashOrSpaceString(int input) {
  String toReturn = "nothing";
  if (input == DOT) {
    toReturn = "dot";
  } else if (input == DASH) {
    toReturn = "dash";
  } else if (input == LETTER_SPACE) {
    toReturn = "end of letter";
  } else if (input == WORD_SPACE) {
    toReturn = "end of word";
  }
  return toReturn;
}

/**
 * Given DOT, DASH, LETTER_SPACE, WORD_SPACE, or NOTHING, return a string with a symbol
 */
String getDotDashOrSpaceSymbolString(int input) {
  String toReturn = "";
  if (input == DOT) {
    toReturn = "·";
  } else if (input == DASH) {
    toReturn = "-";
  } else if (input == LETTER_SPACE) {
    toReturn = " ";
  } else if (input == WORD_SPACE) {
    toReturn = " | ";
  }
  return toReturn;
}
