#include "NFA.h"
#include "DFA.h"
#include <iostream>
#include "../Google_test/testing_uncs.cpp"

int main(){
  NFA nfa;
  std::cin >> nfa;
  nfa.RemoveEps();
  DFA dfa(nfa);
  dfa.Minimize();
  std::string res = dfa.GetRegex();
  std::cout << res << '\n';
  res = "(" + res + ")";
  std::cout << ParseRegex(res, "babbbbbbbbb") << dfa.ReadWord("babbbbbbbbb");
}