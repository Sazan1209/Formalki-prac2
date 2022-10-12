//
// Created by mikhail on 09.10.22.
//

#ifndef FORMALKI__BITSET_H_
#define FORMALKI__BITSET_H_

#include <vector>
#include <cstdint>

class Bitset {
 public:
  Bitset() = default;
  Bitset(const Bitset&) = default;
  explicit Bitset(size_t size);
  Bitset(Bitset&& bitset) noexcept;
  Bitset& operator=(Bitset&& bitset) noexcept;

  void Set(size_t pos);
  void Clear();
  Bitset& operator|=(const Bitset& bitset);
  bool operator[](size_t pos) const;

  std::vector<size_t> GetVals();
  void Resize(size_t size);
  [[nodiscard]] size_t BitCount() const;

 private:
  std::vector<uint64_t> body_;
  uint64_t last_mem_mask{};
  static size_t BitCount(uint64_t num);
  static size_t TwoLog(uint64_t digit);
};

#endif //FORMALKI__BITSET_H_
