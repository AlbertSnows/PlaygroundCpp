#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>
class rule_of_three
{
	/* raw pointer used as a handle to a
	* dynamically allocated memory block
	*/
	char* cstring; 

	rule_of_three(const char* s, std::size_t n)
		: cstring(new char[n]) { // allocate
		std::memcpy(cstring, s, n); // populate
	}
public: explicit rule_of_three(const char* s = "")
	: rule_of_three(s, std::strlen(s) + 1) {}

	  ~rule_of_three() { // I. destructor
		  delete[] cstring; // deallocate
	}
	  rule_of_three(const rule_of_three& other) // II. copy costructor
		  :	rule_of_three(other.cstring) {}

	  rule_of_three& operator=(const rule_of_three& other) { 
		  // III. copy assignment
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