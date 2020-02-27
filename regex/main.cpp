#include <fstream>
#include <iostream>
#include <string_view>
#include <string>
#include <future>
#include <thread>
#include <regex>

class file_error: public std::exception {
  std::string_view mMessage;
public:
  file_error(std::string_view message) : mMessage(message) {}
  const char* what() const noexcept override {
    return mMessage.data();
  }
};
class filecloser {
  std::ifstream mFile;
  public:
    filecloser(std::ifstream &&file) : mFile{std::move(file)}{
      if (!mFile.is_open()) {
        throw file_error("sorry file is not openned");
      }
    }
    ~filecloser() {
      if (mFile.is_open()) {
        mFile.close();
      }
    }

    std::ifstream &stream() { return mFile;}
};

std::string read_file(std::string_view filename) {
  try {
    std::ifstream input(filename);
    filecloser fc{std::move(input)};
    auto &stream = fc.stream();
    stream >> std::noskipws;
    std::string line = std::string(std::istream_iterator<char>(stream), std::istream_iterator<char>());
    return line;
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
    return "";
  }
}
void print_words(const std::string &data) {
  try {
    std::regex word_regex(R"(size)");
    auto words = std::sregex_iterator(data.begin(), data.end(), word_regex);
    std::cout << "Words: " << std::distance( words, std::sregex_iterator()) << " count\n"; 
  } catch( std::exception &e) {
    std::cout << e.what() << '\n';
  }
}
int main() {
  std::string data = read_file("rename_1");
  if( data.empty() ) {
    std::cout << "empty data\n";
    return -1;
  }
  print_words(data);
  return 0;
}

