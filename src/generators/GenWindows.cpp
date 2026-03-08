#include "GenWindows.hpp"

#ifdef _WIN32

#pragma comment(lib, "bcrypt.lib")

GenWindows::GenWindows() : hAlg(NULL) {
  BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RNG_ALGORITHM, NULL, 0);
}

GenWindows::~GenWindows() {
  if (hAlg) {
    BCryptCloseAlgorithmProvider(hAlg, 0);
  }
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
