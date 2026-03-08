#include "GeneratorFactory.hpp"
#include "GenCRand.hpp"
#include "GenExponential.hpp"
#include "GenMT19937.hpp"
#include "GenNormal.hpp"
#include "GenWindows.hpp"


std::vector<std::unique_ptr<IGenerator>>
GeneratorFactory::createAllAvailable() {
  std::vector<std::unique_ptr<IGenerator>> list;
  list.push_back(std::make_unique<GenMT19937>());
  list.push_back(std::make_unique<GenCRand>());
  list.push_back(std::make_unique<GenNormal>());
  list.push_back(std::make_unique<GenExponential>());
#ifdef _WIN32
  list.push_back(std::make_unique<GenWindows>());
#endif
  return list;
}
