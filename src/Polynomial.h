//
// Created by Artem on 03.11.2025.
//

#ifndef DMATGCOLLOQUIUM_POLYNOMIAL_H
#define DMATGCOLLOQUIUM_POLYNOMIAL_H

#include "RationalNumber.h"



class Polynomial {
public:
    struct rationalSupport{
        long long numerator, denominator;
    };

    explicit Polynomial(const std::vector<RationalNumber> &coefficients) : coefficients(coefficients) {}

    Polynomial(const std::vector<std::string>& coefficientsA); //сюда попадает строка в прямом поряде, т.е. если было x^3+2/5x^3+3x^3+4 то сюда должно прийти {1/1,2/5,3/1,4/1}
    Polynomial(const std::vector<rationalSupport>& coefficientsA); //ограничения на numerator и denominator как в обычных rational. Числа также в прямом порядке
    // например чтобы передать 3/4x^2 - 2x + 2/5 нужно передать { {3, 4}, {-2, 1}, {2, 5} }

    Polynomial(const Polynomial& other) : coefficients(other.coefficients) {}
    Polynomial(Polynomial&& other) noexcept : coefficients(std::move(other.coefficients)) {}

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            this->coefficients = other.coefficients;
        }
        return *this;
    }
    Polynomial& operator=(Polynomial&& other) noexcept {
        if (this != &other) {
            this->coefficients = std::move(other.coefficients);
        }
        return *this;
    }

    const std::vector<RationalNumber>& getCoefficients() noexcept;
    std::string toString() const;
    Polynomial add(const Polynomial& other) const;
    Polynomial subtract(const Polynomial& other) const;
    Polynomial multiplyByRational(const RationalNumber& b) const;
    Polynomial multiplyByXInKPower(std::size_t k) const;
    RationalNumber getLeadingCoefficient() const;
    std::size_t getDegree() const;
    Polynomial factorOut() const;
    Polynomial multiply(const Polynomial& other) const;
    Polynomial quotient(const Polynomial& other) const;
    Polynomial remainder(const Polynomial& other) const;
    Polynomial GCD(const Polynomial& other) const;
    Polynomial derivative() const;
    Polynomial makeSquareFree() const;

private:
    std::vector<RationalNumber> coefficients;
};


#endif //DMATGCOLLOQUIUM_POLYNOMIAL_H
