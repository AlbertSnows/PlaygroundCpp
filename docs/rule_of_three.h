#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>

/*
* Analysis
* you need a destructor, copy constructor, and copy assignment
* 
* constructing use cases - copy constructor copys without assigment. e.g. heap allocation, and many more, use make_unique
* assignment - copying, moving with assignment, they use equals
* 
* moving - auto x = move(y);
* copying - auto x = y;
* 
* auto& x = y; // reference allocation
* 
* 
* e.g. string s = "oasneuthsaou" raii
* or
* string s2("aoeunshtoansuehas") constructor
* or 
* string s = { "oausntahoeus" aoseunthaouse" }
* or
* string s = new string("ouonauehts"); // constructor
* or
* string s2 = make_unique<string>("oeuasoeunth");
* or
* string s = "aouenht"; RAII
* string s2 = s; // copy assignment
* string s3(s); // copy constructor JUST SYNTAX !!
*/
class rule_of_three
{
	/* raw pointer used as a handle to a
	* dynamically allocated memory block
	*/
	char* cstring; 

	// this is a private init function
	rule_of_three(const char* s, std::size_t n)
		: cstring(new char[n]) { // allocate
		std::memcpy(cstring, s, n); // populate
	}
public: 
	// not sure what explicit does
	// calls the private function with s and strlen of s

	explicit rule_of_three(const char* s = "")
	: rule_of_three(s, std::strlen(s) + 1) {}

	  ~rule_of_three() { // I. destructor
		  delete[] cstring; // deallocate
	}
	  // & = by reference
	  rule_of_three(const rule_of_three& other) // II. copy costructor
		  :	rule_of_three(other.cstring) {}

	  rule_of_three& operator=(const rule_of_three& other) { 
		  // III. copy assignment
		  // if it's already the same class (by reference?)
		  if (this == &other)
			  return *this;
		  std::size_t n{ std::strlen(other.cstring) + 1 };
		  char* new_cstring = new char[n]; // allocate
		  std::memcpy(new_cstring, other.cstring, n); // populate
		  delete[] cstring;
		  cstring = new_cstring;
		  return *this;
	  }

	  operator const char* () const { // accessor
		  return cstring; 
	  }
};

/*
* Analysis
* if you want move semantics,
* (e.g. take ownership of memory)
* you need to also add a move and move assignment constructor
*/
class rule_of_five {
	/* raw pointer used as a handle to a
	* dynamically allocated memory block
	*/
	char* cstring;
public: 
	explicit rule_of_five(const char* s = "")
		: cstring(nullptr) {
		if (s) {
			std::size_t n = std::strlen(s) + 1;
			cstring = new char[n]; // allocate
			std::memcpy(cstring, s, n); // populate
		}
	}
	~rule_of_five() {
	  delete[] cstring; // deallocate
	}
	rule_of_five(const rule_of_five& other)
		: rule_of_five(other.cstring) {} // copy constructor
	// exchange trades places of value
	rule_of_five(rule_of_five&& other) noexcept // move constructor
		: cstring(std::exchange(other.cstring, nullptr)) {}
	rule_of_five& operator=(const rule_of_five& other) {
		// copy assignment
		return *this = rule_of_five(other);
	}
	// double reference
	// tells class it's allowed to move
	// noexcept means no exception
	rule_of_five& operator=(rule_of_five&& other) noexcept {
		// move assignment
		std::swap(cstring, other.cstring);
		return *this;
	}
	// alternatively, replace both assignment operators with 
	//  rule_of_five& operator=(rule_of_five other) noexcept
	//  {
	//      std::swap(cstring, other.cstring);
	//      return *this;
	//  }
};

class rule_of_zero {
	std::string cppstring; 
public: rule_of_zero(const std::string& arg)
	: cppstring(arg) {}
};

// makes class prone to slicing?
class base_of_five_defaults {
public: 
	base_of_five_defaults(const base_of_five_defaults&)
		= default;
	base_of_five_defaults(base_of_five_defaults&&)
		= default;
	base_of_five_defaults& operator
		= (const base_of_five_defaults&) = default;
	base_of_five_defaults& operator
		= (base_of_five_defaults&&) = default;
	virtual ~base_of_five_defaults() = default;
};

// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c67-a-polymorphic-class-should-suppress-public-copymove
// virtual just means a class can be overridden
// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c21-if-you-define-or-delete-any-copy-move-or-destructor-function-define-or-delete-them-all












int main() {
	rule_of_three ol{ "abc" };
	std::cout << ol << ' ';
	auto o2{ ol }; // II. uses copy constructor
	std::cout << o2 << ' ';
	rule_of_three o3("def");
	std::cout << o3 << ' ';
	o3 = o2; // III. uses copy assignment
	std::cout << o3 << '\n';
	// I. all deconstructors are called here
}

/*
* Analysis
* copy vs copy assign
* Problem scope: 
* When you make a new instantion of a class, 
* and assign it to the previous value, 
* is it copied by value or assignment?
* 
*/