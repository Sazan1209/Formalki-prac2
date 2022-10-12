//
// Created by mikhail on 09.10.22.
//

#include "NFA.h"

enum {
  kNotVisited = -1
};

void NFA::RemoveEps() {
  Bitset visited(states_.size());
  for (size_t i = 0; i < states_.size(); ++i) {
    visited.Clear();
    ContractMoves(i, visited, i);
  }
  Prune();
}

Bitset NFA::ContractMoves(size_t state_num, Bitset& visited, size_t begin) {
  Bitset res(states_.size());
  res.Set(state_num);
  if (visited[state_num]) {
    return res;
  }
  visited.Set(state_num);
  auto& curr_state = states_[state_num];
  if (curr_state.moves.size() == kSigmaSize) { // doesn't have eps-transitions
    return res;
  }
  bool can_contract_self = true;
  for (auto next : curr_state.moves[kEps]) {
    res |= ContractMoves(next, visited, begin);
    if (states_[next].moves.size() != kSigmaSize) {
      can_contract_self = false;
    }
  }
  if (can_contract_self || state_num == begin) {
    ContractState(state_num, res);
    res.Clear();
    res.Set(state_num);
  }
  return res;
}

void NFA::ContractState(size_t state_num, Bitset& reachable) {
  auto reachable_states = std::move(reachable.GetVals());
  auto& curr_state = states_[state_num];
  for (size_t letter = 0; letter < kSigmaSize; ++letter) {
    for (auto next : reachable_states) {
      curr_state.moves_bitset[letter] |=
          states_[next].moves_bitset[letter];
    }
    curr_state.moves[letter] =
        std::move(states_[state_num].moves_bitset[letter].GetVals());
  }
  for (auto next : reachable_states) {
    if (accepting_states_[next]) {
      accepting_states_.Set(state_num);
      break;
    }
  }
  states_[state_num].moves.pop_back();
}

std::istream& operator>>(std::istream& input, NFA& self) {
  size_t states_num;
  size_t moves_num;
  size_t f_size;
  input >> states_num >> moves_num >> f_size;
  self = std::move(NFA(states_num));
  for (size_t i = 0; i < moves_num; ++i) {
    size_t from;
    size_t to;
    char letter;
    input >> letter >> from >> to;

    self.states_[from].moves[LetterToNum(letter)].push_back(to);
    self.states_[from].moves_bitset[LetterToNum(letter)].Set(to);
  }
  for (size_t i = 0; i < f_size; ++i) {
    size_t state;
    input >> state;
    self.accepting_states_.Set(state);
  }
  self.Prune();
  return input;
}

std::ostream& operator<<(std::ostream& output, NFA& self) {
  for (size_t i = 0; i < self.states_.size(); ++i) {
    output << "State " << i << '\n';
    for (size_t letter = 0; letter < self.states_[i].moves.size(); ++letter) {
      if (self.states_[i].moves[letter].empty()) {
        continue;
      }
      output << NumToLetter(letter) << ": ";
      for (auto next : self.states_[i].moves[letter]) {
        output << next << ' ';
      }
      output << '\n';
    }
  }
  auto accepting_states = std::move(self.accepting_states_.GetVals());
  output << "F: ";
  for (auto curr : accepting_states) {
    output << curr << ' ';
  }
  output << '\n';
  return output;
}

void NFA::DFS(size_t state_num, Bitset& visited) {
  if (visited[state_num]) {
    return;
  }
  visited.Set(state_num);
  auto& curr = states_[state_num];
  for (auto& row : curr.moves) {
    for (auto next : row) {
      DFS(next, visited);
    }
  }
}

NFA& NFA::operator=(NFA&& nfa) noexcept {
  states_ = std::move(nfa.states_);
  accepting_states_ = std::move(nfa.accepting_states_);
  return *this;
}

NFA::NFA(size_t size) : accepting_states_(size) {
  states_.reserve(size);
  for (size_t i = 0; i < size; ++i) {
    states_.emplace_back(size);
  }
}

void NFA::Prune() {
  Bitset reachable_bitset(states_.size());
  DFS(0, reachable_bitset);
  auto reachable = reachable_bitset.GetVals();
  Bitset new_accepting(reachable.size());
  std::unordered_map<size_t, size_t> re_num;
  for (size_t i = 0; i < reachable.size(); ++i) {
    // reachable is in sorted order, which means we never overwrite necessary data
    re_num[reachable[i]] = i;
    if (accepting_states_[reachable[i]]) {
      new_accepting.Set(reachable[i]);
    }
    if (reachable[i] != i) {
      states_[i] = std::move(states_[reachable[i]]);
    }
  }
  accepting_states_ = std::move(new_accepting);
  states_.erase(states_.begin() + reachable.size(), states_.end());
  has_eps = false;
  for (auto& state : states_) {
    for (size_t letter = 0; letter < state.moves.size(); ++letter) {
      auto& curr_bitset = state.moves_bitset[letter];
      auto& curr_moves = state.moves[letter];
      curr_bitset.Resize(reachable.size());
      for (auto& curr_move : curr_moves) {
        curr_move = re_num[curr_move];
        curr_bitset.Set(curr_move);
      }
    }
    if (state.moves.size() == kSigmaSize + 1) {
      if (state.moves[kEps].empty()) {
        state.moves.pop_back();
        state.moves_bitset.pop_back();
      } else {
        has_eps = true;
      }
    }
  }
}

bool NFA::ReadWord(const std::string& word) {
  if (has_eps) {
    std::unordered_set<EpsState, EpsStateHash> visited;
    return ReadWordEps(word, 0, 0, visited);
  }
  return ReadWordNonEps(word, 0, 0);
}

bool NFA::ReadWordNonEps(const std::string& word, size_t pos,
                         size_t state_num) {
  if (pos == word.size()) {
    return accepting_states_[state_num];
  }
  auto letter = LetterToNum(word[pos]);
  for (auto next : states_[state_num].moves[letter]) {
    if (ReadWordNonEps(word, pos + 1, next)) {
      return true;
    }
  }
  return false;
}
bool NFA::ReadWordEps(const std::string& word, size_t pos, size_t state_num,
                      std::unordered_set<NFA::EpsState,
                                         NFA::EpsStateHash>& visited) {
  if (pos == word.size()) {
    return accepting_states_[state_num];
  }
  if (visited.find({pos, state_num}) != visited.end()) {
    return false;
  }
  visited.insert({pos, state_num});
  auto letter = LetterToNum(word[pos]);
  for (auto next : states_[state_num].moves[letter]) {
    if (ReadWordEps(word, pos + 1, next, visited)) {
      return true;
    }
  }
  if (states_[state_num].moves.size() == kSigmaSize + 1) {
    for (auto next : states_[state_num].moves[kEps]) {
      if (ReadWordEps(word, pos, next, visited)) {
        return true;
      }
    }
  }
  return false;
}

NFA::State::State(size_t size) : moves_bitset(kSigmaSize + 1),
                                 moves(kSigmaSize + 1) {
  for (auto& curr : moves_bitset) {
    curr.Resize(size);
  }
}

NFA::State::State(NFA::State&& state) noexcept:
    moves_bitset(std::move(state.moves_bitset)),
    moves(std::move(state.moves)) {}

NFA::State& NFA::State::operator=(NFA::State&& state) noexcept {
  moves = std::move(state.moves);
  moves_bitset = std::move(state.moves_bitset);
  return *this;
}

size_t NFA::EpsStateHash::operator()(const NFA::EpsState& eps_state) const {
  return eps_state.state_num ^ eps_state.pos;
}

bool NFA::EpsState::operator==(const NFA::EpsState& eps_state) const {
  return pos == eps_state.pos && state_num == eps_state.state_num;
}
