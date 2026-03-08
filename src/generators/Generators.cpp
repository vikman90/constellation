#include "Generators.hpp"

GenMT19937::GenMT19937() : gen(std::random_device{}()), dist(0.0, 1.0) {}
double GenMT19937::next() { return dist(gen); }
const char *GenMT19937::name() const {
  return "Mersenne Twister (std::mt19937)";
}

GenCRand::GenCRand() { srand(static_cast<unsigned>(time(nullptr))); }
double GenCRand::next() { return static_cast<double>(rand()) / RAND_MAX; }
const char *GenCRand::name() const { return "C rand()"; }

#ifdef _WIN32
GenWindows::GenWindows() : hAlg(NULL) {
  BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RNG_ALGORITHM, NULL, 0);
}
GenWindows::~GenWindows() {
  if (hAlg)
    BCryptCloseAlgorithmProvider(hAlg, 0);
}
double GenWindows::next() {
  unsigned int val = 0;
  if (hAlg) {
    BCryptGenRandom(hAlg, (PUCHAR)&val, sizeof(val), 0);
  }
  return static_cast<double>(val) / 0xFFFFFFFF;
}
const char *GenWindows::name() const { return "Windows BCryptGenRandom"; }
#endif

std::vector<std::unique_ptr<IGenerator>>
GeneratorFactory::createAllAvailable() {
  std::vector<std::unique_ptr<IGenerator>> list;
  list.push_back(std::make_unique<GenMT19937>());
  list.push_back(std::make_unique<GenCRand>());
#ifdef _WIN32
  list.push_back(std::make_unique<GenWindows>());
#endif
  return list;
}
