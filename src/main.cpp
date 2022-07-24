#include "chunker.hpp"
#include "rolling-hash.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

enum args : size_t
{
  program_path = 0,
  old_file = 1,
  new_file = 2,
  out_file = 3,
  MAX_ARGC = 4
};

std::ostream&
operator<<(std::ostream& stream, const chunk_t& item)
{
  stream << "start:" << item.start << " end:" << item.end << ' ';
  if (item.type == separator) {
    stream << "sep";
  }
  if (item.type == normal) {
    stream << "nor";
  }

  stream << " size:" << item.end - item.start + 1;
  return stream;
}

std::vector<chunk_t>
chunker_file(std::ifstream& infile, uint64_t no_of_bits)
{
  std::vector<chunk_t> result;

  constexpr size_t BUFFER_SIZE = 4096;

  size_t buffer_no = 0;

  auto add_global_offset = [&BUFFER_SIZE, &buffer_no](const chunk_t& chunk) {
    return chunk_t{ .start = chunk.start + BUFFER_SIZE * buffer_no,
                    .end = chunk.end + BUFFER_SIZE * buffer_no,
                    .type = chunk.type };
  };

  std::string buffer(BUFFER_SIZE, 0);
  while (infile) {
    std::cout << "reading..." << std::endl;
    std::cout << "buffer.size() is " << buffer.size() << std::endl;
    infile.read(buffer.data(), BUFFER_SIZE);
    const size_t bytes_read = infile.gcount();
    std::cout << "read " << bytes_read << " bytes" << std::endl;
    auto chunks = chunker(buffer, no_of_bits);
    std::cout << "obtained " << chunks.size() << " chunks" << std::endl;
    std::transform(chunks.begin(), chunks.end(), chunks.begin(), add_global_offset);
    result.insert(result.end(), chunks.begin(), chunks.end());
    ++buffer_no;
  }

  uint64_t sum = 0;

  for (const auto& chunk : result) {
    sum += chunk.end - chunk.start;
    std::cout << chunk << std::endl;
  }

  std::cout << "a total of " << result.size() << " chunks" << std::endl;
  std::cout << "average chunk length: " << sum / result.size() << std::endl;

  std::sort(
    result.begin(), result.end(), [](const chunk_t& lhs, const chunk_t& rhs) {
      return lhs.end - lhs.start > rhs.end - rhs.start;
    });

  std::cout << "median chunk length: " << result[result.size() / 2] << std::endl;

  return result;
}

int
compute_diff(int argc, char* argv[]) // NOLINT
{
  if (argc != args::MAX_ARGC) {
    std::cerr << "argc is not " << MAX_ARGC << std::endl;
    return -1;
  }

  std::span argv_span(argv, argc);

  std::filesystem::path old_file_path = argv_span[args::old_file];
  std::filesystem::path new_file_path = argv_span[args::new_file];
  std::filesystem::path out_file_path = argv_span[args::out_file];

  std::cout << old_file_path << std::endl;
  std::cout << new_file_path << std::endl;
  std::cout << out_file_path << std::endl;

  if (not std::filesystem::exists(old_file_path)) {
    std::cerr << "no file at " << old_file_path << std::endl;
    return -1;
  }

  if (not std::filesystem::exists(new_file_path)) {
    std::cerr << "no file at " << new_file << std::endl;
    return -1;
  }

  if (std::filesystem::exists(out_file_path)) {
    std::cerr << "file already exists at " << out_file << std::endl;
    return -1;
  }

  std::ifstream old_file(old_file_path, std::ios::binary | std::ios::in);
  old_file.exceptions(std::ios::badbit);

  std::ifstream new_file(new_file_path, std::ios::binary | std::ios::in);
  new_file.exceptions(std::ios::badbit);

  constexpr size_t no_of_bits = 16;

  const auto old_chunks = chunker_file(old_file, no_of_bits);
  const auto new_chunks = chunker_file(new_file, no_of_bits);

  return 0;
}

int
main(int argc, char* argv[])
{
  int result = -1;

  try {
    result = compute_diff(argc, argv);
  } catch (const std::exception& exc) {
    std::cerr << "exc in main: " << exc.what() << std::endl;
  }

  return result;
}
