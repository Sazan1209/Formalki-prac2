//
// Created by mikhail on 14.10.22.
//

#include "gtest/gtest.h"

#include <string>
#include <fstream>
#include "NFA.h"
#include "DFA.h"


//regexp (a + b)^* b (a + b)^3
class NFA_Fixture_2 : public ::testing::Test {
 public:
  NFA_Fixture_2() {
    std::string test_dir = TEST_DIR;
    std::ifstream inp(test_dir + "/Automata2");
    inp >> nfa;
    nfa_without_eps = nfa;
    nfa_without_eps.RemoveEps();
    dfa = DFA(nfa_without_eps);
    m_dfa = dfa;
    m_dfa.Minimize();
  }
  NFA nfa;
  NFA nfa_without_eps;
  DFA dfa;
  DFA m_dfa;
};

TEST_F(NFA_Fixture_2, ababab) {
  auto word = "ababab";
  bool res = false;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
}

TEST_F(NFA_Fixture_2, ababb) {
  auto word = "ababb";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
}

TEST_F(NFA_Fixture_2, b) {
  auto word = "bbbb";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
}

TEST_F(NFA_Fixture_2, aab) {
  auto word = "aab";
  bool res = false;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
}

TEST_F(NFA_Fixture_2, e) {
  auto word = "ababababaabababa";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
}

TEST_F(NFA_Fixture_2, min_dfa){
  EXPECT_EQ(m_dfa.Size(), 16);
}
