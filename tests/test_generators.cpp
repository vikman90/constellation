#include "generators/GenCRand.hpp"
#include "generators/GenExponential.hpp"
#include "generators/GenMT19937.hpp"
#include "generators/GenNormal.hpp"
#include "generators/GenWindows.hpp"
#include <gtest/gtest.h>


// Test MT19937
TEST(GeneratorTests, MT19937GeneratesInRange) {
  GenMT19937 gen;
  for (int i = 0; i < 1000; ++i) {
    double val = gen.next();
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 1.0);
  }
}

// Test CRand
TEST(GeneratorTests, CRandGeneratesInRange) {
  GenCRand gen;
  for (int i = 0; i < 1000; ++i) {
    double val = gen.next();
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 1.0);
  }
}

// Test Normal Distribution (can go out of [0,1], but we test basic properties)
TEST(GeneratorTests, NormalDistributionReturnsFiniteValues) {
  GenNormal gen;
  double sum = 0.0;
  int iters = 1000;
  for (int i = 0; i < iters; ++i) {
    double val = gen.next();
    // Just verify it returns something without crashing
    sum += val;
  }
  // Mean should be roughly around 0.5 according to the implementation
  double mean = sum / iters;
  EXPECT_GT(mean, 0.3);
  EXPECT_LT(mean, 0.7);
}

// Exponential should be mostly positive
TEST(GeneratorTests, ExponentialGeneratesPositiveValues) {
  GenExponential gen;
  for (int i = 0; i < 100; ++i) {
    double val = gen.next();
    EXPECT_GE(val, 0.0);
  }
}

#ifdef _WIN32
TEST(GeneratorTests, WindowsBCryptGeneratesInRange) {
  GenWindows gen;
  for (int i = 0; i < 1000; ++i) {
    double val = gen.next();
    EXPECT_GE(val, 0.0);
    EXPECT_LE(val, 1.0);
  }
}
#endif
