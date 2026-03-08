#include "GenNormal.hpp"

GenNormal::GenNormal() : gen(std::random_device{}()), dist(0.5, 0.15) {}

double GenNormal::next() { return dist(gen); }

const char *GenNormal::name() const {
  return "Normal (Gaussian) Dist [µ=0.5, σ=0.15]";
}
