//
// Created by mikhail on 09.10.22.
//

#include "Bitset.h"

std::vector<size_t> Bitset::GetVals() {
  std::vector<size_t> res;
  res.reserve(BitCount());
  for (size_t i = 0; i < body_.size(); ++i) {
    uint64_t curr = body_[i];
    while (curr != 0) {
      auto digit = -curr & curr;
      curr ^= digit;
      res.push_back(64 * i + TwoLog(digit));
    }
  }
  return res;
}

void Bitset::Resize(size_t size) {
  body_.resize((size - 1) / 64 + 1);
  constexpr uint64_t one = 1;
  last_mem_mask = (one << (size % 64)) - 1;
  Clear();
}

size_t Bitset::BitCount() const {
  size_t res = 0;
  for (auto curr : body_) {
    res += BitCount(curr);
  }
  return res;
}

size_t Bitset::BitCount(uint64_t num) {
  num = ((num & 0xAAAAAAAAAAAAAAAA) >> 1) + (num & 0x5555555555555555);
  num = ((num & 0xCCCCCCCCCCCCCCCC) >> 2) + (num & 0x3333333333333333);
  num = ((num & 0xF0F0F0F0F0F0F0F0) >> 4) + (num & 0x0F0F0F0F0F0F0F0F);
  num = ((num & 0xFF00FF00FF00FF00) >> 8) + (num & 0x00FF00FF00FF00FF);
  num = ((num & 0xFFFF0000FFFF0000) >> 16) + (num & 0x0000FFFF0000FFFF);
  num = ((num & 0xFFFFFFFF00000000) >> 32) + (num & 0x00000000FFFFFFFF);
  return num;
}

Bitset::Bitset(size_t size) : body_((size - 1) / 64 + 1, 0) {
  constexpr uint64_t one = 1;
  last_mem_mask = (one << (size % 64)) - 1;
}
void Bitset::Set(size_t pos) {
  constexpr uint64_t one = 1;
  body_[pos / 64] |= (one << (pos % 64));
}
void Bitset::Clear() {
  for (auto& curr : body_) {
    curr = 0;
  }
}

Bitset& Bitset::operator|=(const Bitset& bitset) {
  for (size_t i = 0; i < body_.size(); ++i) {
    body_[i] |= bitset.body_[i];
  }
  return *this;
}

bool Bitset::operator[](size_t pos) const {
  constexpr uint64_t one = 1;
  return (body_[pos / 64] & (one << (pos % 64))) != 0;
}

size_t Bitset::TwoLog(uint64_t digit) {
  size_t res = 0;
  res += 32 * ((digit & 0xFFFFFFFF00000000) != 0);
  res += 16 * ((digit & 0xFFFF0000FFFF0000) != 0);
  res += 8 * ((digit & 0xFF00FF00FF00FF00) != 0);
  res += 4 * ((digit & 0xF0F0F0F0F0F0F0F0) != 0);
  res += 2 * ((digit & 0xCCCCCCCCCCCCCCCC) != 0);
  res += 1 * ((digit & 0xAAAAAAAAAAAAAAAA) != 0);
  return res;
}

bool Bitset::operator==(const Bitset& bitset) const {
  if (bitset.body_.size() != body_.size()){
    return false;
  }
  for (size_t i = 0; i < body_.size(); ++i){
    if (body_[i] != bitset.body_[i]){
      return false;
    }
  }
  return true;
}
bool Bitset::operator&&(const Bitset& bitset) const{
  for (size_t i = 0; i < body_.size(); ++i) {
    if ((body_[i] & bitset.body_[i]) != 0){
      return true;
    }
  }
  return false;
}

size_t BitsetHash::operator()(const Bitset& bitset) const {
  size_t res = 0;
  for (auto curr : bitset.body_){
    res ^= curr;
  }
  return res;
}
