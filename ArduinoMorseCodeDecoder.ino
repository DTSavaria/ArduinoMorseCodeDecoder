/**
 * (c) 2017 Daniel Savaria
 * Released under the MIT license
 */
 
#include "MorseNode.h"

#include "Symbols.h"

//#define TFT_DISPLAY
#ifdef TFT_DISPLAY

#include <Adafruit_ST7735.h> 

const int TFT_CS = 10;
const int TFT_RST = 9;
const int TFT_DC = 8;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#endif

//------------------------------------------------------------------------------

#define ARRAY_SIZE 256

const int LED = LED_BUILTIN;
const int BUTTON = 7;

//------------------------------------------------------------------------------

unsigned long dotMillisMin = 50; //minimum length of a button press for a dot in milliseconds
unsigned long dashMillisMin = 200; //minimum length of a button press for a dash in milliseconds

unsigned long letterSpaceMillisMin = 500; //minimum length of no button press for a letter in milliseconds
unsigned long wordSpaceMillisMin = 1700; //minimum length of no button press for a word space in milliseconds

//------------------------------------------------------------------------------

int inputVal = LOW; //the current input
int previousInputVal = LOW; //the previous input

unsigned long holdStart = 0; //the time the input started
unsigned long currentHold = 0; //the length of time of the current button press in milliseconds
unsigned long previousHold = 0; //the length of time of the previous button press in milliseconds

unsigned long spaceStart = 0; //the time the space started
unsigned long currentSpace = 0; //the length of time of the current space in milliseconds
unsigned long previousSpace = 0; //the length of time of the previous space in milliseconds
boolean spaceProcessed = false;

//------------------------------------------------------------------------------

MorseNode * alphabet;

int inputSymbols[ARRAY_SIZE];
int inputIndex = 0;

//------------------------------------------------------------------------------

int getInput();
void outputCharacter(char character);
void outputString(String string);
int determineDotDash(unsigned long holdMillis);
int determineLetterWordSpace(unsigned long spaceMillis);
String getDotDashOrSpaceString(int input);
String getDotDashOrSpaceSymbolString(int input);

//------------------------------------------------------------------------------

void setup() {
  #ifdef TFT_DISPLAY
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);

  #else
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
  #endif
  
  Serial.begin(9600);

  alphabet = MorseNode::getInternationalAlphabet();
}

//------------------------------------------------------------------------------

void loop() {
  inputVal = getInput();
  
  digitalWrite(LED, inputVal);

  if (inputVal == HIGH && previousInputVal == LOW) {
    holdStart = millis(); //capture the time the button started being pressed
  }

  if (inputVal == LOW && previousInputVal == HIGH) {
    spaceStart = millis(); //capture the time the button stopped being pressed
  }

  if (inputVal == HIGH) {
    currentHold = millis() - holdStart; //if the button is being pressed, see for how long
    currentSpace = 0; //the button is being pressed, reset the space
    spaceProcessed = false;
  } else {
    currentHold = 0; //the button is not being pressed, reset the hold
    currentSpace = millis() - spaceStart; //the button is not being pressed, see for how long
  }

  int result = NOTHING;

  if (currentHold < previousHold) {

    //if the button stopped being pressed, determine if the previous press was a dot or dash
    result = determineDotDash(previousHold);
  } else if (!spaceProcessed && determineLetterWordSpace(currentSpace) == WORD_SPACE) {
    result = WORD_SPACE;
    spaceProcessed = true;
  } else if (!spaceProcessed && currentSpace < previousSpace) {

    //if the space is over, determine if the previous space was a letter or word space
    result = determineLetterWordSpace(previousSpace);
    spaceProcessed = true;
  }

  if (result != NOTHING) {

    if ((result == DOT || result == DASH) && inputIndex < ARRAY_SIZE) {
      
      //if a dot or dash (and we didn't fill up the array) add it to the array
      inputSymbols[inputIndex] = result;
      inputIndex += 1;
    } else {

      //not a dot or dash (or array is full), the the character and print it
      char c = alphabet->decode(&inputSymbols[0], inputIndex);

      if (c != '\0') { //print the character
        outputCharacter(c);
      } else { // couldn't find character, print symbols
        for (int i = 0; i < inputIndex; i++) {
          outputString(getDotDashOrSpaceSymbolString(inputSymbols[i]));
        }
      }

      if (result == WORD_SPACE) {
        outputCharacter(' ');
        spaceProcessed = true;
      }

      inputIndex = 0;
    }
  }

  //set the previous values
  previousInputVal = inputVal;
  previousHold = currentHold;
  previousSpace = currentSpace;
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

int getButtonInput() {
  return (digitalRead(BUTTON) == HIGH) ? LOW : HIGH; //switch high and low because it's a pullup
}

//------------------------------------------------------------------------------

int getJoystickInput()
{
  int joystickState = analogRead(3);
  
  if (joystickState < 650) {
    return HIGH;
  } else {
    return LOW;
  }
}

//------------------------------------------------------------------------------

int getInput() {
  #ifdef TFT_DISPLAY
    return getJoystickInput();
  #else
    return getButtonInput();
  #endif
  
}

//------------------------------------------------------------------------------

void outputCharacter(char character) {
  Serial.print(character);
  
  #ifdef TFT_DISPLAY
    tft.print(character);
  #endif
}

//------------------------------------------------------------------------------

void outputString(String string) {
  Serial.print(string);
  
  #ifdef TFT_DISPLAY
    tft.print(string);
  #endif
}

//------------------------------------------------------------------------------
