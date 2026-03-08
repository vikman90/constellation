#pragma once
#include "IGenerator.hpp"
#include <random>

/**
 * @brief Mersenne Twister RNG implementation.
 *
 * Wraps the standard C++11 std::mt19937 engine, providing high-quality
 * pseudo-random numbers with a uniform distribution.
 */
class GenMT19937 : public IGenerator {
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist;

public:
  GenMT19937();
  double next() override;
  const char *name() const override;
};
