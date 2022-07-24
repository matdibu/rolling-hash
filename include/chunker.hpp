#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <bitset>
#include <cstddef>
#include <string_view>
#include <vector>

#include <iostream>

enum chunk_type : bool
{
  normal,
  separator
};

struct chunk_t
{
  size_t start;
  size_t end;
  chunk_type type;
  constexpr bool operator==(const chunk_t& other) const = default;
  [[nodiscard]] size_t size() const { return end - start; }
};

std::vector<chunk_t>
chunker(const std::u8string_view& input, uint64_t no_of_bits)
{
  std::vector<chunk_t> result;

  uint64_t mask = (~0ULL) >> no_of_bits << no_of_bits;
  mask = ~mask;

  chunk_t current_chunk{};

  for (const auto chr : input) {
    const bool is_separator = (chr & mask) == 0;
    if (is_separator) {
      if (current_chunk.size() > 0) {
        --current_chunk.end;
        result.push_back(current_chunk);
        ++current_chunk.end;
      }

      current_chunk.start = current_chunk.end;

      result.push_back(chunk_t{
        .start = current_chunk.end, .end = current_chunk.end, .type = separator });

      ++current_chunk.end;
      ++current_chunk.start;

    } else {
      ++current_chunk.end;
    }
  }

  if (current_chunk.size() > 0) {
    --current_chunk.end;
    result.push_back(current_chunk);
  }

  return result;
}

#endif // CHUNKER_HPP
