//
// Created by mikhail on 14.10.22.
//

#include "testing_funcs.h"

struct ParseState {
  bool Visited(size_t word_pos, size_t regex_pos, bool skip_plus);
  ParseState(const std::string& regex, const std::string& word);
  const std::string& regex;
  const std::string& word;
  Bitset visited;
  Bitset is_iter;
  std::vector<size_t> next_closing;
  std::vector<size_t> prev_opening;
  std::vector<size_t> next_plus;
};

ParseState::ParseState(const std::string& regex, const std::string& word)
    : regex(regex), word(word), visited(regex.size() * (word.size() + 1) * 2),
      is_iter(regex.size()), next_closing(regex.size(), -1),
      prev_opening(regex.size(), -1), next_plus(regex.size(), -1) {}

bool ParseState::Visited(size_t word_pos, size_t regex_pos, bool skip_plus) {
  if (visited[(word_pos * regex.size() + regex_pos) * 2 + skip_plus]) {
    return true;
  }
  visited.Set((word_pos * regex.size() + regex_pos) * 2 + skip_plus);
  return false;
}

//Заранее прощу прощения

bool ParseRegexRec(ParseState& state, size_t word_pos, size_t regex_pos,
                   bool skip_plus = true) {
  if (state.word.size() <= word_pos && state.regex.size() <= regex_pos) {
    return true;
  }
  if (state.regex.size() <= regex_pos) {
    return false;
  }
  if (state.Visited(word_pos, regex_pos, skip_plus)) {
    return false;
  }
  char curr = state.regex[regex_pos];
  if (curr == '(') {
    if (state.is_iter[regex_pos]) {
      if (ParseRegexRec(state, word_pos, state.next_closing[regex_pos] + 2)) {
        return true;
      }
    }
    if (ParseRegexRec(state, word_pos, regex_pos + 1)) {
      return true;
    }
    if (state.next_plus[regex_pos] != -1) {
      if (ParseRegexRec(state, word_pos, state.next_plus[regex_pos], false)) {
        return true;
      }
    }
    return false;
  }
  if (curr == ')') {
    if (state.is_iter[regex_pos]) {
      return ParseRegexRec(state, word_pos, state.prev_opening[regex_pos]);
    }
    return ParseRegexRec(state, word_pos, regex_pos + 1);
  }
  if (curr == '+') {
    if (skip_plus) {
      return ParseRegexRec(state, word_pos, state.next_closing[regex_pos]);
    }
    if (ParseRegexRec(state, word_pos, regex_pos + 1)) {
      return true;
    }
    if (state.next_plus[regex_pos] != -1) {
      return ParseRegexRec(state, word_pos, state.next_plus[regex_pos], false);
    }
    return false;
  }
  if (curr == '1') {
    return ParseRegexRec(state, word_pos, regex_pos + 1);
  }
  if (state.word.size() <= word_pos){
    return false;
  }
  if (curr == state.word[word_pos]) {
    return ParseRegexRec(state, word_pos + 1, regex_pos + 1);
  }
  return false;
}

bool ParseRegex(const std::string& regex, const std::string& word) {
  ParseState state(regex, word);
  std::stack<size_t> stack;
  for (size_t i = 0; i < regex.size(); ++i) {
    if (regex[i] == '(') {
      stack.push(i);
    }
    state.prev_opening[i] = stack.top();
    if (regex[i] == ')') {
      stack.pop();
    }
  }
  std::stack<size_t> plus_stack;
  plus_stack.push(-1);
  for (size_t i = regex.size(); i != 0; --i) {
    if (regex[i - 1] == ')') {
      stack.push(i - 1);
      plus_stack.push(-1);
    }
    state.next_plus[i - 1] = plus_stack.top();
    state.next_closing[i - 1] = stack.top();
    if (regex[i - 1] == '+'){
      plus_stack.push(i - 1);
    }
    if (regex[i - 1] == '(') {
      while (plus_stack.top() != -1){
        plus_stack.pop();
      }
      plus_stack.pop();
      stack.pop();
    }
  }
  for (size_t i = 0; i < regex.size(); ++i) {
    if (regex[i] == '*') {
      state.is_iter.Set(i - 1);
      state.is_iter.Set(state.prev_opening[i - 1]);
    }
  }
  return ParseRegexRec(state, 0, 0);
}