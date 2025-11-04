//
// Created by Artem on 02.11.2025.
//

#include "NaturalNumber.h"
#include "Exceptions/UniversalStringException.h"


std::string NaturalNumber::toString() noexcept {
    if (this->numbers.empty())
        throw UniversalStringException("atypical behavior, the vector of numbers should not be empty");
    std::string result(this->numbers.size(), '0');
    for (int i = this->numbers.size()-1; i >= 0; --i) {
        result[this->numbers.size()-i-1] = '0'+this->numbers.at(i);
    }
    return result;
}

const std::vector<uint8_t> & NaturalNumber::getNumbers() noexcept {
    return this->numbers;
}

NaturalNumber::NaturalNumber(unsigned long long int a) {
    while(a > 0){
        this->numbers.push_back(a % 10);
        a /= 10;
    }
}

NaturalNumber::NaturalNumber(const std::string &a) {
    if (a.empty())
        throw UniversalStringException("wrong argument, the string of numbers should not be empty");
    this->numbers = std::vector<uint8_t>(a.size());
    std::size_t i = a.length() - 1;
    while (true){
        numbers[a.size()-i-1] = a[i] - '0';
        if (i != 0)
            i--;
        else
            break;
    }
}

NaturalNumber::NaturalNumber(const std::vector<uint8_t> &CpNumbers) {
    if (CpNumbers.empty())
        throw UniversalStringException("wrong argument, the vector of numbers should not be empty");
    this->numbers = CpNumbers;
}


