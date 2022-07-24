#ifndef CHUNKER_HPP
#define CHUNKER_HPP

#include <string_view>
#include <vector>

enum chunk_type
{
  normal = 1,
  separator = 2
};

struct chunk_t
{
  size_t start;
  size_t end;
  chunk_type type;

  [[nodiscard]] constexpr bool operator==(const chunk_t& other) const = default;
  [[nodiscard]] bool empty() const noexcept;

} __attribute__((aligned(32))); // NOLINT

std::vector<chunk_t>
chunker(const std::string_view& input, uint64_t no_of_bits);

#endif // CHUNKER_HPP
