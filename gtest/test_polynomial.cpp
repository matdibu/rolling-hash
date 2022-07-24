#include "gtest/gtest.h"

#include "rolling-hash.hpp"

#include <array>
#include <climits>
#include <random>

class DefaultPolynomial : public ::testing::Test
{
protected:
  DefaultPolynomial()
    : poly(Polynomial::DEFAULT_ALPHA, Polynomial::DEFAULT_MODULO)
  {
  }
  Polynomial poly;
};

TEST(ComputeHash, Zero)
{
  std::array<uint64_t, 0> empty_input{};
  ASSERT_EQ(0, polynomial_rolling_hash(empty_input, 0, 0));
}

TEST_F(DefaultPolynomial, CtorAndGet)
{
  EXPECT_EQ(poly.get_alpha(), Polynomial::DEFAULT_ALPHA);
  EXPECT_EQ(poly.get_modulo(), Polynomial::DEFAULT_MODULO);
}

TEST_F(DefaultPolynomial, EmptyHash)
{
  EXPECT_EQ(poly.get_hash(), 0);
}

TEST_F(DefaultPolynomial, OneChar)
{
  std::array input{ 'a' };
  for (const auto chr : input) {
    poly.push(chr);
  }
  ASSERT_EQ(poly.get_hash(),
            polynomial_rolling_hash(input, poly.get_alpha(), poly.get_modulo()));
}

TEST_F(DefaultPolynomial, TwoChars)
{
  std::array input{ 'a', 'b' };
  for (const auto chr : input) {
    poly.push(chr);
  }
  ASSERT_EQ(poly.get_hash(),
            polynomial_rolling_hash(input, poly.get_alpha(), poly.get_modulo()));
}

TEST_F(DefaultPolynomial, ManyChars)
{
  std::vector<char> input(CHAR_MAX);
  std::generate(input.begin(), input.end(), [value = 0]() mutable { return value++; });

  for (const auto chr : input) {
    poly.push(chr);
  }

  ASSERT_EQ(poly.get_hash(),
            polynomial_rolling_hash(input, poly.get_alpha(), poly.get_modulo()));
}

TEST_F(DefaultPolynomial, RandomChars)
{
	std::random_device rnd_device;
	std::mt19937 gen32 {rnd_device()};
	std::uniform_int_distribution<char> dist {CHAR_MIN, CHAR_MAX};
	auto gen = [&dist, &gen32] () {
		return dist(gen32);
	};

	std::vector<unsigned char> input(10000);
	std::generate(input.begin(), input.end(), gen);

	for (const auto chr : input) {
		poly.push(chr);
		printf("%#x ", chr);
	}

	ASSERT_EQ(poly.get_hash(), polynomial_rolling_hash(input, poly.get_alpha(), poly.get_modulo()));
}
