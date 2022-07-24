#include "gtest/gtest.h"

#include "chunker.hpp"

#include <algorithm>
#include <array>
#include <ranges>

std::ostream&
operator<<(std::ostream& stream, const chunk_t& item)
{
  stream << std::boolalpha << item.start << ' ' << item.end << ' '
         << static_cast<bool>(item.type) << std::endl;
  return stream;
}

struct config_t
{
  struct input
  {
    const std::u8string_view string;
    const uint64_t bits;
  };

  const input input;
  const std::vector<chunk_t> expected;
};

static bool
is_config_valid(const config_t& config)
{
  const auto result = chunker(config.input.string, config.input.bits);

  const bool is_valid = std::ranges::equal(result, config.expected);

  if (not is_valid)
  {
		for(const auto& item : result)
		{
			std::cout << item << ' ';
		}
		std::cout << std::endl;
  }

  return is_valid;
}

TEST(Chunker, OneSep)
{
  const config_t config = { .input = { .string = u8"8", .bits = 3 },
                            .expected = { { .start = 0, .end = 0, .type = separator } } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, TwoSep)
{
  const config_t config = { .input = { .string = u8"88", .bits = 3 },
                            .expected = { { .start = 0, .end = 0, .type = separator },
                                          { .start = 1, .end = 1, .type = separator } } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, ThreeSep)
{
  const config_t config = { .input = { .string = u8"888", .bits = 3 },
                            .expected = {
                              { .start = 0, .end = 0, .type = separator },
                              { .start = 1, .end = 1, .type = separator },
                              { .start = 2, .end = 2, .type = separator },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, OneNormal)
{
  const config_t config = { .input = { .string = u8"7", .bits = 3 },
                            .expected = {
                              { .start = 0, .end = 0, .type = normal },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, OneTwice)
{
  const config_t config = { .input = { .string = u8"77", .bits = 3 },
                            .expected = {
                              { .start = 0, .end = 1, .type = normal },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, SepItemSep)
{
  const config_t config = { .input = { .string = u8"878", .bits = 3 },
                            .expected = {
                              { .start = 0, .end = 0, .type = separator },
                              { .start = 1, .end = 1, .type = normal },
                              { .start = 2, .end = 2, .type = separator },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, TwoNTwoSepTwoN)
{
  const config_t config = { .input = { .string = u8"778877", .bits = 3 },
                            .expected = {
                              { .start = 0, .end = 1, .type = normal },
                              { .start = 2, .end = 2, .type = separator },
                              { .start = 3, .end = 3, .type = separator },
                              { .start = 4, .end = 5, .type = normal },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}

TEST(Chunker, LongSepOne)
{
  const config_t config = { .input = { .string = u8"122345", .bits = 2 },
                            .expected = {
                              { .start = 0, .end = 3, .type = normal },
                              { .start = 4, .end = 4, .type = separator },
                              { .start = 5, .end = 5, .type = normal },
                            } };

  ASSERT_TRUE(is_config_valid(config));
}
