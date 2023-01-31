#include "tools.h"

int sum(const std::vector<int>& numbers) {
    int result = 0;
    for (int number : numbers) {
        result += number;
    }
    return result;
}

double average(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return 0.0;
    }
    return static_cast<double>(sum(numbers)) / numbers.size();
}

bool compareMin(const std::vector<int>& a, const std::vector<int>& b) {
    return *std::min_element(a.begin(), a.end()) > *std::min_element(b.begin(), b.end());
}