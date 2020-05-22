#include <parallel/timer.hpp>

#include <iostream>

int main() {
  // timerInterfal = 1, repeat = true, block
  utils::Timer fire_once{std::chrono::seconds(1), false,
                         [](utils::Timer &t) { std::cout << "fire\n"; }};

  fire_once.start();
}
