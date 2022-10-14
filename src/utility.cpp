//
// Created by mikhail on 11.10.22.
//

#include "utility.h"

size_t LetterToNum(char letter) {
  for (size_t i = 0; i < kSigmaSize + 1; ++i) {
    if (alphabet[i] == letter) {
      return i;
    }
  }
  return kEps;
}

char NumToLetter(size_t num) {
  if (num >= kSigmaSize) {
    return kCharEps;
  }
  return alphabet[num];
}