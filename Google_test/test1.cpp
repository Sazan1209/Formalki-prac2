//
// Created by mikhail on 12.10.22.
//

#include "gtest/gtest.h"

#include "NFA.h"
#include "DFA.h"
#include "testing_funcs.h"
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
    dfa = DFA(nfa_without_eps);
    m_dfa = dfa;
    m_dfa.Minimize();
    m_dfa_complement = m_dfa;
    m_dfa_complement.Complement();
    reg_exp = "(" + m_dfa.GetRegex() + ")";
  }
  NFA nfa;
  NFA nfa_without_eps;
  DFA dfa;
  DFA m_dfa;
  DFA m_dfa_complement;
  std::string reg_exp;
};

TEST_F(NFA_Fixture_1, ababab) {
  auto word = "ababab";
  bool res = false;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
  EXPECT_EQ(ParseRegex(reg_exp, word), res);
  EXPECT_EQ(m_dfa_complement.ReadWord(word), !res);
}

TEST_F(NFA_Fixture_1, ababb) {
  auto word = "ababb";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
  EXPECT_EQ(ParseRegex(reg_exp, word), res);
  EXPECT_EQ(m_dfa_complement.ReadWord(word), !res);
}

TEST_F(NFA_Fixture_1, b) {
  auto word = "b";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
  EXPECT_EQ(ParseRegex(reg_exp, word), res);
  EXPECT_EQ(m_dfa_complement.ReadWord(word), !res);
}

TEST_F(NFA_Fixture_1, aab) {
  auto word = "aab";
  bool res = true;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
  EXPECT_EQ(ParseRegex(reg_exp, word), res);
  EXPECT_EQ(m_dfa_complement.ReadWord(word), !res);
}

TEST_F(NFA_Fixture_1, e) {
  auto word = "";
  bool res = false;
  EXPECT_EQ(nfa.ReadWord(word), res);
  EXPECT_EQ(nfa_without_eps.ReadWord(word), res);
  EXPECT_EQ(dfa.ReadWord(word), res);
  EXPECT_EQ(m_dfa.ReadWord(word), res);
  EXPECT_EQ(ParseRegex(reg_exp, word), res);
  EXPECT_EQ(m_dfa_complement.ReadWord(word), !res);
}

TEST_F(NFA_Fixture_1, min_dfa){
  EXPECT_EQ(m_dfa.Size(), 5);
}