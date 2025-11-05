//
// Created by Artem on 02.11.2025.
//

#ifndef DMATGCOLLOQUIUM_NATURALNUMBER_H
#define DMATGCOLLOQUIUM_NATURALNUMBER_H

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

class NaturalNumber {
public:

    explicit NaturalNumber(const std::vector<uint8_t> &CpNumbers);
    NaturalNumber(unsigned long long a); //решение для облегченного тестирования, потом будет выпелено
    NaturalNumber(const std::string& a); //основной конструктор

    NaturalNumber(const NaturalNumber& other) : numbers(other.numbers) {}
    NaturalNumber(NaturalNumber&& other) noexcept : numbers(std::move(other.numbers)) {
        other.numbers = {0};
    }

    NaturalNumber& operator=(const NaturalNumber& other) {
        if (this != &other) {
            numbers = other.numbers;
        }
        return *this;
    }
    NaturalNumber& operator=(NaturalNumber&& other) noexcept {
        if (this != &other) {
            numbers = std::move(other.numbers);
        }
        return *this;
    }

    std::string toString() noexcept;
    const std::vector<uint8_t>& getNumbers() noexcept;
    uint8_t cmp(const NaturalNumber* other) const;
    bool isEqualZero() const;
    void increment();
    NaturalNumber add(const NaturalNumber& other) const;
    NaturalNumber subtract(const NaturalNumber& other) const;
    NaturalNumber multiplyByDigit(std::size_t b) const;
    NaturalNumber multiplyByPowerOfTen(std::size_t k) const;
    NaturalNumber multiply(const NaturalNumber& other) const;
    NaturalNumber subtractMultiplied(const NaturalNumber& other, std::size_t c) const;
    NaturalNumber getFirstDivisionDigit(const NaturalNumber& other) const;
    NaturalNumber quotient(const NaturalNumber& other) const;
    NaturalNumber remainder(const NaturalNumber& other) const;
    NaturalNumber GCD(const NaturalNumber& other) const;
    NaturalNumber LCM(const NaturalNumber& other) const;


private:
    std::vector<uint8_t> numbers;
};


#endif //DMATGCOLLOQUIUM_NATURALNUMBER_H
