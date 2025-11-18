QT += core gui widgets

CONFIG += c++11

# Главный исходный файл
SOURCES += main.cpp \
    src/IntegerNumber.cpp \
    src/NaturalNumber.cpp \
    src/Polynomial.cpp \
    src/RationalNumber.cpp \
    src/Validator/Validator.cpp \
    src/Validator/Utils/Lexer.cpp \
    src/Validator/Utils/Parser.cpp \
    src/Validator/Utils/Token.cpp

# Заголовочные файлы
HEADERS += \
    src/IntegerNumber.h \
    src/NaturalNumber.h \
    src/Polynomial.h \
    src/RationalNumber.h \
    src/Validator/Validator.h \
    src/Exceptions/UniversalStringException.h \
    src/Validator/Utils/Lexer.h \
    src/Validator/Utils/Parser.h \
    src/Validator/Utils/Token.h \
    src/Validator/Utils/Monom.h
