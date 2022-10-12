//
// Created by mikhail on 12.10.22.
//

#include "gtest/gtest.h"

#include "NFA.h"
#include <fstream>

//regexp a*(ab + b)*b
class NFA_Fixture_1 : public ::testing::Test {
 public:
  NFA_Fixture_1() {
    std::string test_dir = TEST_DIR;
    std::ifstream inp(test_dir + "/Automata1");
    inp >> nfa;
    nfa_without_eps = nfa;
    nfa_without_eps.RemoveEps();
  }
  NFA nfa;
  NFA nfa_without_eps;
};

TEST_F(NFA_Fixture_1, ababab) {
  auto word = "ababab";
  EXPECT_EQ(nfa.ReadWord(word), false);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), false);
}

TEST_F(NFA_Fixture_1, ababb) {
  auto word = "ababb";
  EXPECT_EQ(nfa.ReadWord(word), true);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), true);
}

TEST_F(NFA_Fixture_1, b) {
  auto word = "b";
  EXPECT_EQ(nfa.ReadWord(word), true);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), true);
}

TEST_F(NFA_Fixture_1, aab) {
  auto word = "aab";
  EXPECT_EQ(nfa.ReadWord(word), true);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), true);
}

TEST_F(NFA_Fixture_1, e) {
  auto word = "";
  EXPECT_EQ(nfa.ReadWord(word), false);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), false);
}
