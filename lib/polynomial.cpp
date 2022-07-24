#include "rolling-hash.hpp"

// https://en.wikipedia.org/wiki/Modular_exponentiation#Implementation_in_Lua
[[nodiscard]] uint64_t
powerMod(uint64_t base, uint64_t exponent, const uint64_t modulo)
{
  if (modulo == 1) {
    return 0;
  }
  uint64_t result = 1;
  base = base % modulo;
  while (exponent > 0) {
    if (exponent % 2 == 1) {
      result = (result * base) % modulo;
    }
    base = (base * base) % modulo;
    exponent = exponent >> 1U;
  }
  return result;
}

Polynomial::Polynomial(poly_alpha_t alpha, poly_modulo_t modulo)
  : alpha(alpha)
  , modulo(modulo)
{
}

[[nodiscard]] Polynomial::poly_alpha_t
Polynomial::get_alpha() const
{
  return alpha;
}
[[nodiscard]] Polynomial::poly_modulo_t
Polynomial::get_modulo() const
{
  return modulo;
}

[[nodiscard]] Polynomial::poly_hash_t
Polynomial::get_hash() const
{
  return hash;
}

void
Polynomial::push(poly_char_t input)
{
  hash *= get_alpha();
  hash %= get_modulo();
  hash += input;
  hash %= get_modulo();
}
