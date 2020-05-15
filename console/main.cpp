#include <list>
#include <algorithm>
#include <iostream>

int main() {
  std::list<int> input = {1,32,12,34,3,646,57,876,3};

  auto divide_point = std::partition(input.begin(), input.end(),[](const int &v){return v < 42;});

  for(auto d : divide_point) {
    std::cout << d << ' ';
  }



  return 0;
}
