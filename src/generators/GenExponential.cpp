#include "GenExponential.hpp"

GenExponential::GenExponential() : gen(std::random_device{}()), dist(5.0) {}

double GenExponential::next() { return dist(gen); }

const char *GenExponential::name() const { return "Exponential Dist [λ=5.0]"; }
