//
// Created by mikhail on 09.10.22.
//

#ifndef FORMALKI__BITSET_H_
#define FORMALKI__BITSET_H_

#include <vector>
#include <cstdint>

class BitsetHash;

class Bitset {
 public:
  explicit Bitset(size_t size);
  Bitset() = default;

  void Set(size_t pos);
  void Clear();
  Bitset& operator|=(const Bitset& bitset);
  bool operator==(const Bitset& bitset) const;
  bool operator[](size_t pos) const;
  bool operator&&(const Bitset& bitset) const;

  std::vector<size_t> GetVals();
  void Resize(size_t size);
  [[nodiscard]] size_t BitCount() const;

  friend BitsetHash;
 private:
  std::vector<uint64_t> body_;
  static size_t BitCount(uint64_t num);
  static size_t TwoLog(uint64_t digit);
};

struct BitsetHash{
  size_t operator()(const Bitset& bitset) const;
};

#endif //FORMALKI__BITSET_H_
