#pragma once

class ref_count {
private:
	int uses = 1;
	int weak = 1;
protected:
	virtual void destory_this() = 0;
	virtual void destory_resource() = 0;
public:
	ref_count() noexcept {}
	virtual ~ref_count() noexcept {}

	void incref() {
		++uses;
	}

	void incwref() {
		++weak;
	}

	void decref() {
		--uses;
		if (uses == 0) {
			destory_resource();
			decwref();
		}
	}

	void decwref() {
		--weak;
		if (weak == 0) {
			destory_this();
		}
	}

	int use_count() {
		return uses;
	}
};

template<typename T>
class ref_count_resource :public ref_count {
public:
	ref_count_resource(T* res_) :res(res_) {}
	ref_count_resource(const T* res_) :res(res_) {}

private:
	T* res;

	void destory_this() {
		delete this;
	}
	void destory_resource() {
		delete res;
	}
};
