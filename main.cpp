#include "NFA.h"
#include <iostream>

int main(){
  NFA nfa;
  std::cin >> nfa;
  nfa.RemoveEps();
  DFA dfa(nfa);
  std::cout << dfa.ReadWord("aabaaabaaababab");
}