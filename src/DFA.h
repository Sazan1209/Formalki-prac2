//
// Created by mikhail on 12.10.22.
//

#ifndef FORMALKI_SRC_DFA_H_
#define FORMALKI_SRC_DFA_H_

#include <vector>
#include <string>
#include <unordered_map>
#include "Bitset.h"
#include "globals.h"
#include "utility.h"
#include <queue>

class NFA;

class DFA {
 public:
  size_t Size();
  DFA() = default;
  explicit DFA(const NFA&);
  bool ReadWord(const std::string& word);
  void Minimize();
  void DFS(Bitset& visited, size_t state);

 private:
  struct State{
    State();
    size_t& operator[](size_t pos);
    std::vector<size_t> moves;
  };
  std::vector<State> states_;
  Bitset accepting_states_;
  void Prune();
  void ClassesInit(std::vector<size_t>& classes);
  void SeparateClasses(std::vector<size_t>& classes);
  void MakeMDFA(std::vector<size_t>& classes);
};

#include "NFA.h" //Спасибо c++ за наше счастливое детство

#endif //FORMALKI_SRC_DFA_H_
