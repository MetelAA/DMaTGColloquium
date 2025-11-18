#include "Token.h"

Token Token::CreateToken(char symb) {
    switch (symb) {
        case '+': return Token(TokenType::Add, symb);
        case '*': return Token(TokenType::Mul, symb);
        case '-': return Token(TokenType::Sub, symb);
        case '/': return Token(TokenType::Div, symb);
        case '^': return Token(TokenType::Pow, symb);
        case 'x': return Token(TokenType::X, symb);
        default:
            if (std::isdigit(symb)) {
                return Token(TokenType::Num, symb);
            }

            throw UniversalStringException("Token::CreateToken: Invalid symb");
    }
}