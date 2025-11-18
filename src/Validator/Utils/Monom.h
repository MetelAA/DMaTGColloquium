#ifndef MONOM_H
#define MONOM_H

#include <string>

/**
 * @brief Структура Monom представляет математический моном.
 *
 * Моном - это выражение вида: coefficient * x^degree
 * где coefficient может быть целым числом или дробью.
 */
struct Monom {
    std::string degree;
    std::string numerator;
    std::string denominator;
};

#endif // MONOM_H