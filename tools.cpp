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

bool compareMin(const Item& a, const Item& b) {
    return a.min < b.min;
}