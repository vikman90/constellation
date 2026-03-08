#include "GenMT19937.hpp"

GenMT19937::GenMT19937() : gen(std::random_device{}()), dist(0.0, 1.0) {}

double GenMT19937::next() { return dist(gen); }

const char *GenMT19937::name() const {
  return "Mersenne Twister (std::mt19937)";
}
