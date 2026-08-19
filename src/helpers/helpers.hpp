#pragma once
#include <iostream>
#include <sstream>
#include <vector>

template<typename... Args>
void dbg(Args&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

// C++ has no built-in way to print/stringify a vector -- element type just
// needs to support << itself (int, string, pair<string,int>, etc.).
template<typename T>
std::string vecToString(const std::vector<T>& v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); i++) {
        oss << v[i];
        if (i + 1 < v.size()) oss << ", ";
    }
    oss << "]";
    return oss.str();
}
