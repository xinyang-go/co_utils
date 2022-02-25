#include "co_utils.hpp"
#include <iostream>

generator<int> fibo(int n) {
    int x = 0, a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        if (i > 20) {
            throw std::runtime_error("too big!");
        }
        x = a + b;
        co_yield x;
        a = b;
        b = x;
    }
}

int main() {
    for(auto x : fibo(30)) {
        std::cout << x << std::endl;
    }

    return 0;
}
