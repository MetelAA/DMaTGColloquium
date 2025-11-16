//
// Created by Artem on 02.11.2025.
//

#include <algorithm>
#include "IntegerNumber.h"
#include "Exceptions/UniversalStringException.h"

bool IntegerNumber::isNegative() const noexcept {
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

std::string IntegerNumber::toString() const {
    return (isNegativeFlag ? "-" : "") + this->number->toString();
}

const std::vector<uint8_t>& IntegerNumber::getNumbers() const noexcept {
    return this->number->getNumbers();
}

// Z-8: Умножение целых чисел.
IntegerNumber IntegerNumber::multiply(const IntegerNumber &other) const {
    // getSign() дает ноль, если число равно нулю. Поэтому юзаю его.
    // Тут простая проверка, если первое или второе число ноль - умножение равно нулю.
    uint8_t numberSign = this->getSign();
    uint8_t otherSign = other.getSign();
    if (numberSign == 0 || otherSign == 0) {
        return IntegerNumber("0");
    }

    // Берем модуль числа для умножения и умножаем числа.
    NaturalNumber numberAbs = this->abs();
    NaturalNumber otherAbs = other.abs();
    NaturalNumber multiplyAbs = numberAbs.multiply(otherAbs);

    // Вспоминаем, какой знак был у нашего числа.
    // Отрицательное, если знаки у обоих чисел не одинаковые.
    bool resultIsNegative = numberSign != otherSign;

    return IntegerNumber(multiplyAbs.getNumbers(), resultIsNegative);
}

//Z9: Частное от деления целого на целое (делитель отличен от нуля)
IntegerNumber IntegerNumber::quotient(const IntegerNumber &other) const {
    if (other.getSign() == 0) {
        throw UniversalStringException("you cannot divide by zero");
    }

    NaturalNumber dividend = this->abs();
    const NaturalNumber divisor = other.abs();

    if (dividend.cmp(&divisor) == 1) { // divisor > dividend => поделить не можем, вернем 0
        return IntegerNumber(std::vector<uint8_t>{0}, false);
    }

    // Бинарный поиск частного
    NaturalNumber left("1");
    NaturalNumber right = dividend; // Максимально возможное частное

    NaturalNumber quotient_natural("0");

    while (left.cmp(&right) != 2) { // left <= right
        // mid = (left + right) / 2
        NaturalNumber sum = left.add(right);

        // Реализуем деление на 2 через битовые операции (поразрядное деление)
        std::vector<uint8_t> sum_digits = sum.getNumbers();
        std::vector<uint8_t> mid_digits;
        uint8_t carry = 0;

        // Деление в столбик на 2
        for (int i = sum_digits.size() - 1; i >= 0; i--) {
            uint16_t current = sum_digits[i] + carry * 10;
            uint8_t digit = current / 2;
            carry = current % 2;
            mid_digits.push_back(digit);
        }
        std::reverse(mid_digits.begin(), mid_digits.end());

        // Убираем ведущие нули
        while (mid_digits.size() > 1 && mid_digits.back() == 0) {
            mid_digits.pop_back();
        }

        NaturalNumber mid(mid_digits);

        // product = mid * divisor
        NaturalNumber product = divisor.multiply(mid);

        int cmp_result = product.cmp(&dividend);

        if (cmp_result == 0) { // product == dividend
            quotient_natural = mid;
            break;
        } else if (cmp_result == 2) { // product > dividend
            // mid слишком большой, ищем в левой половине
            NaturalNumber one("1");
            right = mid.subtract(one);
        } else { // product < dividend
            // mid подходит, но может быть большее
            quotient_natural = mid;
            NaturalNumber one("1");
            left = mid.add(one);
        }
    }

    // Конвертируем NaturalNumber в IntegerNumber
    IntegerNumber result(quotient_natural.getNumbers(), false);

    return this->getSign() != other.getSign() ? result.negate() : result;
}

//Z10: Остаток от деления целого на целое (делитель отличен от нуля)
IntegerNumber IntegerNumber::remainder(const IntegerNumber &other) const {
    if (other.getSign() == 0) {
        throw UniversalStringException("you cannot divide by zero");
    }

    const NaturalNumber dividend_abs = this->abs();
    const NaturalNumber divisor_abs = other.abs();

    // Если делимое меньше делителя по модулю
    if (dividend_abs.cmp(&divisor_abs) == 1) {
        // Если делимое отрицательное, корректируем
        if (this->isNegative()) {
            return IntegerNumber(divisor_abs.subtract(dividend_abs).getNumbers(), false);
        }
        return *this;
    }

    // Вычисляем частное и произведение
    const IntegerNumber quotient_result = this->quotient(other);
    const IntegerNumber product = other.multiply(quotient_result);
    IntegerNumber remainder = this->subtract(product);

    // Если остаток отрицательный - корректируем (добавляем |divisor|)
    if (remainder.isNegative()) {
        remainder = remainder.add(IntegerNumber(divisor_abs.getNumbers(), false));
    }

    return remainder;
}

//
//Герасимов Дмитрий 4385
//

//Z1: Абсолютная величина числа, результат - натуральное
NaturalNumber IntegerNumber::abs() const{
    return NaturalNumber(*this->number);
}

//Z2: Определение положительности числа (2 - положительное, 0 — равное нулю, 1 - отрицательное)
uint8_t IntegerNumber::getSign() const {
    if (!this->number->isNotEqualZero()) {
        return 0;
    }
    return this->isNegativeFlag ? 1 : 2;
}

//Z3: Умножение целого на (-1)
IntegerNumber IntegerNumber::negate() const {
    if (!this->number->isNotEqualZero())
        return *this;
    return IntegerNumber(this->number->getNumbers(), !this->isNegativeFlag);
}

//Z4: Преобразование натурального в целое
IntegerNumber IntegerNumber::toInteger(const NaturalNumber &other) {
    return IntegerNumber(other.getNumbers(), false);
}

//Z5: Преобразование целого неотрицательного в натуральное
NaturalNumber IntegerNumber::toNatural(const IntegerNumber& other) const {
    if (other.isNegativeFlag) {
        throw UniversalStringException("to convert an integer to a natural, it must be greater than or equal to 0.");
    }
    return NaturalNumber(*other.number);
}

//Z6: Сложение целых чисел
IntegerNumber IntegerNumber::add(const IntegerNumber &other) const {
    NaturalNumber absThis = this->abs();
    NaturalNumber absOther = other.abs();
    size_t maxSize = std::max(absThis.getNumbers().size(), absOther.getNumbers().size());
    std::vector<uint8_t> resultNumbers(maxSize + 1, 0);

    if (this->getSign() == other.getSign()) {
        NaturalNumber sum = absThis.add(absOther);
        bool isNeg = this->isNegativeFlag;
        return IntegerNumber(sum.getNumbers(), isNeg);
    } else {
        uint8_t cmp = absThis.cmp(&absOther);
        if (cmp == 0) {
            return IntegerNumber(std::vector<uint8_t>{0}, false);
        } else if (cmp == 2) {
            NaturalNumber diff = absThis.subtract(absOther);
            return IntegerNumber(diff.getNumbers(), this->isNegativeFlag);
        } else {
            NaturalNumber diff = absOther.subtract(absThis);
            return IntegerNumber(diff.getNumbers(), other.isNegativeFlag);
        }
    }
}

//Z7: Вычитание целых чисел
IntegerNumber IntegerNumber::subtract(const IntegerNumber& other) const {
    IntegerNumber negOther = other.negate();
    return this->add(negOther);
}

//
//Герасимов Дмитрий 4385
//
