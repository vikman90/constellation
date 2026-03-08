#pragma once
#include "IGenerator.hpp"

class GenCRand : public IGenerator {
public:
  GenCRand();
  double next() override;
  const char *name() const override;
};
