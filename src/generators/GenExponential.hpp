#pragma once
#include "IGenerator.hpp"
#include <random>

class GenExponential : public IGenerator {
  std::mt19937 gen;
  std::exponential_distribution<double> dist;

public:
  GenExponential();
  double next() override;
  const char *name() const override;
};
