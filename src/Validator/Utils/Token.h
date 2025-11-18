#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <cctype>
#include "../../Exceptions/UniversalStringException.h"

/**
 * @brief Перечисление TokenType хранит все возможные типы токенов.
 */
enum class TokenType {
    Mul = '*',
    Add = '+',
    Sub = '-',
    Div = '/',
    Pow = '^',
    X = 'x',
    Num
};

/**
 * @brief Структура Token представляет отдельный токен.
 *
 * Может создавать токены из символа, сравнивать токены и хранить
 * значение и тип токена.
 */
struct Token {
    /**
     * @brief Создает токен из переданного символа.
     *
     * Проверяет символ и выбирает подходящий тип токена.
     *
     * @param symb Символ для создания токена.
     * @return Token Созданный токен.
     *
     * @throws TokenError Если символ не распознается как допустимый токен.
     */
    static Token CreateToken(char symb);

    /**
     * @brief Оператор сравнения двух токенов.
     *
     * Сравнивает тип и значение токенов.
     *
     * @param other Другой токен для сравнения.
     * @return true если токены одинаковые.
     * @return false если токены различаются.
     */
    bool operator==(const Token& other) const {
        return type == other.type && value == other.value;
    }

    TokenType type;      /**< Тип токена */
    std::string value;   /**< Значение токена */

private:
    /**
     * @brief Конструктор токена.
     * @param t Тип токена
     * @param s Символ токена
     */
    Token(TokenType t, char s) : type(t), value(1, s) {}
};

#endif // TOKEN_H