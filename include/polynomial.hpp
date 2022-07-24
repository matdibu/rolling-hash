#pragma once

#include <cstddef>
#include <cstdint>

// https://en.wikipedia.org/wiki/Modular_exponentiation#Implementation_in_Lua
[[nodiscard]] uint64_t
powerMod(uint64_t base, uint64_t exponent, uint64_t modulo);

// https://en.wikipedia.org/wiki/Rolling_hash#Polynomial_rolling_hash
[[nodiscard]] uint64_t
polynomial_rolling_hash(auto input_range, uint64_t alpha, uint64_t modulo);

class Polynomial
{
public:
  using poly_alpha_t = uint64_t;
  using poly_modulo_t = uint64_t;
  using poly_hash_t = uint64_t;
  using poly_char_t = unsigned char;

  static constexpr poly_alpha_t DEFAULT_ALPHA = 13;
  static constexpr poly_modulo_t DEFAULT_MODULO = 1000;

  Polynomial(poly_alpha_t alpha, poly_modulo_t modulo);

  [[nodiscard]] poly_alpha_t get_alpha() const;
  [[nodiscard]] poly_modulo_t get_modulo() const;

  [[nodiscard]] poly_hash_t get_hash() const;

  void push(poly_char_t input);

private:
  poly_alpha_t alpha;
  poly_modulo_t modulo;
  poly_hash_t hash;
};