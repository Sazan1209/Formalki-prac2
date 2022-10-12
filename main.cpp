#include "NFA.h"
#include <iostream>

int main(){
  NFA nfa;
  std::cin >> nfa;
  NFA nfa_without_eps(nfa);
  nfa_without_eps.RemoveEps();
  size_t word_num;
  std::cin >> word_num;
  std::string word;
  for (;word_num;--word_num){
    std::cin >> word;
    std::cout << nfa.ReadWord(word) << ' ' << nfa_without_eps.ReadWord(word) << std::endl;
  }
}