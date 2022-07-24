#include "chunker.hpp"

bool chunk_t::empty() const noexcept {
	return start == end;
}

std::vector<chunk_t>
chunker(const std::u8string_view& input, uint64_t no_of_bits)
{
  std::vector<chunk_t> result;

  uint64_t mask = (~0ULL) >> no_of_bits << no_of_bits;
  mask = ~mask;

  chunk_t current_chunk{ .type = normal };

  for (const auto chr : input) {
    const bool is_separator = (chr & mask) == 0;
    if (is_separator) {
      if (not current_chunk.empty()) {
        result.emplace_back(chunk_t{ .start = current_chunk.start,
                                     .end = current_chunk.end - 1,
                                     .type = current_chunk.type });
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

  if (not current_chunk.empty()) {
    --current_chunk.end;
    result.push_back(current_chunk);
  }

  return result;
}
