//
// Created by Artem on 03.11.2025.
//

#include "RationalNumber.h"
#include "Exceptions/UniversalStringException.h"

RationalNumber::RationalNumber(long long numeratorA, long long denominatorA) {
    this->numerator = new IntegerNumber(numeratorA);
    this->denominator = new NaturalNumber(denominatorA);
}

RationalNumber::RationalNumber(const std::string &numeratorA, const std::string &denominatorA) {
    this->numerator = new IntegerNumber(numeratorA);
    this->denominator = new NaturalNumber(denominatorA);
}

std::string RationalNumber::toString() const{
    return this->numerator->toString() + "/" + this->denominator->toString();
}

const IntegerNumber &RationalNumber::getIntegerNumerator() const noexcept {
    return *this->numerator;
}

const NaturalNumber &RationalNumber::getNaturalDenominator() const noexcept {
    return *this->denominator;
}

RationalNumber::RationalNumber(const std::string &s) {
    size_t delimiterPos = s.find('/');
    std::string numeratorS = s.substr(0, delimiterPos);
    std::string denominatorS = s.substr(delimiterPos + 1);
    this->numerator = new IntegerNumber(numeratorS);
    this->denominator = new NaturalNumber(denominatorS);
}

// Q-1: сокращение дроби.
void RationalNumber::reduce() {
    // Беру модуль числителя, чтобы поиск НОД не вызвал проблем.
    // Далее ищу НОД.
    NaturalNumber numeratorAbs = this->numerator->abs();
    NaturalNumber gcd = numeratorAbs.GCD(*this->denominator);

    // Если НОД равен 1, то это финиш (некуда сокращать).
    const std::vector<uint8_t> &gcdDigits = gcd.getNumbers();
    size_t digitsCount = gcdDigits.size();
    if (digitsCount == 1) {
        if (gcdDigits[0] == 1) {
            return;
        }
    }

    // Сокращаем на НОД. Если мы сократили на НОД, то
    // это максимально возможно ужатая версия чисел. Дальше никак.
    IntegerNumber reducedNumerator = this->numerator->quotient(IntegerNumber(gcd.getNumbers(), false));
    NaturalNumber denominator = *this->denominator;
    NaturalNumber reducedDenominator = denominator.quotient(gcd);

    // Удаляем указатели на старые числа, тупо ставим новые.
    delete this->numerator;
    delete this->denominator;

    this->numerator = new IntegerNumber(reducedNumerator);
    this->denominator = new NaturalNumber(reducedDenominator);
}

// Q-2: Проверка сокращенного дробного на целое,
// если рациональное число является целым, то «да», иначе «нет»
bool RationalNumber::isInteger() const {
    // Раз оно сокращенное, то знаменатель априори должен быть равен 1.
    // Если он не равен 1, то значит число точняк не целое.
    // (Потому что любое целое - это дробь со знаменателем 1: 56 = 56/1)
    const std::vector<uint8_t> &denomDigits = this->denominator->getNumbers();

    if (denomDigits.size() == 1) {
        if (denomDigits[0] == 1) {
            return true;
        }
    }
    return false;
}

// Q-3: Преобразование целого в дробное.
RationalNumber RationalNumber::fromInteger(const IntegerNumber &intNum) {
    NaturalNumber denominator(1);
    return RationalNumber(intNum, denominator);
}

// Q-4: Преобразование сокращенного дробного в целое.
IntegerNumber RationalNumber::toInteger(const RationalNumber &other) const {
    // Проверка, что оно реально может быть целым (представимо как целое).
    // Если нет, то толку продолжать...
    if (!this->isInteger()) {
        throw UniversalStringException(
                "RationalNumber::toInteger: cannot convert fraction to integer, "
                "denominator must be 1. Current value: " + this->toString()
        );
    }

    // Раз оно представимо как целое, то знаменатель = 1,
    // про него забываем, нас волнует только числитель.
    return IntegerNumber(*this->numerator);
}
