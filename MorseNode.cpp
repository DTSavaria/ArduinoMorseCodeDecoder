
#include "MorseNode.hpp"

//------------------------------------------------------------------------------

MorseNode::MorseNode() 
: MorseNode('\0', nullptr, nullptr) {
}

//------------------------------------------------------------------------------

MorseNode::MorseNode(MorseNode * dot, MorseNode * dash)
: MorseNode('\0', dot, dash) {
}

//------------------------------------------------------------------------------

MorseNode::MorseNode(char character)
: MorseNode(character, nullptr, nullptr) {
}

//------------------------------------------------------------------------------

MorseNode::MorseNode(char character, MorseNode * dot, MorseNode * dash)
: character(character),
  dot(dot),
  dash(dash) {
}

//------------------------------------------------------------------------------

char MorseNode::getCharacter() const {
  return character;
}

//------------------------------------------------------------------------------

MorseNode * MorseNode::getDot() const {
  return dot;
}

//------------------------------------------------------------------------------

MorseNode * MorseNode::getDash() const {
  return dash;
}

//------------------------------------------------------------------------------

char MorseNode::decode(int * symbols, int size) const {
  if (size < 1) {
    return getCharacter();
  } else {
    if (*symbols == DOT && getDot() != nullptr) {
      return getDot()->decode(&symbols[1], size - 1);
    } else if (*symbols == DASH && getDash() != nullptr) {
      return getDash()->decode(&symbols[1], size - 1);
    } else {
      return '\0';
    }
  }
}

//------------------------------------------------------------------------------

void MorseNode::setDot(MorseNode * dot) {
  this->dot = dot;
}

//------------------------------------------------------------------------------

void MorseNode::setDash(MorseNode * dash) {
  this->dash = dash;
}

//------------------------------------------------------------------------------

MorseNode * MorseNode::getInternationalAlphabet() {

  MorseNode * h = new MorseNode('h');
  MorseNode * v = new MorseNode('v');
  MorseNode * s = new MorseNode('s', h, v);
  MorseNode * f = new MorseNode('f');
  MorseNode * u = new MorseNode('u', f, nullptr);
  MorseNode * i = new MorseNode('i', s, u);
  MorseNode * l = new MorseNode('l');
  MorseNode * r = new MorseNode('r', l, nullptr);
  MorseNode * p = new MorseNode('p');
  MorseNode * j = new MorseNode('j');
  MorseNode * w = new MorseNode('w', p, j);
  MorseNode * a = new MorseNode('a', r, w);
  MorseNode * e = new MorseNode('e', i, a);
  MorseNode * b = new MorseNode('b');
  MorseNode * x = new MorseNode('x');
  MorseNode * d = new MorseNode('d', b, x);
  MorseNode * c = new MorseNode('c');
  MorseNode * y = new MorseNode('y');
  MorseNode * k = new MorseNode('k', c, y);
  MorseNode * n = new MorseNode('n', d, k);
  MorseNode * z = new MorseNode('z');
  MorseNode * q = new MorseNode('q');
  MorseNode * g = new MorseNode('g', z, q);
  MorseNode * o = new MorseNode('o');
  MorseNode * m = new MorseNode('m', g, o);
  MorseNode * t = new MorseNode('t', n , m);
  
  MorseNode * root = new MorseNode(e, t);
  return root;
}

//------------------------------------------------------------------------------
