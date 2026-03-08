#pragma once
#include "IGenerator.hpp"
#include <memory>
#include <vector>

/**
 * @brief Factory class to instantiate random number generators.
 *
 * Centralizes the creation of all supported IGenerator implementations,
 * including platform-specific ones like GenWindows.
 */
class GeneratorFactory {
public:
  /**
   * @brief Creates and returns a list of all generators available on this
   * system.
   *
   * @return A vector of unique pointers to instantiated IGenerators.
   */
  static std::vector<std::unique_ptr<IGenerator>> createAllAvailable();
};
