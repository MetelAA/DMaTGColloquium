//
// Created by Artem on 03.11.2025.
//

#ifndef DMATGCOLLOQUIUM_RATIONALNUMBER_H
#define DMATGCOLLOQUIUM_RATIONALNUMBER_H

#include "NaturalNumber.h"
#include "IntegerNumber.h"

class RationalNumber {
public:
    RationalNumber(const IntegerNumber& numeratorA, const NaturalNumber& denominatorA){
        numerator = new IntegerNumber(numeratorA);
        denominator = new NaturalNumber(denominatorA);
    }
    RationalNumber(long long numeratorA, long long denominatorA); //решение для облегченного тестирования, потом будет выпелено
    RationalNumber(const std::string& numeratorA, const std::string& denominatorA);
    RationalNumber(const std::string& s); //основной конструктор

    RationalNumber(const RationalNumber& other): numerator(new IntegerNumber(*other.numerator)), denominator(new NaturalNumber(*other.denominator)){}
    RationalNumber(RationalNumber&& other) noexcept: numerator(other.numerator), denominator(other.denominator){
        other.denominator = nullptr;
        other.numerator = nullptr;
    }

    RationalNumber& operator=(const RationalNumber& other){
        if (this != &other){
            delete this->denominator;
            delete this->numerator;
            this->numerator = new IntegerNumber(*other.numerator);
            this->denominator = new NaturalNumber(*other.denominator);
        }
        return *this;
    }

    RationalNumber& operator=(RationalNumber&& other) noexcept{
        if (this != &other) {
            this->numerator = other.numerator;
            this->denominator = other.denominator;
            other.denominator = nullptr;
            other.numerator = nullptr;
        }
        return *this;
    }

    ~RationalNumber(){
        delete numerator;
        delete denominator;
    }

    static RationalNumber fromInteger(const IntegerNumber& intNum);
    std::string toString() const;
    const IntegerNumber& getIntegerNumerator() const noexcept;
    const NaturalNumber& getNaturalDenominator() const noexcept;
    void reduce();
    bool isInteger() const;
    IntegerNumber toInteger(const RationalNumber& other) const;
    RationalNumber add(const RationalNumber& other) const;
    RationalNumber subtract(const RationalNumber& other) const;
    RationalNumber multiply(const RationalNumber& other) const;
    RationalNumber division(const RationalNumber& other) const;

private:
    IntegerNumber* numerator; //числитель
    NaturalNumber* denominator;
};


#endif //DMATGCOLLOQUIUM_RATIONALNUMBER_H
