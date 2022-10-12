//
// Created by mikhail on 09.10.22.
//

#ifndef FORMALKI__NFA_H_
#define FORMALKI__NFA_H_

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include "Bitset.h"
#include <unordered_map>
#include <unordered_set>
#include "globals.h"
#include "utility.h"

class NFA {
 public:
  NFA() = default;
  NFA(const NFA&) = default;
  explicit NFA(size_t size);
  NFA& operator=(NFA&& nfa) noexcept;
  void RemoveEps();
  bool ReadWord(const std::string& word);
  friend std::istream& operator>>(std::istream& input, NFA& self);
  friend std::ostream& operator<<(std::ostream& output, NFA& self);

 private:
  struct State {
    State(const State&) = default;
    explicit State(size_t size);
    State(State&&) noexcept;
    State& operator=(State&&) noexcept;
    std::vector<std::vector<size_t>> moves;
    std::vector<Bitset> moves_bitset;
  };
  struct EpsState {
    EpsState() = default;
    bool operator==(const EpsState&) const;
    size_t pos;
    size_t state_num;
  };
  struct EpsStateHash {
    size_t operator()(const EpsState& eps_state) const;
  };

  bool has_eps = false;
  std::vector<State> states_;
  Bitset accepting_states_;

  void Prune();
  void DFS(size_t state_num, Bitset& visited);
  bool ReadWordEps(const std::string& word, size_t pos, size_t state_num,
                   std::unordered_set<EpsState, EpsStateHash>& visited);
  bool ReadWordNonEps(const std::string& word, size_t pos, size_t state_num);
  Bitset ContractMoves(size_t curr_state, Bitset& visited, size_t begin);
  void ContractState(size_t state_num, Bitset& reachable);
};

#endif //FORMALKI__NFA_H_
