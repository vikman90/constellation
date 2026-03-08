#pragma once

/**
 * @brief Interface for continuous random number generators.
 *
 * Provides a standardized way to generate numerical values, intended primarily
 * to be mapped visually between 0.0 and 1.0.
 */
class IGenerator {
public:
  virtual ~IGenerator() = default;

  /**
   * @brief Generates the next random double.
   *
   * @return A random coordinate, usually in the range [0.0, 1.0].
   */
  virtual double next() = 0;

  /**
   * @brief Gets the human-readable name of the generator.
   *
   * @return A null-terminated string containing the name.
   */
  virtual const char *name() const = 0;
};
