#pragma once

/* 
Note that this is a simple implementation of unique_pointer. 

It is designed for ALLOCATED MEMORY POINTER. Why? Since I need
to take rvalue reference as aninput and the default 
deconstructor uses delete. 

Becasue the stack memory and heap(dynamic) memory are seperate. 
You can't just use `free()` or `delete` to deallocate the 
pointer initialized in the stack memory. Hence, for real C++ 11
unique pointer, you should use your own destructor or there will
be undefined behavior causing segmentation fault.

Undefined Behavior Example:

  int test_int = 1;
  int * test_ptr = &test_int;
  test_unique_ptr<int> test_uni_0(test_ptr);

STD also behaves as UB:

  std::unique_ptr<int> test_uni_std(test_ptr);

*/

template<typename T>
class test_unique_ptr {
 public:
  test_unique_ptr() noexcept {}
  test_unique_ptr(T* ptr) {
    _ptr = ptr;
    ptr = nullptr;
  }

  test_unique_ptr(test_unique_ptr && right) noexcept {
     _ptr = right._ptr;
     right._ptr = nullptr;
  }

  ~test_unique_ptr() {
    deconstruct_self();
  }

  // rvalue reference: make sure the unique_ptr is unique
  test_unique_ptr& operator=(test_unique_ptr && right) {
    if (this == &right) return *this;
    deconstruct_self();
    _ptr = right._ptr;
    right._ptr = nullptr;
    return *this;
  }

  T& operator* () {
    return *_ptr;
  }

  T* operator-> ();

  T* get() {
    return _ptr;
  }

  T* release() {
    T* obj = _ptr;
    _ptr = nullptr;
    return obj;
  }

  void reset(T* ptr = nullptr) {
    deconstruct_self();
    _ptr = ptr;
  }

  test_unique_ptr(const test_unique_ptr&) = delete;
  test_unique_ptr& operator= (const test_unique_ptr&) = delete;

 private:
  T* _ptr{nullptr};

  void deconstruct_self() {
    if (_ptr) {
      delete _ptr;
      _ptr = nullptr;
    }
  }
};
