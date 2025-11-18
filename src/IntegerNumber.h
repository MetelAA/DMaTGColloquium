    //
    // Created by Artem on 02.11.2025.
    //

    #ifndef DMATGCOLLOQUIUM_INTEGERNUMBER_H
    #define DMATGCOLLOQUIUM_INTEGERNUMBER_H

    #include <cstdint>
    #include "NaturalNumber.h"

    class IntegerNumber {
    public:
        IntegerNumber(const std::vector<uint8_t>& numbers, bool isNegative): isNegativeFlag(isNegative){
            this->number = new NaturalNumber(numbers);
        };
        IntegerNumber(const std::string& s); //основной конструктор
        IntegerNumber(long long a); //решение для облегченного тестирования, потом будет выпелено

        IntegerNumber(const IntegerNumber& other): isNegativeFlag(other.isNegativeFlag), number(new NaturalNumber(*other.number)) {}
        IntegerNumber(IntegerNumber&& other) noexcept: isNegativeFlag(other.isNegativeFlag), number(other.number) {other.number = nullptr;}

        IntegerNumber& operator=(const IntegerNumber& other) {
            if (this != &other) {
                delete this->number;
                this->number = new NaturalNumber(*other.number);
                this->isNegativeFlag = other.isNegativeFlag;
            }
            return *this;
        }
        IntegerNumber& operator=(IntegerNumber&& other) noexcept {
            if (this != &other) {
                delete this->number;
                this->number = other.number;
                this->isNegativeFlag = other.isNegativeFlag;
                other.number = nullptr;
            }
            return *this;
        }

        ~IntegerNumber() {
            delete this->number;
        }


        bool isNegative() const noexcept;
        std::string toString() const;
        const std::vector<uint8_t>& getNumbers() const noexcept;
        NaturalNumber abs() const;
        uint8_t getSign() const;
        IntegerNumber negate() const;
        static IntegerNumber toInteger(const NaturalNumber& other);
        NaturalNumber toNatural(const IntegerNumber& other) const;
        IntegerNumber add(const IntegerNumber& other) const;
        IntegerNumber subtract(const IntegerNumber& other) const;
        IntegerNumber multiply(const IntegerNumber& other) const;
        IntegerNumber quotient(const IntegerNumber& other) const;
        IntegerNumber remainder(const IntegerNumber& other) const;


    private:
        NaturalNumber *number;
        bool isNegativeFlag;
    };


    #endif //DMATGCOLLOQUIUM_INTEGERNUMBER_H
