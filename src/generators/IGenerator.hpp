#pragma once

class IGenerator {
public:
  virtual ~IGenerator() = default;

  // Returns a random double between 0.0 and 1.0
  virtual double next() = 0;

  // Returns the human-readable name of the generator
  virtual const char *name() const = 0;
};
