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

// Z-8: Умножение целых чисел.
IntegerNumber IntegerNumber::multiply(const IntegerNumber &other) const {
    // getSign() дает ноль, если число равно нулю. Поэтому юзаю его.
    // Тут простая проверка, если первое или второе число ноль - умножение равно нулю.
    u_int8_t numberSign = this->getSign();
    u_int8_t otherSign = other.getSign();
    if (numberSign == 0 || otherSign == 0) {
        return IntegerNumber(0);
    }

    // Берем модуль числа для умножения и умножаем числа.
    NaturalNumber numberAbs = this->abs();
    NaturalNumber otherAbs = other.abs();
    NaturalNumber multiplyAbs = numberAbs.multiply(otherAbs);

    // Вспоминаем, какой знак был у нашего числа.
    // Отрицательное, если знаки у обоих чисел не одинаковые.
    bool resultIsNegative = numberSign != otherSign;

    const std::vector<uint8_t> &multiplyDigits = multiplyAbs.getNumbers();
    IntegerNumber result = IntegerNumber(multiplyDigits, resultIsNegative);

    return result;
};