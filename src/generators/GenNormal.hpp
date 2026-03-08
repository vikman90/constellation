#pragma once
#include "IGenerator.hpp"
#include <random>

class GenNormal : public IGenerator {
  std::mt19937 gen;
  std::normal_distribution<double> dist;

public:
  GenNormal();
  double next() override;
  const char *name() const override;
};
