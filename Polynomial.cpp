//
// Created by Artem on 03.11.2025.
//

#include "Polynomial.h"
#include "Exceptions/UniversalStringException.h"



const std::vector<RationalNumber>& Polynomial::getCoefficients() noexcept {
    return this->coefficients;
}

std::string Polynomial::toString() const{
    if (this->coefficients.empty())
        throw UniversalStringException("atypical behavior, the vector of coefficients should not be empty");
    std::string result;
    for (int i = this->coefficients.size() - 1; i >= 0; --i) {
        result += this->coefficients.at(i).toString() + "x^" + std::to_string(i);
        if (i!=0)
            result += " + ";
    }
    return result;
}

Polynomial::Polynomial(const std::vector<std::string>& coefficientsA) {
    if (coefficientsA.empty())
        throw UniversalStringException("wrong argument, the string of numbers should not be empty");
    std::size_t i = coefficientsA.size() - 1;
    while (true) {
        this->coefficients.emplace_back(RationalNumber(coefficientsA[i]));
        if (i != 0)
            i--;
        else
            break;
    }
}

Polynomial::Polynomial(const std::vector<rationalSupport> &coefficientsA) {
    if (coefficientsA.empty())
        throw UniversalStringException("wrong argument, the string of numbers should not be empty");
    std::size_t i = coefficientsA.size() - 1;
    while (true) {
        this->coefficients.emplace_back(RationalNumber(coefficientsA[i].numerator, coefficientsA[i].denominator));
        if (i != 0)
            i--;
        else
            break;
    }
}

//P-4 умножение полинома на x в k-ой степени
Polynomial Polynomial::multiplyByXInKPower(std::size_t k) const {
    std::vector<RationalNumber> result;
    RationalNumber zero(IntegerNumber({0}, false), NaturalNumber({1}));
    //Добавляем в конец полинома количество нулей, равное k
    for (size_t i = 0; i < k; i++) {
        result.push_back(zero);
    }
    //Переписываем после нулей оставшиеся коэффиценты 
    for (size_t i = 0; i < this->coefficients.size(); i++) {
        result.push_back(this->coefficients[i]);
    }
    return Polynomial(result);
}

//P-5 Возврат старшего коэффицента
RationalNumber Polynomial::getLeadingCoefficient() const {
    RationalNumber result(this->coefficients[this->coefficients.size() - 1]);
    return result;
}

//P-6 Получение степени многочлена
std::size_t Polynomial::getDegree() const {
    std::size_t result = this->coefficients.size() - 1;
    return result;
}