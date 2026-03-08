#pragma once
#include "IGenerator.hpp"
#include <memory>
#include <vector>


class GeneratorFactory {
public:
  static std::vector<std::unique_ptr<IGenerator>> createAllAvailable();
};
