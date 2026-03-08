#pragma once
#include "IGenerator.hpp"

/**
 * @brief Standard C library RNG implementation.
 *
 * Wraps the legacy rand() function from <cstdlib>. Typically lower quality
 * and visually biased compared to modern C++ engines.
 */
class GenCRand : public IGenerator {
public:
  GenCRand();
  double next() override;
  const char *name() const override;
};
