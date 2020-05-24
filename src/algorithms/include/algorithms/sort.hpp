#ifndef CPPWORKFLOW_ALGORITHMS_SORT_HPP
#define CPPWORKFLOW_ALGORITHMS_SORT_HPP
namespace algorithms {
  template <typename Container>
  void sort(const Container& c) {
    if (c.empty()) {
      return;
    }
    using iterator = Container::iterator;

  }
}

#endif // CPPWORKFLOW_ALGORITHMS_SORT_HPP
