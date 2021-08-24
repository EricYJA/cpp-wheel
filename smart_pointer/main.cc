#include "test_unique_pointer.hpp"
#include <memory>

int main() {

  int test_int = 1;
  int * test_ptr = &test_int;

  test_unique_ptr<int> test_uni_1(new int(10));
  return 0;
}