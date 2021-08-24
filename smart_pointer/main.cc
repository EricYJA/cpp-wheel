#include "test_unique_pointer.hpp"
#include "test_shared_pointer.hpp"
#include "test_weak_pointer.hpp"
#include <memory>

int main() {
  test_unique_ptr<int> test_uni_0(new int(10));
  test_shared_ptr<int> test_share_0(new int (1));
  test_weak_ptr<int> test_weak_ptr(test_share_0);
  return 0;
}