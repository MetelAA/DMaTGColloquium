//
// Created by Artem on 03.11.2025.
//

#include "Polynomial.h"
#include "Exceptions/UniversalStringException.h"

#include <algorithm>

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

//P10: Остаток от деления полиномов
Polynomial Polynomial::remainder(const Polynomial &other) const {
    // Находим неполное частное от деления
    Polynomial incomplete_quotient = this->quotient(other);
    // Умножаем неполное частное на делитель
    Polynomial product = incomplete_quotient.multiply(other);
    // Вычитаем из исходного полинома произведение, получаем остаток от деления
    return this->subtract(product);
}

//P11: НОД полиномов
Polynomial Polynomial::GCD(const Polynomial &other) const {
    // Так как 0 делитя на все, что угодно, мы не можем точно определить НОД
    // Возвращаем константу, так как на нее делится все, но в ней нет корней
    if(this->getDegree() == 1 && this->coefficients[0].getIntegerNumerator().getSign() == 0 ||
       other.getDegree() == 1 && other.coefficients[0].getIntegerNumerator().getSign() == 0){
        throw UniversalStringException("Polynomial::GCD: wrong argument, one of the polynomials is equivalent to 0, it is impossible to uniquely determine the GCD");
    }

    // Если среди переданных значений все корректные
    // Инициализируем 2 полинома для применения к ним алгоритма Евклида
    Polynomial* polynom1 = new Polynomial(*this);
    Polynomial* polynom2 = new Polynomial(other);
    // Находим остаток от деления первого на второй
    Polynomial* remainder = new Polynomial(this->remainder(other));

    // Пока остаток не станет равен 0, то есть пока количетво коэффициентов больше 1
    // Или пока единственный коэффициент не 0, применяем алгоритм Евклида
    while(!(remainder->getDegree() == 1 &&
            remainder->coefficients[0].getIntegerNumerator().getSign() == 0)){
        // Очищаем память от первого полинома, его мы уже обработали
        delete polynom1;
        // Новыми данными для применения алгоритма становятся
        // второй полином и остаток от деления первого на второй
        polynom1 = polynom2;
        polynom2 = remainder;
        // Вычисляем новый остаток
        remainder = new Polynomial(polynom1->remainder(*polynom2));
    }

    // Сохраняем НОД, это предпоследий остаток,
    // то есть тот полином, на который делили в последний раз
    Polynomial gcd = Polynomial(*polynom2);

    // Так как все полиномы делятся на константу, и она не несет смысловой нагрузки,
    // сделаем многочлен приведенным (старший коэффициент 1)
    // Для этого нужно поделить на старший коэффицинет или умножить на обратный к нему
    RationalNumber highest_coeff = polynom2->coefficients[polynom2->getDegree()];
    RationalNumber unit = RationalNumber(IntegerNumber({1}, false), NaturalNumber({1}));
    RationalNumber anti_highest_coeff = unit.division(highest_coeff);

    gcd = gcd.multiplyByRational(anti_highest_coeff);

    // Очищаем память
    delete polynom1;
    delete polynom2;
    delete remainder;

    return gcd;
}

//P12: Производная полинома
Polynomial Polynomial::derivative() const {
    // Проверим степень полинома
    // Если степень 0, значит, полином представляет обой константу,
    // следоватеьно, производная равна 0, особый случай, обрабатываетя отдельно
    if(this->getDegree() == 0){
        return Polynomial({RationalNumber(IntegerNumber({0}, false), NaturalNumber{1})});
    }

    // Иначе создаем новый объект для хранения производной
    Polynomial derivative = Polynomial(*this);
    // Начиная с 1 степени переменной, перебираем все коэффициенты
    for(size_t i = 1; i < this->coefficients.size(); i++){
        // Переводим показатель степени в дробное представление,
        // чтобы перемножить с дробным коэффциентом с помощью метода дроби
        std::string str = std::to_string(i) + "/1";
        RationalNumber power = RationalNumber(str);
        // Так как при дифференцировании степень переменной понижается,
        // результат умножения коэффициента на показатель степени записываем
        // в ячейку, соответствующую меньшей степени переменной
        derivative.coefficients[i - 1] = this->coefficients[i].multiply(power);
    }
    // Так как копировали исходный полином, а степень производной на 1 меньше,
    // последняя ячейка, которая соответствует старшему коэффициенту, больше не нужна
    derivative.coefficients.pop_back();
    return derivative;
}

//P13: Многочлен с корнями 1 кратности
Polynomial Polynomial::makeSquareFree() const {
    // Найдем производную полинома
    Polynomial derivative = this->derivative();
    // НОД полином и его производной будет содеражть все кратные корни
    Polynomial gcd = this->GCD(derivative);
    // Поделим на НОД, чтобы остались только корни кратности 1
    return derivative.remainder(gcd);
}

//P7: Вынесение из многочлена НОК знаменателей коэффициентов и НОД числителей
Polynomial Polynomial::factorOut() const {
	NaturalNumber nod = coefficients.at(0).getIntegerNumerator().abs(); //предположили, что первые числа будут НОД и НОК
	NaturalNumber nok = coefficients.at(0).getNaturalDenominator();

	for(size_t i = 1; i < coefficients.size(); i++){
		nod = nod.GCD(coefficients.at(i).getIntegerNumerator().abs()); //находим НОД из последовательного алгоритма Евклида для всех коэффициентов
		nok = nok.LCM(coefficients.at(i).getNaturalDenominator()); //пользуемся формулой связи НОД и НОК последовательно для всех коэффициентов
	}

	return this->multiplyByRational(RationalNumber(IntegerNumber(nok.getNumbers(), false), nod)); //получившийся полином = НОК/НОД * исходный полином
}

//P8: Умножение многочленов
Polynomial Polynomial::multiply(const Polynomial &other) const {
	RationalNumber zero(
		IntegerNumber(std::vector<uint8_t>{0}, false),
		NaturalNumber(std::vector<uint8_t>{1})
	);

	// Проверка на нулевой полином (всё коэффициенты = 0)
	bool thisZero = true;
	for (const auto &c : this->coefficients)
		if (c.getIntegerNumerator().abs().isNotEqualZero()) { thisZero = false; break; }
	bool otherZero = true;
	for (const auto &c : other.coefficients)
		if (c.getIntegerNumerator().abs().isNotEqualZero()) { otherZero = false; break; }
	if (thisZero || otherZero) return Polynomial({zero});

	size_t n = this->coefficients.size();
	size_t m = other.coefficients.size();

	// Резервируем результат и инициализируем нулями — один объект RationalNumber на слот
	std::vector<RationalNumber> resultCoeffs;
	resultCoeffs.assign(n + m - 1, zero);

	// Классическое O(n*m) умножение, но стараемся минимизировать копии
	for (size_t i = 0; i < n; ++i) {
		const RationalNumber &ai = this->coefficients[i];
		// если ai == 0 — пропускаем
		if (!ai.getIntegerNumerator().abs().isNotEqualZero()) continue;

		for (size_t j = 0; j < m; ++j) {
			const RationalNumber &bj = other.coefficients[j];
			if (!bj.getIntegerNumerator().abs().isNotEqualZero()) continue;

			// получаем произведение — предполагается, что multiply() возвращает уже новый RationalNumber
			RationalNumber prod = ai.multiply(bj);

			// добавляем в слот — предполагается, что add() мутирует левый аргумент (как у тебя)
			resultCoeffs[i + j] = resultCoeffs[i + j].add(prod);
			// не вызываем reduce() здесь!
		}
	}

	// Один проход: сократить дроби в конце — намного реже, чем при каждом add.
	for (auto &r : resultCoeffs) r.reduce();

	return Polynomial(resultCoeffs);
}


//P9: Частное от деления многочлена на многочлен при делении с остатком
//P9: Частное от деления многочлена на многочлен при делении с остатком
Polynomial Polynomial::quotient(const Polynomial &other) const {
    std::vector<RationalNumber> dividendCoefficientVector = coefficients; // делимое
    std::vector<RationalNumber> divisorCoefficientVector = other.coefficients; // делитель

    RationalNumber zero(IntegerNumber({0}, false), NaturalNumber({1}));

    // Проверка деления на ноль
    if (!divisorCoefficientVector.back().getIntegerNumerator().abs().isNotEqualZero()) {
        throw UniversalStringException("you cannot divide by zero");
    }

    size_t divisorSize = divisorCoefficientVector.size();
    size_t dividendSize = dividendCoefficientVector.size();

    // Если делимое меньше делителя — частное = 0
    if (dividendSize < divisorSize) {
        return Polynomial({zero});
    }

    // Результат (частное)
    std::vector<RationalNumber> quotient(dividendSize - divisorSize + 1, zero);

    // Копия делимого для работы
    Polynomial remainder(dividendCoefficientVector);

    RationalNumber divisorLeading = divisorCoefficientVector.back();

    // Основной цикл деления "в столбик"
    for (int i = static_cast<int>(dividendSize) - static_cast<int>(divisorSize); i >= 0; --i) {
        // Если остаток обнулился, выходим
        if (remainder.coefficients.back().getIntegerNumerator().abs().isNotEqualZero() == false) {
            remainder.coefficients.pop_back();
            if (remainder.coefficients.size() < divisorSize)
                break;
        }

        // Вычисляем текущий коэффициент частного
        RationalNumber coeff = remainder.coefficients.back().division(divisorLeading);
        quotient[i] = coeff;

        // Формируем делитель * coeff * x^i
        std::vector<RationalNumber> shifted(i, zero);
        for (auto d : divisorCoefficientVector)
            shifted.push_back(d.multiply(coeff));

        Polynomial toSubtract(shifted);
        remainder = remainder.subtract(toSubtract);

        // Удаляем ведущие нули в остатке
        while (remainder.coefficients.size() > 1 &&
               !remainder.coefficients.back().getIntegerNumerator().abs().isNotEqualZero()) {
            remainder.coefficients.pop_back();
        }

        if (remainder.coefficients.size() < divisorSize)
            break;
    }

    // Удаляем ведущие нули в частном
    while (quotient.size() > 1 &&
           !quotient.back().getIntegerNumerator().abs().isNotEqualZero()) {
        quotient.pop_back();
    }

    return Polynomial(quotient);
}



//P1: Сложение многочленов
Polynomial Polynomial::add(const Polynomial &other) const {
    const std::vector<RationalNumber>& longer = (this->coefficients.size() >= other.coefficients.size())
                                                 ? this->coefficients
                                                 : other.coefficients;
    const std::vector<RationalNumber>& shorter = (this->coefficients.size() >= other.coefficients.size())
                                                 ? other.coefficients
                                                 : this->coefficients;

    std::vector<RationalNumber> resultCoeffs;
    resultCoeffs.reserve(longer.size());

    for (size_t i = 0; i < shorter.size(); ++i) {
        resultCoeffs.push_back(longer.at(i).add(shorter.at(i)));
    }

    for (size_t i = shorter.size() ; i < longer.size(); ++i) {
        resultCoeffs.push_back(longer.at(i));
    }

    for (size_t i = resultCoeffs.size() - 1; i >= 1; --i) {
        if (!resultCoeffs.at(i).getIntegerNumerator().abs().isNotEqualZero())
            resultCoeffs.pop_back(); //примнение pop_back корректно, тк мы имеем право удалять только ведущие нули, если коэффицент при наибольшей степени не 0, то даже не рассматриваем оставшиеся
            //также последний элемент веткора даже при равенстве нулю не убираем, тк пустой вектор коээфициентов для полинома не корректен
        else
            break;
    }

    for (auto & resultCoeff : resultCoeffs) {
        if (resultCoeff.getIntegerNumerator().abs().isNotEqualZero()){
            resultCoeff.reduce();
        }
    }

    return Polynomial(resultCoeffs);
}

//P2: Вычитание многочленов
Polynomial Polynomial::subtract(const Polynomial &other) const {

    std::vector<RationalNumber> otherMinusCoeffs;
    otherMinusCoeffs.reserve(other.coefficients.size());
    for (const RationalNumber& pol : other.coefficients) {
        otherMinusCoeffs.emplace_back(RationalNumber(pol.getIntegerNumerator().negate(), pol.getNaturalDenominator()));
    }

    return this->add(Polynomial(otherMinusCoeffs));
}

//P3: Умножение многочлена на рациональное число
Polynomial Polynomial::multiplyByRational(const RationalNumber &b) const {
    if (!b.getIntegerNumerator().abs().isNotEqualZero()){
        return Polynomial(std::vector<RationalNumber>{RationalNumber(IntegerNumber(std::vector<uint8_t>{0}, false), NaturalNumber(std::vector<uint8_t>{1}))});
    }
    std::vector<RationalNumber> result;
    result.reserve(this->coefficients.size());
    for (std::size_t i = 0; i < this->coefficients.size(); ++i) {
        result.push_back(this->coefficients[i].multiply(b));

    }
    for (auto & resultCoeff : result) {
        resultCoeff.reduce();
    }

    return Polynomial(result);
}

//P-4 умножение полинома на x в k-ой степени
Polynomial Polynomial::multiplyByXInKPower(std::size_t k) const {
    std::vector<RationalNumber> result;
    RationalNumber zero(IntegerNumber({0}, false), NaturalNumber(std::vector<uint8_t>{1}));
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
    return this->coefficients[this->coefficients.size() - 1];
}

//P-6 Получение степени многочлена
std::size_t Polynomial::getDegree() const {
    return this->coefficients.size() - 1;
}
