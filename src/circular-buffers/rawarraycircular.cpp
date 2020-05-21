#include <iostream>
#include <iterator>

namespace circle {

namespace core {
template<typename T, size_t Size = 4>
class circularraw {
  public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using reference = value_type&;
    using const_reference = const value_type&;

    template<typename It>
    class iterator_impl {
      public:
        using value = It;
        iterator_impl(pointer p) : raw(p) {}

        reference operator*() { return *raw; }
        iterator_impl operator++() {
          return iterator_impl(++raw);
        }
        
        iterator_impl next() {
            return iterator_impl(raw + 1);
        }

        bool operator!=(const iterator_impl& ii) {
          return raw != ii.raw;
        }
        
        friend std::ostream& operator<<(std::ostream &out, const iterator_impl& ii) {
          out << ii.raw;
          return out;
        }
      private:
        pointer raw{nullptr};
    };

    using iterator = iterator_impl<value_type>;
    using const_iterator = const iterator;
    iterator begin() {
      return pbegin;
    }
    iterator end() {
      return pend;
    }
    // constructors
    circularraw() : buffer(), pbegin(buffer), pend(buffer + Size), pposition(buffer) {}


    void push (const_reference newvalue) {
      auto next = pposition.next();
      if (!(next != pend)) {
        next = pbegin;
      }
      *pposition = newvalue;
      pposition = next;
    }
    
    // return current positon of circular buffer
    iterator position() {
        return pposition;
    }
    
    
    std::array<value_type, Size> to_array() {
        
    }

  private:
     value_type buffer[Size];
     iterator pbegin;
     iterator pend;
     iterator pposition;
    
};
}

template<typename Iter, typename F>
F for_each(Iter begin, Iter end, F &&f) {
    for (;begin != end; ++begin) {
        f(*begin);
    }
    return f;
}



}

int main() {
  using namespace circle::core;
  circularraw<int> buffer;
  buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4);
  buffer.push(5);
  buffer.push(6);
  buffer.push(7);
  
    
  circle::for_each(buffer.begin(), buffer.end(), [](const auto &val){
    std::cout << val;
  });

}
