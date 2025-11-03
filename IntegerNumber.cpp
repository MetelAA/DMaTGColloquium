//
// Created by Artem on 02.11.2025.
//

#include "IntegerNumber.h"
#include "Exceptions/UniversalStringException.h"

bool IntegerNumber::isNegative() noexcept {
    return this->isNegativeFlag;
}

IntegerNumber::IntegerNumber(const std::string &s) {
    if (s.empty())
        throw UniversalStringException("wrong argument, the string of numbers should not be empty");
    this->isNegativeFlag = s[0] == '-';
    if (isNegativeFlag)
        this->number = new NaturalNumber(s.substr(1));
    else
        this->number = new NaturalNumber(s);
}

IntegerNumber::IntegerNumber(long long int a) {
    this->isNegativeFlag = a < 0;
    this->number = new NaturalNumber(isNegativeFlag ? a*(-1) : a);
}

std::string IntegerNumber::toString() {
    return (isNegativeFlag ? "-" : "") + this->number->toString();
}

const std::vector<uint8_t>& IntegerNumber::getNumbers() noexcept {
    return this->number->getNumbers();
}

IntegerNumber IntegerNumber::multiply(const NaturalNumber &other) {
    uint8_t numberSign = this->getSign();
    if (numberSign == 0 || other.isEqualZero()) {
        return IntegerNumber(0);
    }

    NaturalNumber numberAbs = this->abs();
    NaturalNumber multiplyAbs = numberAbs.multiply(other);

    bool resultIsNegative = false;
    if (numberSign == 1) {
        resultIsNegative = true;
    }
    
    const std::vector<uint8_t> &multiplyDigits = multiplyAbs.getNumbers();
    IntegerNumber result = IntegerNumber(multiplyDigits, resultIsNegative);

    return result;
};