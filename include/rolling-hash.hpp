#ifndef ROLLING_HASH_HPP
#define ROLLING_HASH_HPP

#include <cstdint>
#include <ranges>

// https://en.wikipedia.org/wiki/Modular_exponentiation#Implementation_in_Lua
[[nodiscard]] uint64_t
powerMod(uint64_t base, uint64_t exponent, uint64_t modulo);

// https://en.wikipedia.org/wiki/Rolling_hash#Polynomial_rolling_hash
[[nodiscard]] uint64_t
polynomial_rolling_hash(const std::ranges::range auto& input_range, uint64_t alpha, uint64_t modulo)
{
  uint64_t result = 0;

  if (modulo < 2) {
    return 0;
  }

  size_t size = input_range.size() - 1;
  for (const auto item : input_range) {
    result += (item % modulo) * powerMod(alpha, size, modulo);
    result %= modulo;
    size--;
  }

  return result;
}

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
  poly_hash_t hash {0};
};

#endif // ROLLING_HASH_HPP
