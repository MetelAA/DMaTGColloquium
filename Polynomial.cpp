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
        throw UniversalStringException("a typical behavior, the vector of coefficients should not be empty");
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

Polynomial Polynomial::factorOut() const
{
    NaturalNumber nod = coefficients.at(0).getIntegerNumerator().abs(); //предположили, что первые числа будут НОД и НОК
    NaturalNumber nok = coefficients.at(0).getNaturalDenominator();
    
    for(size_t i = 1; i < coefficients.size(); i++){
        nod = nod.GCD(coefficients.at(i).getIntegerNumerator().abs()); //находим НОД из последовательного алгоритма Евклида для всех коэффициентов
        nok = nok.LCM(coefficients.at(i).getNaturalDenominator()); //пользуемся формулой связи НОД и НОК последовательно для всех коэффициентов
    }
    return this->multiplyByRational(RationalNumber(nok.toString(), nod.toString())); //получившийся полином = НОК/НОД * исходный полином
}

Polynomial Polynomial::multiply(const Polynomial &other) const
{
    std::vector<RationalNumber> multiplicandCoefficentVector = coefficients; //векторы коэффициентов
    std::vector<RationalNumber> multiplierCoefficentVector = other.coefficients;
    RationalNumber zero (std::string("0/1"));
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

Polynomial Polynomial::quotient(const Polynomial &other) const
{
    std::vector<RationalNumber> dividendCoefficientVector = coefficients; //вектора коэффициентов - делимого
    std::vector<RationalNumber> divisorCoefficientVector = other.coefficients; //делителя

    RationalNumber zero = RationalNumber(std::string("0/1"));
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
                currentPolynomial.coefficients.push_back(RationalNumber("0/1")); //заполняем, пока незначащими нулями
            }
            for(auto x : other.multiplyByRational(currentCoefficient).getCoefficients()){ //добавляем в старшие степени коэффициенты делителя, умноженного на currentCoefficient
                currentPolynomial.coefficients.push_back(x); 
            } //получим currentPolynomial по определению
            this->subtract(currentPolynomial); //вычитаем из делимого
            tmpRes.push_back(currentCoefficient); //записываем коэффициент во временный результат - помним определение полного результата
            currentPolynomial.coefficients = {}; //обнуляем вектор currentPolynomial
        }else{
            tmpRes.push_back(RationalNumber("0/1")); //коэффициент 0, если его нет в частном - можно еще раз посмотреть строку с условием
        }
    }
    if(tmpRes.size()){ //если частное не пусто
        for(std::vector<RationalNumber>::iterator it = tmpRes.end(); it >= tmpRes.end(); it--){ //идем в обратном порядке по временному ответу
            res.coefficients.push_back(*it); //записываем в ответ - нужно, чтобы частное x^2 + 2x + 3 было записано в векторе, как {3, 2, 1}
        }
    }
    return res;
}