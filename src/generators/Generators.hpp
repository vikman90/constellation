#pragma once
#include "IGenerator.hpp"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <random>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")
#endif

// Concrete MT19937 Generator
class GenMT19937 : public IGenerator {
  std::mt19937 gen;
  std::uniform_real_distribution<double> dist;

public:
  GenMT19937();
  double next() override;
  const char *name() const override;
};

// Concrete C rand() Generator
class GenCRand : public IGenerator {
public:
  GenCRand();
  double next() override;
  const char *name() const override;
};

#ifdef _WIN32
// Concrete Windows BCrypt Generator
class GenWindows : public IGenerator {
  BCRYPT_ALG_HANDLE hAlg;

public:
  GenWindows();
  ~GenWindows();
  double next() override;
  const char *name() const override;
};
#endif

// Generator Factory
class GeneratorFactory {
public:
  static std::vector<std::unique_ptr<IGenerator>> createAllAvailable();
};
