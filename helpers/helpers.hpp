#pragma once
#include <iostream>

template<typename... Args>
void dbg(Args&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}
