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
		other.reset();
	}

	// Move assignment
	smart_ptr& operator=(smart_ptr&& other) {
		ptr = other.ptr;
		ref_count = other.ref_count;
		null_checked = other.null_checked;
		if (ptr) {
			(*ref_count)++;
		}
		other.reset();
		return *this;
	}

	// Dereferencing
	T& operator*() {
		if (!null_checked) {
 			cerr << "Warning: pointer is used without checking on null\n";
		}
		return *ptr;
	}

	// ->
	T* operator->() {
		if (!null_checked) {
			cerr << "Warning: pointer is used without checking on null\n";
		}
		return ptr;
	}

	// Destructor
	~smart_ptr() {
		reset();
	}

	// Number of active references
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

	// Reset
	void reset() {
		if (ptr) {
			(*ref_count)--;
			if (*ref_count == 0) {
				delete ptr;
				delete ref_count;
				cout << "end";
			}
		}
		ptr = nullptr;
		ref_count = nullptr;
	}

	// Reset to new ptr
	void reset(T* _ptr) {
		reset();
		*this = smart_ptr(_ptr);
	}

	// TODO:
	// Swap
	// Make shared

};

void foo(smart_ptr<int> x) {
	if (x)
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
	if (xp)
		cout << *xp << "\n";
	if (xp) {
		auto xp1 = xp;
		auto xp2(xp);
		xp2 = xp1;
		cout << xp1.number_of_refs() << "\n";
	}
	cout << *x << " x\n";
	if (xp)
		foo(xp);
	cout << *x << " x\n";
	cout << xp.number_of_refs() << "\n";
	auto z = new pair<int, int>({1, 2});
	if (z)
		cout << z->first << " " << z->second << "\n";
	cout << "success\n";
}