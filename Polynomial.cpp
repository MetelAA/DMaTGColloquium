//
// Created by Artem on 03.11.2025.
//

#include "Polynomial.h"
#include "Exceptions/UniversalStringException.h"



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
    std::vector<RationalNumber> multiplicandCoefficentVector = this->coefficients; //векторы коэффициентов
    std::vector<RationalNumber> multiplierCoefficentVector = other.coefficients;
    RationalNumber zero (IntegerNumber(std::vector<uint8_t>{0}, false), NaturalNumber(std::vector<uint8_t>{1}));

    if(!multiplicandCoefficentVector.back().getIntegerNumerator().abs().isNotEqualZero() || //в случае умножения на 0, получаем 0 сразу
       !multiplierCoefficentVector.back().getIntegerNumerator().abs().isNotEqualZero()){
        return Polynomial({zero});
    }

    Polynomial res ((std::vector<RationalNumber>){});    //алгоритм построен на последовательном суммировании всех произведений
    RationalNumber sum = RationalNumber(zero);                 //коэффициентов, степени переменной которых при сложении равны текущей для получившегося полинома
    size_t higherIndex;                 //индекс коэффициента при большей степени - верхний индекс
    size_t lowerIndex;                  //индекс коэффициента при меньшей степени - нижний индекс
    size_t maxDeg;                      //максимум из степеней перемножаемых многочленов
    size_t multiplicandCoefficentVectorInitialSize = multiplicandCoefficentVector.size(); //изначальный размер множимого многочлена
    size_t multiplierCoefficentVectorInitialSize = multiplierCoefficentVector.size();   //изначальный размер многочлена-множителя

    if(multiplicandCoefficentVectorInitialSize > multiplierCoefficentVectorInitialSize){ //узнаем, который многочлен имеет большую степень
        maxDeg = multiplicandCoefficentVectorInitialSize-1;
        for(size_t i = 0; i < multiplicandCoefficentVectorInitialSize-multiplierCoefficentVectorInitialSize; i++){ //заполняем меньший многочлен незначащими нулями, чтобы не выходить за границы контейнера
            multiplierCoefficentVector.push_back(zero);
        }
    }else{ //тоже самое, но зеркально, в случае, если больше иной многочлен
        maxDeg = multiplierCoefficentVectorInitialSize-1;
        for(size_t  i = 0; i < multiplierCoefficentVectorInitialSize-multiplicandCoefficentVectorInitialSize; i++){
            multiplicandCoefficentVector.push_back(zero);
        }
    }

    for(size_t i = 0; i < multiplicandCoefficentVectorInitialSize + multiplierCoefficentVectorInitialSize - 1; i++){ //цикл для коэффициентов при каждой степени
        sum = zero;    //сумма коэффициентов данной степени - переменная, в которой "приводятся подобные"
        if(i <= maxDeg){    //если степень, для которой мы считаем коэффициент, не превышает максимальную
            lowerIndex = 0; //то умножаем, начиная со свободного члена
        }else{              //если степень, для которой мы считаем коэффициент, больше максимальной степени
            lowerIndex = i - maxDeg; //то умножаем, начиная с той степени, при умножении с которой на максимальную, получим стпень, для которой мы считаем коэффициент
        } //пример: x^2+x+1 * 2x+3 - считаем для степени 3, которая выше максимальной (2). lowerIndex = 1 - это означает, получить 3 степень можно только при умножении максимальной на первую (lowerIndex)
        higherIndex = i - lowerIndex; //верхний индекс, указывает либо на коэффициент при степени равной текущей, либо на коэффициент при максимальной
        while(higherIndex >= lowerIndex){ //нас удовлетворяют все индексы (по совместительству степени), которые в сумме дают текущую степень
            if(higherIndex > lowerIndex){ //представлим это так, будто один индекс поднимается по лестнице, а другой спускается (смотрим инкремент и декремент в конце)
                sum.add(multiplicandCoefficentVector.at(higherIndex).multiply(multiplierCoefficentVector.at(lowerIndex))); //если индексы на разных уровнях
                sum.add(multiplierCoefficentVector.at(higherIndex).multiply(multiplicandCoefficentVector.at(lowerIndex))); //то умножаем пары
            }else{
                sum.add(multiplicandCoefficentVector.at(higherIndex).multiply(multiplierCoefficentVector.at(lowerIndex))); //если на одном уровне, то дважды не засчитываем
            } //пример: x^4+x^3+2x^2+x+1 * 3x^3+x^2+4x+3; считаем для x^2
            higherIndex--; //Верхняя и нижняя ступеньки: (2x^2)*3 и (x^2)*1
            lowerIndex++;  //Верхний спустился и нижний поднялся (теперь они на одной ступеньке): x*4x
        }   //такая логика исходит из очевидного - а именно из того, что умножение x^3 на что угодно (при натуральных степенях + '0') не даст x^2
        //ровно как и умножение свободного члена ('3') на что угодно не даст x^5 - максимум x^4.
        res.coefficients.push_back(sum); //закидываем коэффициент в ответ
    }
    return res;

}

//P9: Частное от деления многочлена на многочлен при делении с остатком
Polynomial Polynomial::quotient(const Polynomial &other) const {
    std::vector<RationalNumber> dividendCoefficientVector = coefficients; //вектора коэффициентов - делимого
    std::vector<RationalNumber> divisorCoefficientVector = other.coefficients; //делителя

    RationalNumber zero (IntegerNumber(std::vector<uint8_t>{0}, false), NaturalNumber(std::vector<uint8_t>{1}));
    if(!divisorCoefficientVector.back().getIntegerNumerator().abs().isNotEqualZero()){  //проверка на деление на 0
        throw UniversalStringException("you cannot divide by zero");
    }
    size_t divisorCoefficientVectorSize = divisorCoefficientVector.size();
    if(dividendCoefficientVector.size() < divisorCoefficientVectorSize){ //если делимое степенью (размером) меньше делителя, то сразу 0
        return Polynomial({zero});
    }

    Polynomial res ((std::vector<RationalNumber>) {}); //алгоритм копирует деление в столбик, а так как при деление в столбик мы начинаем со старшего члена
    std::vector<RationalNumber> tmpRes; //нужен временный вектор, в котором результат записан, как получается при счете
    //в ответ пойдет его отзеркаленный вид
    //то есть в этом векторе полином x^2 + 2x + 3 записан в прямом виде {1, 2, 3}
    RationalNumber currentCoefficient (zero); //Что мы делаем при делении в столбик? Берем такой множитель, чтобы старший член сократился, верно? Эта переменная и есть этот множитель - каждый раз новая
    Polynomial currentPolynomial ((std::vector<RationalNumber>) {}); //А потом пре делении в столбик мы вычитаем из делимого делитель, помноженный на currentCoefficient - получим от вычитания остаток - это он и есть - каждый раз новый
    for(size_t i = dividendCoefficientVector.size()-1; i > divisorCoefficientVectorSize - 2; i--){ //начинаем со старшей степени
        if(i < dividendCoefficientVector.size()){ //может случиться так, что сократится не только старший член, но и какой-то следующий - тогда нужно "пропустить" шаг итерации, поставив 0
            currentCoefficient = dividendCoefficientVector.back().division(divisorCoefficientVector.back()); //по определению
            for(size_t j = 0; j < dividendCoefficientVector.size()-divisorCoefficientVectorSize; j++){ //типа умножаем на x^n, но так как вектор коэфф. пуст, то нельзя использовать multiplyByXInKPower
                currentPolynomial.coefficients.push_back(zero); //заполняем, пока незначащими нулями
            }
            for(auto x : other.multiplyByRational(currentCoefficient).getCoefficients()){ //добавляем в старшие степени коэффициенты делителя, умноженного на currentCoefficient
                currentPolynomial.coefficients.push_back(x);
            } //получим currentPolynomial по определению
            this->subtract(currentPolynomial); //вычитаем из делимого
            tmpRes.push_back(currentCoefficient); //записываем коэффициент во временный результат - помним определение полного результата
            currentPolynomial.coefficients = {}; //обнуляем вектор currentPolynomial
        }else{
            tmpRes.push_back(zero); //коэффициент 0, если его нет в частном - можно еще раз посмотреть строку с условием
        }
    }
    if(tmpRes.size()){ //если частное не пусто
        for(std::vector<RationalNumber>::iterator it = tmpRes.end(); it >= tmpRes.end(); it--){ //идем в обратном порядке по временному ответу
            res.coefficients.push_back(*it); //записываем в ответ - нужно, чтобы частное x^2 + 2x + 3 было записано в векторе, как {3, 2, 1}
        }
    }
    return res;
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

    std::size_t diff = longer.size() - shorter.size();

    for (std::size_t i = 0; i < diff; ++i) {
        resultCoeffs.push_back(longer[i]);
    }

    for (std::size_t i = diff; i < longer.size(); ++i) {
        RationalNumber sum = longer[i].add(shorter[i - diff]);
        resultCoeffs.push_back(sum);
    }

    for (int i = 0; i < resultCoeffs.size(); ++i) { //и вообще эта ситуация возможно только при diff==0
        if (!resultCoeffs.at(i).getIntegerNumerator().abs().isNotEqualZero())
            resultCoeffs.pop_back(); //примнение pop_back корректно, тк мы имеем право удалять только ведущие нули, если коэффицент при наибольшей степени не 0, то даже не рассматриваем оставшиеся
        else
            break;
    }

    return Polynomial(resultCoeffs);
}

//P2: Вычитание многочленов
Polynomial Polynomial::subtract(const Polynomial &other) const {
    const std::vector<RationalNumber>& longer  = (this->coefficients.size() >= other.coefficients.size())
                                                 ? this->coefficients
                                                 : other.coefficients;
    const std::vector<RationalNumber>& shorter = (this->coefficients.size() >= other.coefficients.size())
                                                 ? other.coefficients
                                                 : this->coefficients;

    std::vector<RationalNumber> resultCoeffs;
    resultCoeffs.reserve(longer.size());

    std::size_t diff = longer.size() - shorter.size();

    bool thisIsLonger = (this->coefficients.size() >= other.coefficients.size());

    for (std::size_t i = 0; i < diff; ++i) {
        if (thisIsLonger)
            resultCoeffs.push_back(longer[i]);
        else
            resultCoeffs.push_back(RationalNumber(0, 1).subtract(longer[i]));
    }

    for (std::size_t i = diff; i < longer.size(); ++i) {
        RationalNumber first_value = thisIsLonger ? longer[i] : RationalNumber(0, 1);
        RationalNumber second_value = thisIsLonger ? shorter[i - diff] : longer[i - diff];
        RationalNumber res_value = thisIsLonger ? first_value.subtract(second_value) : second_value.subtract(first_value);
        resultCoeffs.push_back(res_value);
    }

    for (int i = 0; i < resultCoeffs.size(); ++i) { //и вообще эта ситуация возможно только при diff==0
        if (!resultCoeffs.at(i).getIntegerNumerator().abs().isNotEqualZero())
            resultCoeffs.pop_back(); //примнение pop_back корректно, тк мы имеем право удалять только ведущие нули, если коэффицент при наибольшей степени не 0, то даже не рассматриваем оставшиеся
        else
            break;
    }

    return Polynomial(resultCoeffs);
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
