#pragma once
#include"ref_count.hpp"
#include"test_shared_ptr.hpp"
template<typename T>
class weak_ptr {
public:
	weak_ptr(){}
	weak_ptr(const shared_ptr<T>& sp):_Ptr(sp._Ptr),_Ref_count(sp._Ref_count) {
		if (_Ref_count) _Ref_count->incwref();
	}
	weak_ptr(const weak_ptr& other):_Ptr(other._Ptr), _Ref_count(other._Ref_count) {
		if (_Ref_count) _Ref_count->incwref();
	}
	weak_ptr(weak_ptr&& right){
		swap(_Ptr, right._Ptr);
		swap(_Ref_count, right._Ref_count);
	}
	~weak_ptr() {
		if (_Ref_count) _Ref_count->decwref();
	}
	weak_ptr& operator=(const weak_ptr& other) {
		if (&other == this) return *this;
		if (_Ref_count) _Ref_count->decwref();
		_Ptr = other._Ptr;
		_Ref_count = other._Ref_count;
		if (_Ref_count) _Ref_count->incwref();
	}
	weak_ptr& operator=(weak_ptr&& right) {
		if (&right == this) return *this;
		if (_Ref_count) _Ref_count->decref();
		_Ref_count = nullptr;
		_Ptr = nullptr;
		swap(_Ref_count, right._Ref_count);
		swap(_Ptr, right._Ptr);
	}
	weak_ptr& operator=(const shared_ptr<T>& sp) {
		if (_Ref_count) _Ref_count->decwref();
		_Ptr = sp._Ptr;
		_Ref_count = sp._Ref_count;
		if (_Ref_count) _Ref_count->incwref();
		return *this;
	}

	shared_ptr<T> lock() {
		if (expire()) return shared_ptr<T>();
		return shared_ptr<T>(_Ptr, _Ref_count);
	}

	bool expire() {
		return !_Ref_count || _Ref_count->use_count() == 0;
	}

	int use_count() {
		if (_Ref_count) return _Ref_count->use_count();
		return 0;
	}

private:
	T* _Ptr{ nullptr };
	Ref_Count* _Ref_count{ nullptr };
};
