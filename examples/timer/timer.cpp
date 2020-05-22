#include <parallel/timer.hpp>

#include <iostream>

int main() {
  // timerInterfal = 1, repeat = true, block
  timer every_second{std::chrono::seconds(1),true,[](timer &t){
    std::cout << "fire\n";
  }};

  every_second.start();

  int just_wait{};
  std::cin >> just_wait;

}
