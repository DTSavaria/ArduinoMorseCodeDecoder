
#ifndef MORSENODE_H
#define MORSENODE_H

#include "Symbols.h"

class MorseNode {
  public:
  
    MorseNode();
    MorseNode(MorseNode * dot, MorseNode * dash);
    MorseNode(char character);
    MorseNode(char character, MorseNode * dot, MorseNode * dash);

    static MorseNode * getInternationalAlphabet();
    
    char getCharacter() const;
    MorseNode * getDot() const;
    MorseNode * getDash() const;

    char decode(int * symbols, int size) const;

    void setDot(MorseNode * dot);
    void setDash(MorseNode * dash);
    
  private:
  
    const char character;
    MorseNode * dot;
    MorseNode * dash;
}; 

#endif
