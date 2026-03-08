#pragma once
#include "IGenerator.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif
#include <bcrypt.h>

class GenWindows : public IGenerator {
  BCRYPT_ALG_HANDLE hAlg;

public:
  GenWindows();
  ~GenWindows();
  double next() override;
  const char *name() const override;
};
#endif
