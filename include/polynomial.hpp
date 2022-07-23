#pragma once

#include <cstdint>

class Polynomial
{
public:
  Polynomial(uint64_t alpha, uint64_t modulo)
    : alpha(alpha)
    , modulo(modulo)
  {
  }

  [[nodiscard]] uint64_t get_alpha() const { return alpha; }
  [[nodiscard]] uint64_t get_modulo() const { return modulo; }

private:
  uint64_t alpha;
  uint64_t modulo;
};
