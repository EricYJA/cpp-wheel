#pragma once
#include"ref_count.hpp"
#include"test_shared_pointer.hpp"

template<typename T>
class test_weak_ptr {
public:
	test_weak_ptr() noexcept {}
	test_weak_ptr(const test_shared_ptr<T>& sp):_ptr(sp._ptr),_ref_count(sp._ref_count) {
		if (_ref_count) _ref_count->incwref();
	}
	test_weak_ptr(const test_weak_ptr& other):_ptr(other._ptr), _ref_count(other._ref_count) {
		if (_ref_count) _ref_count->incwref();
	}
	test_weak_ptr(test_weak_ptr&& right){
    _ptr = right._ptr;
    right._ptr = nullptr;
    _ref_count = right._ref_count;
    right._ref_count = nullptr;
	}

	~test_weak_ptr() {
		if (_ref_count) _ref_count->decwref();
	}

	test_weak_ptr& operator=(const test_weak_ptr& other) {
		if (&other == this) return *this;
		if (_ref_count) _ref_count->decwref();
		_ptr = other._ptr;
		_ref_count = other._ref_count;
		if (_ref_count) _ref_count->incwref();
	}
	test_weak_ptr& operator=(test_weak_ptr&& right) {
		if (&right == this) return *this;
		if (_ref_count) _ref_count->decref();
    _ptr = right._ptr;
    right._ptr = nullptr;
    _ref_count = right._ref_count;
    right._ref_count = nullptr;
	}
	test_weak_ptr& operator=(const test_shared_ptr<T>& sp) {
		if (_ref_count) _ref_count->decwref();
		_ptr = sp._ptr;
		_ref_count = sp._ref_count;
		if (_ref_count) _ref_count->incwref();
		return *this;
	}

	test_shared_ptr<T> lock() {
		if (expire()) return test_shared_ptr<T>();
		return test_shared_ptr<T>(_ptr, _ref_count);
	}

	bool expire() {
		return !_ref_count || _ref_count->use_count() == 0;
	}

	int use_count() {
		if (_ref_count) return _ref_count->use_count();
		return 0;
	}

private:
	T* _ptr{ nullptr };
	ref_count* _ref_count{ nullptr };
};
