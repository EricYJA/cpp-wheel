#pragma once
#include"ref_count.hpp"

template <typename T>
class test_weak_ptr;

template<typename T>
class test_shared_ptr {
public:
	test_shared_ptr() noexcept {}
	test_shared_ptr(T* ptr) :_ptr(ptr), _ref_count(new ref_count_resource<T>(ptr)) {}
	test_shared_ptr(const test_shared_ptr& other) :_ptr(other._ptr), _ref_count(other._ref_count) {
		if(_ref_count)
			_ref_count->incref();
	}
	test_shared_ptr(test_shared_ptr&& right){
    _ptr = right._ptr;
    right._ptr = nullptr;
    _ref_count = right._ref_count;
    right._ref_count = nullptr;
	}

	~test_shared_ptr() {
		if(_ref_count)
			_ref_count->decref();
	}

	test_shared_ptr& operator=(const test_shared_ptr& other) {
		if (&other ==this) return *this;
		if (_ref_count) _ref_count->decref();
		_ref_count = other._ref_count;
		_ptr = other._ptr;
		if(_ref_count) _ref_count->incref();
		return *this;
	}
  
	test_shared_ptr& operator=(test_shared_ptr&& right) {
		if (&right == this) return *this;
		if(_ref_count) _ref_count->decref();
    _ptr = right._ptr;
    right._ptr = nullptr;
    _ref_count = right._ref_count;
    right._ref_count = nullptr;
	}

	int use_count() {
		if (_ref_count) return _ref_count->use_count();
		return 0;
	}

	bool unique() {
		return !_ref_count || _ref_count->use_count() == 1;
	}

	T& operator*() {
		return *_ptr;
	}
	T* operator->() {
		return _ptr;
	}

	T* get() {
		return _ptr;
	}

private:

	T* _ptr{nullptr};
	ref_count* _ref_count{nullptr};

	friend test_weak_ptr<T>;

  test_shared_ptr(T* ptr, ref_count* ref_count):_ptr(ptr),_ref_count(ref_count) {
		if(_ref_count)
			_ref_count->incref();
	}
};


