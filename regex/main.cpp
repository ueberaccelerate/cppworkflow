
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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
    ~filecloser() noexcept {
      if (mFile.is_open()) {
        mFile.close();
      }
    }

    std::ifstream &stream() { return mFile;}
};

std::string read_file(std::string_view filename) {
  try {
    filecloser fc{std::ifstream(filename)};
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


int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}

