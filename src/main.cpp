#include "chunker.hpp"
#include "rolling-hash.hpp"

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
  stream << item.start << ' ' << item.end << ' ' << static_cast<bool>(item.type);
  return stream;
}

int
main(int argc, char* argv[])
{
  if (argc != args::MAX_ARGC) {
    std::cerr << "argc is not " << MAX_ARGC << std::endl;
    return -1;
  }

  std::span argv_span(argv, argc);

  std::filesystem::path old_file = argv_span[args::old_file];
  std::filesystem::path new_file = argv_span[args::new_file];
  std::filesystem::path out_file = argv_span[args::out_file];

  std::cout << old_file << std::endl;
  std::cout << new_file << std::endl;
  std::cout << out_file << std::endl;

  if (not std::filesystem::exists(old_file)) {
    std::cerr << "no file at " << old_file << std::endl;
    return -1;
  }

  if (not std::filesystem::exists(new_file)) {
    std::cerr << "no file at " << new_file << std::endl;
    return -1;
  }

  if (std::filesystem::exists(out_file)) {
    std::cerr << "file already exists at " << out_file << std::endl;
    return -1;
  }

  std::ifstream old_file_stream(old_file, std::ios::binary | std::ios::in);
  old_file_stream.exceptions(std::ios::badbit);

  constexpr size_t no_of_bits = 8;

  std::vector<chunk_t> old_chunks;

  std::string buffer(4096, 0);
  while (old_file_stream) {
    std::cout << "reading..." << std::endl;
    std::cout << "buffer.size() is " << buffer.size() << std::endl;
    old_file_stream.read(buffer.data(), buffer.size());
    const size_t bytes_read = old_file_stream.gcount();
    std::cout << "read " << bytes_read << " bytes" << std::endl;
    auto chunks = chunker(buffer, no_of_bits);
    std::cout << "obtained " << chunks.size() << " chunks" << std::endl;
    old_chunks.insert(old_chunks.end(), chunks.begin(), chunks.end());
  }

  for (const auto& chunk : old_chunks) {
    std::cout << chunk << std::endl;
  }

  std::cout << "a total of " << old_chunks.size() << " chunks" << std::endl;

  return 0;
}
