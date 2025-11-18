cmake_minimum_required(VERSION 3.24)
project(DMaTGColloquium)

set(CMAKE_CXX_STANDARD 17)

add_executable(DMaTGColloquium main.cpp NaturalNumber.cpp NaturalNumber.h IntegerNumber.cpp IntegerNumber.h Exceptions/UniversalStringException.h RationalNumber.cpp RationalNumber.h Polynomial.cpp Polynomial.h Validator/Validator.cpp Validator/Validator.h Validator/Utils/Lexer.cpp Validator/Utils/Lexer.h Validator/Utils/Monom.h Validator/Utils/Parser.cpp Validator/Utils/Parser.h Validator/Utils/Token.cpp Validator/Utils/Token.h)
