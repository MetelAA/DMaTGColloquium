#include "Lexer.h"
#include "../../Exceptions/UniversalStringException.h"
#include <memory>

Lexer::Lexer(std::string input) : input_(std::move(input)) {}

void Lexer::CreateTokens() {
    if (input_.empty())
        throw UniversalStringException("Lexer.CreateTokens(): empty input string!");

    for (int i = 0; i < input_.length(); i++) {
        char ch = input_[i];

        if (std::isspace(ch)) continue;  // Пропускаем пробельные символы

        // Если символ не цифра и не 'x', создаем токен оператора/скобки
        if (!std::isdigit(ch) && ch != 'x') {
            push_token(ch);
            continue;
        }

        // Если токенов еще нет, создаем новый токен
        if (tokens_.empty()) {
            push_token(ch);
            continue;
        }

        auto& last_token = tokens_.back();

        // Если предыдущий токен не число, создаем новый токен
        if (last_token.type != TokenType::Num) {
            push_token(ch);
            continue;
        }

        // Если текущий символ не 'x', добавляем к предыдущему числовому токену
        if (ch != 'x') {
            last_token.value += ch;
            continue;
        }

        // Специальная обработка: между числом и 'x' неявно подразумевается умножение
        push_token('*');  // Добавляем оператор умножения
        push_token('x');  // Добавляем переменную x
    }
}


const std::vector<Token>& Lexer::GetTokens() const{
    return tokens_;
}

void Lexer::push_token(char ch) {
    try {
        tokens_.push_back(Token::CreateToken(ch));  // Создаем токен из символа
    } catch (const UniversalStringException&) {
        throw UniversalStringException(
                std::string("Lexer::CreateTokens(): invalid token '") + ch + "'"
        );
    }
}