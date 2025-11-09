#include "Validator.h"
#include <cctype>
#include <algorithm>

void Validator::validateNaturalNumber(std::string &number) {
    if(number.empty())
        throw UniversalStringException("Validator::validateNaturalNumber(): empty string.");

    for (auto el : number ) {
        if (!isdigit(el))
            throw UniversalStringException("Validator::validateNaturalNumber(): string contains non-digit characters.");
    }

    auto iterator = number.begin();

    // Удаляем ведущие нули, но оставляем хотя бы один ноль если строка состоит только из нулей
    while (iterator != number.end() && *iterator == '0')
        ++iterator;

    if (iterator == number.end())
        number = "0";  // Сохраняем "0" если все символы были нулями
    else
        number.erase(number.begin(), iterator);  // Удаляем ведущие нули
}

void Validator::validateIntegerNumber(std::string &number) {

    if (number.empty())
        throw UniversalStringException("Validator::validateIntegerNumber(): empty string.");

    bool isNegative = false;

    // Обрабатываем знак минуса
    if (number[0] == '-') {
        isNegative = true;
        number.erase(number.begin(), number.begin() + 1);  // Удаляем знак минуса для проверки
    }

    try {
        validateNaturalNumber(number);  // Проверяем как натуральное число
    }
    catch (UniversalStringException&) {
        throw UniversalStringException("Validator::validateIntegerNumber(): invalid number!");
    }

    // Восстанавливаем знак минуса если он был
    if (isNegative)
        number.insert(number.begin(), '-');

    if (number == "-0")
        throw UniversalStringException("Validator::validateIntegerNumber(): invalid number!");
}

void Validator::validateRationalNumber(std::string &number) {

    if (number.empty())
        throw UniversalStringException("Validator::validateRationalNumber(): empty string.");

    // Разделяем на числитель и знаменатель
    size_t delimiterPos = number.find('/');

    if (delimiterPos == std::string::npos)
        throw UniversalStringException("Validator::validateRationalNumber(): missing '/' in rational number.");

    std::string numerator = number.substr(0, delimiterPos);
    std::string denominator = number.substr(delimiterPos + 1);

    // Проверяем числитель (может быть отрицательным)
    try {
        validateIntegerNumber(numerator);
    }
    catch (UniversalStringException&) {
        throw UniversalStringException("Validator::validateRationalNumber(): invalid numerator!");
    }

    // Проверяем знаменатель (должен быть натуральным числом)
    try {
        validateNaturalNumber(denominator);
    }
    catch (UniversalStringException&) {
        throw UniversalStringException("Validator::validateRationalNumber(): invalid denominator!");
    }

    // Проверяем деление на ноль
    if (denominator == "0")
        throw UniversalStringException("Validator::validateRationalNumber(): Division by zero!");

    number = numerator + '/' + denominator;
}

std::vector<RationalNumber> Validator::validatePolynomial(std::string &number) {
    if (number.empty())
        throw UniversalStringException("Validator::validatePolynomial(): empty string.");

    // Лексический анализ строки
    Lexer lexer(number);
    std::vector<Token> tokens;

    try {
        lexer.CreateTokens();
    }
    catch (UniversalStringException&) {
        throw;
    }

    tokens = lexer.GetTokens();

    std::vector<Monom> polynom;

    // Синтаксический анализ токенов
    try {
        polynom = Parser::parse(tokens);
    }
    catch (UniversalStringException&) {
        throw;
    }

    // Сортируем мономы по возрастанию степени
    std::sort(polynom.begin(), polynom.end(),
              [](const Monom& a, const Monom& b) {
                  try {
                      unsigned long long deg_a = std::stoull(a.degree);
                      unsigned long long deg_b = std::stoull(b.degree);
                      return deg_a < deg_b;
                  } catch (...) {
                      return false;
                  }
              }
    );

    // Проверяем на дублирующиеся степени
    for (size_t i = 1; i < polynom.size(); i++) {
        if (polynom[i].degree == polynom[i-1].degree) {
            throw UniversalStringException("Duplicate degrees found");
        }
    }

    std::vector<RationalNumber> result;

    if (polynom.empty())
        throw UniversalStringException("Validator::validatePolynomial(): no monoms parsed.");

    // Проверяем корректность старшей степени
    try {
        validateNaturalNumber(polynom.back().degree);
    } catch (UniversalStringException&) {throw;}

    // Проверка на переполнение size_t
    try {
        size_t max_deg_test = std::stoull(polynom.back().degree);
        if (max_deg_test > SIZE_MAX - 1) {
            throw UniversalStringException("Validator::validatePolynomial(): degree too large for size_t!");
        }
    } catch (const std::out_of_range&) {
        throw UniversalStringException("Validator::validatePolynomial(): degree exceeds size_t range!");
    } catch (const std::invalid_argument&) {
        throw UniversalStringException("Validator::validatePolynomial(): invalid degree format!");
    }

    // Создаем вектор коэффициентов, где индекс = степень
    size_t max_deg = std::stoull(polynom.back().degree) + 1;
    result.resize(max_deg, RationalNumber("0/1"));  // Заполняем нулями

    // Заполняем коэффициенты для соответствующих степеней
    for (const auto& monom : polynom) {
        size_t deg = std::stoull(monom.degree);
        try {
            // Создаем рациональное число из строки формата "числитель/знаменатель"
            result[deg] = RationalNumber(monom.numerator + "/" + monom.denominator);
        } catch (UniversalStringException&) {throw;}
    }

    bool allZeros = true;
    for (auto& coeff : result) {
        if (coeff.toString() != "0/1") {
            allZeros = false;
            break;
        }
    }
    if (allZeros) {
        throw UniversalStringException("Validator::validatePolynomial(): all coefficients are zero!");
    }

    // Реверсируем вектор перед возвратом
    std::reverse(result.begin(), result.end());

    return result;
}