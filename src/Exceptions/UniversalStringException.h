//
// Created by Artem on 03.11.2025.
//

#ifndef DMATGCOLLOQUIUM_UNIVERSALSTRINGEXCEPTION_H
#define DMATGCOLLOQUIUM_UNIVERSALSTRINGEXCEPTION_H


#include <exception>
#include <string>

class UniversalStringException : public std::exception{
public:
    UniversalStringException(const std::string& message): message{message}{}

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};



#endif //DMATGCOLLOQUIUM_UNIVERSALSTRINGEXCEPTION_H
