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
//  N-1
//  Сравнение чисел:
//  2 — текущее больше,
//  1 — текущее меньше,
//  0 — равны.
uint8_t NaturalNumber::cmp(const NaturalNumber* other)  {
    // По условию экземпляры валидные и не содержат незначащих нулей,
    // поэтому сравниваем прямо по длине и по цифрам.
    if (this->numbers.size() > other->numbers.size()) return 2;
    if (this->numbers.size() < other->numbers.size()) return 1;

    // Длины равны — сравниваем по старшему разряду к младшему.
    for (size_t i = this->numbers.size(); i-- > 0;) {
        uint8_t a = this->numbers[i];
        uint8_t b = other->numbers[i];
        if (a > b) return 2;
        if (a < b) return 1;
    }
    return 0;
}



//  N-3
//  Прибавляет 1 к текущему числу.
void NaturalNumber::increment() {
    if (numbers.empty()) {
        numbers = {1};
        return;
    }
    uint8_t carry = 1;
    for (size_t i = 0; i < numbers.size() && carry; ++i) {
        uint8_t sum = numbers[i] + carry;
        numbers[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) numbers.push_back(carry);
}

//  N-4
//  Сложение двух чисел.
NaturalNumber NaturalNumber::add(const NaturalNumber& other)  {
    size_t n = numbers.size();
    size_t m = other.numbers.size();
    size_t maxlen = std::max(n, m);
    std::vector<uint8_t> res;
    res.reserve(maxlen + 1);
    uint8_t carry = 0;

    // Складываем цифры по разрядам
    for (size_t i = 0; i < maxlen; ++i) {
        uint8_t a = (i < n) ? numbers[i] : 0;
        uint8_t b = (i < m) ? other.numbers[i] : 0;
        uint8_t s = a + b + carry;
        res.push_back(s % 10);
        carry = s / 10;
    }
    if (carry) res.push_back(carry);
    return NaturalNumber(res);
}

//  N-5
//  Вычитание одного числа из другого.
//  Если второе больше первого — ошибка.
NaturalNumber NaturalNumber::subtract(const NaturalNumber& other) {
    uint8_t comparison = this->cmp(&other);
    if (comparison == 1) {
        std::string msg = "NaturalNumber::SUB_NN_N: subtrahend larger than minuend";
        throw UniversalStringException(msg);
    }
    if (comparison == 0) return NaturalNumber(std::vector<uint8_t>{0});
    std::vector<uint8_t> res(numbers.size(), 0);
    int borrow = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        int a = numbers[i];
        int b = (i < other.numbers.size()) ? other.numbers[i] : 0;
        int diff = a - b - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[i] = static_cast<uint8_t>(diff);
    }
    return NaturalNumber(res);
}
// N - 6
//  Умножение на одну цифру (0–9).
NaturalNumber NaturalNumber::multiplyByDigit(std::size_t d) {
    if (d > 9) {
        std::string msg = "NaturalNumber::MUL_ND_N: digit out of range (" + std::to_string(d) + ")";
        throw UniversalStringException(msg);
    }
    if (d == 0) return NaturalNumber(std::vector<uint8_t>{0});
    std::vector<uint8_t> res;
    res.reserve(numbers.size() + 1);
    unsigned int carry = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        unsigned int prod = static_cast<unsigned int>(numbers[i]) * d + carry;
        res.push_back(static_cast<uint8_t>(prod % 10));
        carry = prod / 10;
    }
    while (carry > 0) {
        res.push_back(static_cast<uint8_t>(carry % 10));
        carry /= 10;
    }
    return NaturalNumber(res);
}

//  N-7
//  Умножение на 10^k (сдвиг влево на k позиций).
NaturalNumber NaturalNumber::multiplyByPowerOfTen(std::size_t k)  {
    // Если число равно 0
    if (numbers.size() == 1 && numbers[0] == 0)
        return NaturalNumber(std::vector<uint8_t>{0});

    std::vector<uint8_t> res;
    res.reserve(numbers.size() + k);

    // Добавляем k нулей (сдвигаем)
    for (size_t i = 0; i < k; ++i) res.push_back(0);

    // Добавляем все цифры исходного числа
    res.insert(res.end(), numbers.begin(), numbers.end());
    return NaturalNumber(res);
}


//  N-8
//  Умножение двух натуральных чисел (в столбик).

NaturalNumber NaturalNumber::multiply(const NaturalNumber& other) {
    // Если одно из чисел = 0 → результат = 0
    if ((numbers.size() == 1 && numbers[0] == 0) ||
        (other.numbers.size() == 1 && other.numbers[0] == 0)) {
        return NaturalNumber(std::vector<uint8_t>{0});
    }

    std::vector<uint8_t> res(numbers.size() + other.numbers.size(), 0);

    for (size_t i = 0; i < numbers.size(); ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < other.numbers.size() || carry; ++j) {
            unsigned long long cur = res[i + j] +
                                     static_cast<unsigned long long>(numbers[i]) *
                                     (j < other.numbers.size() ? other.numbers[j] : 0) +
                                     carry;
            res[i + j] = static_cast<uint8_t>(cur % 10);
            carry = static_cast<uint8_t>(cur / 10);
        }
    }

    // Удаляем ведущие нули
    while (res.size() > 1 && res.back() == 0)
        res.pop_back();

    return NaturalNumber(res);
}
