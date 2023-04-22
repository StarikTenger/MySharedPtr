#include <iostream>
#include <memory>

using namespace std;

template<class T>
class smart_ptr {
private:
	T* ptr = nullptr;
	unsigned int* ref_count = nullptr;
	bool null_checked = false;

public:
	// Default constructor
	smart_ptr() : ptr(nullptr), ref_count(new unsigned int(0)) {}

	// Constructor
	smart_ptr(T* ptr) : ptr(ptr), ref_count(new unsigned int(1)) {}

	// Copy constructor
	smart_ptr(const smart_ptr& other) {
		ptr = other.ptr;
		ref_count = other.ref_count;
		null_checked = other.null_checked;
		if (!null_checked) {
			cerr << "Warning: pointer is used without checking on null\n";
		}
		if (ptr) {
			(*ref_count)++;
		}
	}

	// Copy assignment
	smart_ptr& operator=(const smart_ptr& other) {
		//cleanup();
		ptr = other.ptr;
		ref_count = other.ref_count;
		null_checked = other.null_checked;
		if (ptr) {
			(*ref_count)++;
		}
		return *this;
	}

	// Move constructor
	smart_ptr(smart_ptr&& other) {
		ptr = other.ptr;
		ref_count = other.ref_count;
		other.ptr = nullptr;
		other.ref_count = nullptr;
	}

	// Move assignment
	smart_ptr& operator=(smart_ptr&& other) {
		ptr = other.ptr;
		ref_count = other.ref_count;
		null_checked = other.null_checked;
		(*ref_count)++;
		return *this;
	}

	// Dereferencing
	T& operator*() {
		return *ptr;
	}

	// Destructor
	~smart_ptr() {
		(*ref_count)--;
		if (*ref_count == 0) {
			if (ptr) {
				delete ptr;
				ptr = nullptr;
			}
			delete ref_count;
			ref_count = nullptr;
		}
	}

	// For debug
	unsigned int number_of_refs() {
		if (ref_count)
			return *ref_count;
		return 0;
	}

	// Implicit bool conversion
	operator bool() {
		null_checked = true;
		return ptr != nullptr;
	}
};

void foo(smart_ptr<int> x) {
	(*x) = 0;
}

int main() {
	auto x = new int(5);
	auto y = new int(7);
	smart_ptr<int> xp(x);
	foo(xp); // warning here
	if (xp) {
		foo(xp);
	}
	xp = smart_ptr<int>(y);
	if (xp) {
		auto xp1 = xp;
		auto xp2(xp);
		xp2 = xp1;
		cout << xp.number_of_refs() << "\n";
	}
	cout << *x << " x\n";
	if (xp)
		foo(xp);
	cout << *x << " x\n";
	cout << xp.number_of_refs() << "\n";
	cout << "success\n";
}