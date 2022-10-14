//
// Created by mikhail on 12.10.22.
//

#include "DFA.h"
size_t& DFA::State::operator[](size_t pos) { return moves[pos]; }
DFA::State::State() : moves(kSigmaSize){ }

void DFA::Minimize() {
  size_t accepting_states_num = accepting_states_.BitCount();
  if (accepting_states_num == 0 || accepting_states_num == states_.size()) {
    accepting_states_.Resize(1);
    states_.resize(1);
    for (auto& curr : states_[0].moves) {
      curr = 0;
    }
    if (accepting_states_num != 0) {
      accepting_states_.Set(0);
    }
    return;
  }
  std::vector<size_t> classes(states_.size());
  ClassesInit(classes);
  SeparateClasses(classes);
  MakeMDFA(classes);
  Prune();
}

void DFA::ClassesInit(std::vector<size_t>& classes) {
  size_t first_accept = states_.size();
  size_t first_non_accept = states_.size();
  for (size_t i = 0; i < states_.size(); ++i) {
    if (accepting_states_[i]) {
      first_accept = std::min(first_accept, i);
      classes[i] = first_accept;
    } else {
      first_non_accept = std::min(first_non_accept, i);
      classes[i] = first_non_accept;
    }
  }
}

struct ClassId {
  size_t from_class;
  size_t to_class;
  bool operator==(const ClassId& class_id) const {
    return from_class == class_id.from_class && to_class == class_id.to_class;
  }
};

struct ClassIdHash {
  size_t operator()(const ClassId& state_id) const {
    return (state_id.from_class << 1) ^ state_id.to_class;
  }
};

void DFA::SeparateClasses(std::vector<size_t>& classes) {
  bool flag = true;
  size_t classes_num = 2;
  std::vector<size_t> new_classes(states_.size());
  while (flag) {
    flag = false;
    for (size_t letter = 0; letter < kSigmaSize; ++letter) {
      std::unordered_map<ClassId, size_t, ClassIdHash> new_classes_id;
      for (size_t curr_state = 0; curr_state < states_.size(); ++curr_state) {
        size_t next_state = states_[curr_state][letter];
        ClassId class_id{classes[curr_state], classes[next_state]};
        if (new_classes_id.find(class_id) == new_classes_id.end()) {
          new_classes_id[class_id] = new_classes[curr_state] = curr_state;
        } else {
          new_classes[curr_state] = new_classes_id[class_id];
        }
      }
      classes = new_classes;
      if (new_classes_id.size() != classes_num) {
        flag = true;
        classes_num = new_classes_id.size();
      }
    }
  }
}

void DFA::MakeMDFA(std::vector<size_t>& classes) {
  std::unordered_map<size_t, size_t> re_num;
  size_t count = 0;
  for (size_t i = 0; i < classes.size(); ++i) {
    if (classes[i] != i) {
      continue;
    }
    if (i != count) {
      states_[count] = std::move(states_[i]);
    }
    re_num[i] = count;
    ++count;
  }
  Bitset new_accepting_states(count);
  for (auto state_pair : re_num) {
    if (accepting_states_[state_pair.first]) {
      new_accepting_states.Set(state_pair.second);
    }
  }
  accepting_states_ = std::move(new_accepting_states);
  states_.resize(count);
  for (auto& state : states_) {
    for (size_t j = 0; j < kSigmaSize; ++j) {
      auto next = state[j];
      state[j] = re_num[classes[next]];
    }
  }
}

bool DFA::ReadWord(const std::string& word) {
  size_t state = 0;
  for (auto letter : word){
    state = states_[state][LetterToNum(letter)];
  }
  return accepting_states_[state];
}

void DFA::Prune() {
  Bitset reachable_bitset(states_.size());
  DFS(reachable_bitset, 0);
  auto reachable = reachable_bitset.GetVals();
  if (reachable.size() == states_.size()){
    return;
  }
  std::unordered_map<size_t, size_t> re_num;
  Bitset new_accepting(reachable.size());
  for (size_t i = 0; i < reachable.size(); ++i){
    if (accepting_states_[reachable[i]]){
      new_accepting.Set(i);
    }
    if (reachable[i] == i){
      continue;
    }
    states_[i] = std::move(states_[reachable[i]]);
    re_num[reachable[i]] = i;
  }
  states_.resize(reachable.size());
  accepting_states_ = new_accepting;
  for (auto &state : states_){
    for (auto &curr : state.moves){
      curr = re_num[curr];
    }
  }
}

void DFA::DFS(Bitset& visited, size_t state) {
  if (visited[state]){
    return;
  }
  visited.Set(state);
  for (auto next : states_[state].moves){
    DFS(visited, next);
  }
}

DFA::DFA(const NFA& nfa){
  std::unordered_map<Bitset, size_t, BitsetHash> visited;
  std::queue<Bitset> queue;
  Bitset start(nfa.states_.size());
  start.Set(0);
  visited[start] = 0;
  queue.push(std::move(start));
  size_t count = 0;
  while (!queue.empty()){
    states_.emplace_back();
    Bitset& curr_bitset = queue.front();
    auto curr = curr_bitset.GetVals();
    for (size_t letter = 0; letter < kSigmaSize; ++letter){
      Bitset res(nfa.states_.size());
      for (auto state : curr){
        res |= nfa.states_[state].moves_bitset[letter];
      }
      if (visited.find(res) == visited.end()){
        states_[states_.size() - 1][letter] = visited[res] = ++count;
        queue.push(std::move(res));
      } else{
        states_[states_.size() - 1][letter] = visited[res];
      }
    }
    queue.pop();
  }
  accepting_states_.Resize(states_.size());
  for (auto& curr : visited){
    if (curr.first && nfa.accepting_states_){
      accepting_states_.Set(curr.second);
    }
  }
}

size_t DFA::Size() { return states_.size(); }
