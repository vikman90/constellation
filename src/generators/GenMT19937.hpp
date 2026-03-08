#pragma once
#include "IGenerator.hpp"
#include <random>

class GenMT19937 : public IGenerator {
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist;

public:
  GenMT19937();
  double next() override;
  const char *name() const override;
};
