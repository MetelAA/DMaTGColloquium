//
// Created by Artem on 02.11.2025.
//

#include "NaturalNumber.h"
#include "Exceptions/UniversalStringException.h"
#include <cmath>

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


//N-11: Неполное частное от деления первого натурального числа на второе с остатком (делитель отличен от нуля)
NaturalNumber NaturalNumber::quotient(const NaturalNumber &other) const {
    if (other.isEqualZero()) {
        throw UniversalStringException("can not divide by zero");
    }

    if (cmp(&other) == 1) {
        return NaturalNumber(std::vector<uint8_t>{0});
    }

    NaturalNumber dividend(*this); // будем работать с копиями
    const NaturalNumber& divisor(other);
    uint32_t quotient = 0;

    while (dividend.cmp(&divisor) % 2 == 0) {
        dividend = dividend.subtract(divisor); // будем вычитать делитель, пока можно
        quotient++;
    } // O(n * k), постараюсь ускорить, пока так

    std::vector<uint8_t> quotient_result;
    // так как quotient >= 1 => log10 работает исправно
    quotient_result.reserve(static_cast<size_t>(std::log10(quotient) + 1));

    while (quotient > 0) {
        quotient_result.push_back(quotient % 10);
        quotient /= 10;
    }

    return NaturalNumber(quotient_result);
}

//N-12: Остаток от деления первого натурального числа на второе натуральное (делитель отличен от нуля)
NaturalNumber NaturalNumber::remainder(const NaturalNumber &other) const {
    if (other.isEqualZero()) {
        throw UniversalStringException("can not divide by zero");
    }

    if (cmp(&other) == 1) return *this;

    NaturalNumber dividend(*this);
    const NaturalNumber& divisor(other);

    const NaturalNumber quotient = dividend.quotient(divisor);
    const NaturalNumber product = divisor.multiply(quotient);

    return dividend.subtract(product);
}

//N-13: НОД натуральных чисел
NaturalNumber NaturalNumber::GCD(const NaturalNumber &other) const {
    NaturalNumber first_value(*this);
    NaturalNumber second_value(other);

    if (this->isEqualZero() && other.isEqualZero()){
        throw UniversalStringException("the gcd for two zeros is not uniquely defined");
    }

    if (second_value.isEqualZero()) {
        throw UniversalStringException("can not divide by zero");
    }
    // алгоритм Евклида
    while (!second_value.isEqualZero()) {
        if (first_value.cmp(&second_value) == 1) {
            std::swap(first_value, second_value);
        }

        first_value = first_value.remainder(second_value);
    }

    return first_value;
}

//N14: НОК натуральных чисел
NaturalNumber NaturalNumber::LCM(const NaturalNumber &other) const {
    const NaturalNumber first_value(*this);
    const NaturalNumber& second_value(other);

    if (first_value.isEqualZero() || second_value.isEqualZero()) {
        throw UniversalStringException("the lcm for zeros is not uniquely defined");
    }
    // НОК = a * b / НОД(a, b)
    return (first_value.multiply(second_value)).quotient(first_value.GCD(second_value));
}

bool NaturalNumber::isNotEqualZero() {
    return !(numbers.size() == 1 && numbers[0] == 0);
}
