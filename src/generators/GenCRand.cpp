#include "GenCRand.hpp"
#include <cstdlib>
#include <ctime>

GenCRand::GenCRand() { srand(static_cast<unsigned>(time(nullptr))); }

double GenCRand::next() { return static_cast<double>(rand()) / RAND_MAX; }

const char *GenCRand::name() const { return "C rand() [Uniform]"; }
