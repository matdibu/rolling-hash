#include "chunker.hpp"

#include <cstdint>
#include <iostream>

bool
chunk_t::empty() const noexcept
{
  return start == end;
}

static uint64_t
generate_bitmask(const uint64_t no_of_bits)
{
  uint64_t mask = ((~0ULL) >> no_of_bits) << no_of_bits;
  mask = ~mask;

  return mask;
}

chunk_type
type_from_chr_mask(const unsigned char chr, const uint64_t mask)
{
  if ((chr & mask) == 0) {
    return separator;
  }

  return normal;
}

bool
is_same(const char chr, const chunk_t& chunk, const uint64_t mask)
{
  const chunk_type type = type_from_chr_mask(chr, mask);
  return type == chunk.type;
}

std::vector<chunk_t>
chunker(const std::string_view& input, uint64_t no_of_bits)
{
  std::vector<chunk_t> result;

  const uint64_t bitmask = generate_bitmask(no_of_bits);

  chunk_t current_chunk{};

  for (const char chr : input) {
    if (not is_same(chr, current_chunk, bitmask)) {
      if (not current_chunk.empty()) {
        result.emplace_back(chunk_t{ .start = current_chunk.start,
                                     .end = current_chunk.end - 1,
                                     .type = current_chunk.type });
      }

      current_chunk.type = type_from_chr_mask(chr, bitmask);
      current_chunk.start = current_chunk.end;
    }

    current_chunk.end++;
  }

  if (not current_chunk.empty()) {
    --current_chunk.end;
    result.push_back(current_chunk);
  }

  return result;
}
