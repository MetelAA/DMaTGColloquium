//
// Панфилов Григорий Иванович 4385 создал эту программу!
//
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QDateTime>
#include <QDir>

#include "src/Exceptions/UniversalStringException.h"
#include "src/Validator/Validator.h"
#include "src/Polynomial.h"
#include "src/IntegerNumber.h"
#include "src/RationalNumber.h"
#include "src/NaturalNumber.h"

// Система тем
namespace Theme{
    QString lightTheme = R"(
        QMainWindow, QWidget{
            background-color: rgba(240, 240, 240, 0.9);
            color: #333333;
        }
        QPushButton{
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover{
            background-color: #e6e6e6;
            border-color: #adadad;
        }
        QPushButton:pressed{
            background-color: #d4d4d4;
        }
        QLineEdit, QTextEdit{
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 6px;
            selection-background-color: #0078d4;
        }
        QLabel{
            background-color: transparent;
            color: #333333;
        }
        QTabWidget::pane{
            border: 1px solid #cccccc;
            background-color: #ffffff;
        }
        QTabBar::tab{
            background-color: #e6e6e6;
            color: #333333;
            padding: 8px 16px;
            border: 1px solid #cccccc;
            border-bottom: none;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected{
            background-color: #ffffff;
            border-color: #cccccc;
        }
        QTabBar::tab:hover:!selected{
            background-color: #f0f0f0;
        }
        QMessageBox{
			background-color: #ffffff;
			color: #333333;
		}
		QMessageBox QLabel{
			color: #333333;
		}
    )";
    QString darkTheme = R"(
        QMainWindow, QWidget{
            background-color: rgba(30, 30, 30, 0.9);
            color: #ffffff;
        }
        QPushButton{
            background-color: #2d2d30;
            color: #ffffff;
            border: 1px solid #3e3e42;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover{
            background-color: #38383c;
            border-color: #007acc;
        }
        QPushButton:pressed{
            background-color: #004578;
        }
        QLineEdit, QTextEdit{
            background-color: #252526;
            color: #ffffff;
            border: 1px solid #3e3e42;
            border-radius: 4px;
            padding: 6px;
            selection-background-color: #0078d4;
        }
        QLabel{
            background-color: transparent;
            color: #ffffff;
        }
        QTabWidget::pane{
            border: 1px solid #3e3e42;
            background-color: #252526;
        }
        QTabBar::tab{
            background-color: #2d2d30;
            color: #ffffff;
            padding: 8px 16px;
            border: 1px solid #3e3e42;
            border-bottom: none;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected{
            background-color: #252526;
            border-color: #3e3e42;
        }
        QTabBar::tab:hover:!selected{
            background-color: #38383c;
        }
        QMessageBox{
			background-color: #252526;
			color: #ffffff;
		}
		QMessageBox QLabel{
			color: #ffffff;
		}
    )";
}

// Структура для хранения информации о модуле
struct Module{
	QString name;
	QString category;
	int inputCount;
	QString description;
};

// Функция для вывода ошибок на экран
void showError(const std::string& errorMessage){
    QMessageBox::critical(nullptr, "Ошибка", 
                         QString::fromStdString(errorMessage), 
                         QMessageBox::Ok);
}

// Функция для показа справки
void showHelp(bool isDarkTheme){
    QString helpText = R"(
<h3>Общая информация</h3>
<p>Программа предоставляет различные математические модули для работы с натуральными, целыми числами, дробями и многочленами.</p>

<h3>Как пользоваться:</h3>
<ol>
<li>Выберите категорию модуля во вкладках слева</li>
<li>Нажмите на кнопку с названием нужного модуля</li>
<li>Введите данные в поля ввода согласно описанию модуля</li>
<li>Нажмите кнопку "Вычислить" для получения результата</li>
<li>Используйте черновик для сохранения промежуточных результатов</li>
</ol>

<h3>Особенности ввода:</h3>
<ul>
<li><b>Дроби:</b> вводите в формате "a/b" (например: "¾", "-14/3")</li>
<li><b>Многочлены:</b> вводите в стандартной форме (например: "x^3+2/5x^2+3x+4")</li>
</ul>

<h3>Черновик:</h3>
<ul>
<li>Текст в черновике сохраняется при переключении между модулями</li>
<li>Используйте кнопку "Копировать результат в черновик" для быстрого сохранения</li>
<li>Черновик можно очистить соответствующей кнопкой</li>
</ul>

<h3>Темы оформления:</h3>
<p>Переключайтесь между светлой и тёмной темой для комфортной работы</p>
    )";
    QMessageBox helpBox;
    helpBox.setWindowTitle("Справка");
    helpBox.setText(helpText);
    helpBox.setTextFormat(Qt::RichText);
    helpBox.setIcon(QMessageBox::Information);
    if(isDarkTheme){
        helpBox.setStyleSheet(Theme::darkTheme);
    }else{
        helpBox.setStyleSheet(Theme::lightTheme);
    }
    helpBox.exec();
}


// Сравнение натуральных чисел
std::string COM_NN_D(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second =  new NaturalNumber(b);

        auto res = first.cmp(second);

        return std::string(1, res + '0');
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Сравнение натуральных чисел с нулем.
std::string NZER_N_B(std::string& a){
    try{
        Validator::validateNaturalNumber(a);

        auto first = NaturalNumber(a);

        auto res = first.isNotEqualZero();
        return std::string(1, res + '0');
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Добавление к натуральному числу 1.
std::string ADD_1N_N(std::string& a){
    try{
        Validator::validateNaturalNumber(a);

        auto first = NaturalNumber(a);

        first.increment();
        return first.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}


// Сложение натуральных чисел
std::string ADD_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.add(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычитание натуральных чисел
std::string SUB_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.subtract(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение на цифру
std::string MUL_ND_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);

        // Проверяем, что b - это цифра от 0 до 9
        if (b.length() != 1) {
            throw UniversalStringException("Second argument must be a single digit (0-9)");
        }

        uint8_t digit_char = b[0] - '0';

        auto res = first.multiplyByDigit(digit_char);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}


// Умножение на 10 в ст K
std::string MUL_Nk_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);

        // Проверяем, что число влезает в size_t
        if (b.length() > 20) { // SIZE_MAX обычно 18-20 цифр
            throw UniversalStringException("Exponent k is too large");
        }

        size_t k = std::stoull(b); // преобразуем string в size_t

        auto res = first.multiplyByPowerOfTen(k);

        return res.toString();

    }catch(const std::out_of_range& e){
        throw UniversalStringException("Exponent k is too large for size_t");
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение натуральных чисел.
std::string MUL_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.multiply(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычитание из натурального другого натурального, умноженного на цифру для случая с неотрицательным результатом
std::string SUB_NDN_N(std::string& a, std::string& b, std::string& c){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);
        Validator::validateNaturalNumber(c);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        // Проверяем, что b - это цифра от 0 до 9
        if (c.length() != 1) {
            throw UniversalStringException("Second argument must be a single digit (0-9)");
        }

        uint8_t digit_char = c[0] - '0';

        auto res = first.subtractMultiplied(second, digit_char);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычисление первой цифры деления большего натурального на меньшее, домноженное на 10^k,где k - номер позиции этой цифры (номер считается с нуля)
std::string DIV_NN_Dk(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.getFirstDivisionDigit(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Неполное частное от деления первого натурального числа на второе с остатком (делитель отличен от нуля)
std::string DIV_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.quotient(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Остаток от деления первого натурального числа на второе натуральное (делитель отличен от нуля)
std::string MOD_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.remainder(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// НОД натуральных чисел
std::string GCF_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.GCD(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// НОК натуральных чисел
std::string LCM_NN_N(std::string& a, std::string& b){
    try{
        Validator::validateNaturalNumber(a);
        Validator::validateNaturalNumber(b);

        auto first = NaturalNumber(a);
        auto second = NaturalNumber(b);

        auto res = first.LCM(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Абсолютная величина числа, результат - натуральное
std::string ABS_Z_N(std::string& a){
    try{
        Validator::validateIntegerNumber(a);

        auto first = IntegerNumber(a);

        auto res = first.abs();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Определение положительности числа (2 - положительное, 0 — равное нулю, 1 - отрицательное)
std::string POZ_Z_D(std::string& a){
    try{
        Validator::validateIntegerNumber(a);

        auto first = IntegerNumber(a);

        auto res = first.getSign();
        return std::string(1, res + '0');
    }catch(const UniversalStringException& e){
        throw;
    }
}


// Умножение целого на (-1)
std::string MUL_ZM_Z(std::string& a){
    try{
        Validator::validateIntegerNumber(a);

        auto first = IntegerNumber(a);

        auto res = first.negate();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Преобразование натурального в целое
std::string TRANS_N_Z(std::string& a){
    try{
        Validator::validateNaturalNumber(a);

        auto first = NaturalNumber(a);
        auto integer = IntegerNumber(a);

        auto res = integer.toInteger(first);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Преобразование целого неотрицательного в натуральное
std::string TRANS_Z_N(std::string& a){
    try{
        Validator::validateIntegerNumber(a);

        auto first = IntegerNumber(a);

        auto res = first.toNatural(first);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Сложение целых чисел
std::string ADD_ZZ_Z(std::string& a, std::string& b){
    try{
        Validator::validateIntegerNumber(a);
        Validator::validateIntegerNumber(b);

        auto first = IntegerNumber(a);
        auto second = IntegerNumber(b);

        auto res = first.add(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычитание целых чисел
std::string SUB_ZZ_Z(std::string& a, std::string& b){
    try{
        Validator::validateIntegerNumber(a);
        Validator::validateIntegerNumber(b);

        auto first = IntegerNumber(a);
        auto second = IntegerNumber(b);

        auto res = first.subtract(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение целых чисел
std::string MUL_ZZ_Z(std::string& a, std::string& b){
    try{
        Validator::validateIntegerNumber(a);
        Validator::validateIntegerNumber(b);

        auto first = IntegerNumber(a);
        auto second = IntegerNumber(b);

        auto res = first.multiply(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Частное от деления целого на целое (делитель отличен от нуля)
std::string DIV_ZZ_Z(std::string& a, std::string& b){
    try{
        Validator::validateIntegerNumber(a);
        Validator::validateIntegerNumber(b);

        auto first = IntegerNumber(a);
        auto second = IntegerNumber(b);

        auto res = first.quotient(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Остаток от деления целого на целое(делитель отличен от нуля)
std::string MOD_ZZ_Z(std::string& a, std::string& b){
    try{
        Validator::validateIntegerNumber(a);
        Validator::validateIntegerNumber(b);

        auto first = IntegerNumber(a);
        auto second = IntegerNumber(b);

        auto res = first.remainder(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}


// Сокращение дроби
std::string RED_Q_Q(std::string& a){
    try{
        Validator::validateRationalNumber(a);

        auto first = RationalNumber(a);

        first.reduce();
        return first.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Проверка сокращенного дробного на целое, если рациональное число является целым, то «да», иначе «нет»
std::string INT_Q_B(std::string& a){
    try{
        Validator::validateRationalNumber(a);

        auto first = RationalNumber(a);

        bool res = first.isInteger();
        return res ? "true" : "false";
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Преобразование целого в дробное
std::string TRANS_Z_Q(std::string& a){
    try{
        Validator::validateIntegerNumber(a);

        auto first = IntegerNumber(a);
        auto tmp = RationalNumber("1/1");

        auto res = tmp.fromInteger(first);

        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Преобразование сокращенного дробного в целое (если знаменатель равен 1)
std::string TRANS_Q_Z(std::string& a){
    try{
        Validator::validateRationalNumber(a);

        auto first = RationalNumber(a);

        auto res = first.toInteger(first);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Сложение дробей
std::string ADD_QQ_Q(std::string& a, std::string& b){
    try{
        Validator::validateRationalNumber(a);
        Validator::validateRationalNumber(b);

        auto first = RationalNumber(a);
        auto second = RationalNumber(b);

        auto res = first.add(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычитание дробей
std::string SUB_QQ_Q(std::string& a, std::string& b){
    try{
        Validator::validateRationalNumber(a);
        Validator::validateRationalNumber(b);

        auto first = RationalNumber(a);
        auto second = RationalNumber(b);

        auto res = first.subtract(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение дробей
std::string MUL_QQ_Q(std::string& a, std::string& b){
    try{
        Validator::validateRationalNumber(a);
        Validator::validateRationalNumber(b);

        auto first = RationalNumber(a);
        auto second = RationalNumber(b);

        auto res = first.multiply(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Деление дробей (делитель отличен от нуля)
std::string DIV_QQ_Q(std::string& a, std::string& b){
    try{
        Validator::validateRationalNumber(a);
        Validator::validateRationalNumber(b);

        auto first = RationalNumber(a);
        auto second = RationalNumber(b);

        auto res = first.division(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}


// Сложение многочленов
std::string ADD_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.add(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вычитание многочленов
std::string SUB_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.subtract(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение многочлена на рациональное число
std::string MUL_PQ_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        Validator::validateRationalNumber(b);

        auto first = Polynomial(p1);
        auto second = RationalNumber(b);

        auto res = first.multiplyByRational(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение многочлена на x^k, k-натуральное или 0
std::string MUL_Pxk_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        Validator::validateNaturalNumber(b);

        auto first = Polynomial(p1);

        // Проверяем, что число влезает в size_t
        if (b.length() > 20) { // SIZE_MAX обычно 18-20 цифр
            throw UniversalStringException("Exponent k is too large");
        }

        size_t k = std::stoul(b); // преобразуем string в size_t

        auto res = first.multiplyByXInKPower(k);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Старший коэффициент многочлена
std::string LED_P_Q(std::string& a){
    try{
        auto p1 = Validator::validatePolynomial(a);

        auto first = Polynomial(p1);

        auto res = first.getLeadingCoefficient();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Степень многочлена
std::string DEG_P_N(std::string& a){
    try{
        auto p1 = Validator::validatePolynomial(a);

        auto first = Polynomial(p1);

        auto res = first.getDegree();
        return std::to_string(res);
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Вынесение из многочлена НОК знаменателей коэффициентов и НОД числителей
std::string FAC_P_Q(std::string& a){
    try{
        auto p1 = Validator::validatePolynomial(a);

        auto first = Polynomial(p1);

        auto res = first.factorOut();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Умножение многочленов
std::string MUL_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.multiply(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Частное от деления многочлена на многочлен при делении с остатком
std::string DIV_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.quotient(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Остаток от деления многочлена на многочлен при делении с остатком
std::string MOD_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.remainder(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// НОД многочленов
std::string GCF_PP_P(std::string& a, std::string& b){
    try{
        auto p1 = Validator::validatePolynomial(a);
        auto p2 = Validator::validatePolynomial(b);

        auto first = Polynomial(p1);
        auto second = Polynomial(p2);

        auto res = first.GCD(second);
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Производная многочлена
std::string DER_P_P(std::string& a){
    try{
        auto p1 = Validator::validatePolynomial(a);

        auto first = Polynomial(p1);

        auto res = first.derivative();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}

// Преобразование многочлена — кратные корни в простые
std::string NMR_P_P(std::string& a){
    try{
        auto p1 = Validator::validatePolynomial(a);

        auto first = Polynomial(p1);

        auto res = first.makeSquareFree();
        return res.toString();
    }catch(const UniversalStringException& e){
        throw;
    }
}



// === Функция генерации интерфейса выбранного модуля(правая часть калькулятора) ===
void showModuleInterface(QVBoxLayout* moduleLayout, QLabel* titleLabel, QTextEdit* draftField, QPushButton* copyResultButton, const Module& mod){
    // Удаление интерфейса предыдущего модуля
    while(moduleLayout->count() > 0){
		QLayoutItem* child = moduleLayout->takeAt(0);
		if(child->widget()){
			child->widget()->deleteLater();
		}
		delete child;
	}
    // Устанавливаем новый заголовок
    titleLabel->setText(mod.name);
    // Описание модуля
    if(!mod.description.isEmpty()){
        QLabel* descriptionLabel = new QLabel(mod.description);
        descriptionLabel->setWordWrap(true);
        descriptionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        // Стилизация описания
        QFont descFont = descriptionLabel->font();
        descFont.setItalic(true);
        descriptionLabel->setFont(descFont);
        descriptionLabel->setStyleSheet("color: #666666; background-color: rgba(255,255,255,0.7); padding: 8px; border-radius: 4px;");
        moduleLayout->addWidget(descriptionLabel);
        moduleLayout->addSpacing(10);
    }
    // Создаем поля ввода в зависимости от того, сколько нужно чисел
    QVector<QLineEdit*> inputFields;
    for(int i = 0; i < mod.inputCount; ++i){
        QLabel* inputLabel = new QLabel(QString("Число %1:").arg(i + 1));
        moduleLayout->addWidget(inputLabel);
        QLineEdit* inputField = new QLineEdit();
        moduleLayout->addWidget(inputField);
        inputFields.append(inputField);
    }
    // Поле для результата
    QLabel* resultLabel = new QLabel("Результат:");
    moduleLayout->addWidget(resultLabel);
    QTextEdit* resultField = new QTextEdit();
    resultField->setReadOnly(true);
    resultField->setMaximumHeight(100);
    resultField->setLineWrapMode(QTextEdit::WidgetWidth);
    resultField->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    moduleLayout->addWidget(resultField);
    // Кнопка для вычисления
    QPushButton* calculateButton = new QPushButton("Вычислить");
    moduleLayout->addWidget(calculateButton);
    // === Переподключаем кнопку "копировать результат" для текущего модуля ===
    copyResultButton->disconnect();
    // Подключаем заново для текущего модуля
    QObject::connect(copyResultButton, &QPushButton::clicked, [=](){
        QString result = resultField->toPlainText();
        if (!result.isEmpty()) {
            QString currentDraft = draftField->toPlainText();
            if (!currentDraft.isEmpty()) {
                currentDraft += "\n\n"; // Добавляем отступ между записями
            }
            currentDraft += "[" + mod.name + "]: " + result;
            draftField->setPlainText(currentDraft);
            // Прокручиваем к новому содержимому
            QTextCursor cursor = draftField->textCursor();
            cursor.movePosition(QTextCursor::End);
            draftField->setTextCursor(cursor);
        }
    });
    
    QObject::connect(calculateButton, &QPushButton::clicked, [=](){
		try{
			// Собираем строки из полей ввода
			QVector<std::string> inputString;
			for(QLineEdit* field : inputFields){
				inputString.append(field->text().toStdString());
			}
			// >>> Выполнение модулей <<< (название ОБЯЗАТЕЛЬНО должно быть уникальным)
			std::string result;
            if (mod.name == "Сравнение натуральных"){
                result = COM_NN_D(inputString[0], inputString[1]);
            }else if (mod.name == "Проверка на 0"){
                result = NZER_N_B(inputString[0]);
            } else if (mod.name == "Добавление 1"){
                result = ADD_1N_N(inputString[0]);
            }else if (mod.name == "Сложение натуральных"){
                result = ADD_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "Вычитание меньшего"){
                result = SUB_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение на цифру"){
                result = MUL_ND_N(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение на 10^k"){
                result = MUL_Nk_N(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение натуральных"){
                result = MUL_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "Вычитание умноженного"){
                result = SUB_NDN_N(inputString[0], inputString[1], inputString[2]);
            }else if (mod.name == "Первая цифра деления"){
                result = DIV_NN_Dk(inputString[0], inputString[1]);
            }else if (mod.name == "Неполное частное"){
                result = DIV_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "Остаток от деления"){
                result = MOD_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "НОД"){
                result = GCF_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "НОК"){
                result = LCM_NN_N(inputString[0], inputString[1]);
            }else if (mod.name == "Абсолютная величина"){
                result = ABS_Z_N(inputString[0]);
            }else if (mod.name == "Положительность"){
                result = POZ_Z_D(inputString[0]);
            }else if (mod.name == "Умножение на -1"){
                result = MUL_ZM_Z(inputString[0]);
            }else if (mod.name == "Натуральное в целое"){
                result = TRANS_N_Z(inputString[0]);
            }else if (mod.name == "Целое в натуральное"){
                result = TRANS_Z_N(inputString[0]);
            }else if (mod.name == "Сложение целых чисел"){
                result = ADD_ZZ_Z(inputString[0], inputString[1]);
            }else if (mod.name == "Вычитание целых чисел"){
                result = SUB_ZZ_Z(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение целых чисел"){
                result = MUL_ZZ_Z(inputString[0], inputString[1]);
            }else if (mod.name == "Частное деления целых"){
                result = DIV_ZZ_Z(inputString[0], inputString[1]);
            }else if (mod.name == "Остаток деления целых"){
                result = MOD_ZZ_Z(inputString[0], inputString[1]);
            }else if (mod.name == "Сокращение дроби"){
                result = RED_Q_Q(inputString[0]);
            }else if (mod.name == "Проверка на целое"){
                result = INT_Q_B(inputString[0]);
            }else if (mod.name == "Целое в дробное"){
                result = TRANS_Z_Q(inputString[0]);
            }else if (mod.name == "Дробное в целое"){
                result = TRANS_Q_Z(inputString[0]);
            }else if (mod.name == "Сложение дробей"){
                result = ADD_QQ_Q(inputString[0], inputString[1]);
            }else if (mod.name == "Вычитание дробей"){
                result = SUB_QQ_Q(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение дробей"){
                result = MUL_QQ_Q(inputString[0], inputString[1]);
            }else if (mod.name == "Деление дробей"){
                result = DIV_QQ_Q(inputString[0], inputString[1]);
            }else if (mod.name == "Сложение многочленов"){
                result = ADD_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "Вычитание многочленов"){
                result = SUB_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение на дробь"){
                result = MUL_PQ_P(inputString[0], inputString[1]);
            }else if (mod.name == "Умножение на x^k"){
                result = MUL_Pxk_P(inputString[0], inputString[1]);
            }else if (mod.name == "Старший коэффициент"){
                result = LED_P_Q(inputString[0]);
            }else if (mod.name == "Степень многочлена"){
                result = DEG_P_N(inputString[0]);
            }else if (mod.name == "НОК и НОД"){
                result = FAC_P_Q(inputString[0]);
            }else if (mod.name == "Умножение многочленов"){
                result = MUL_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "Частное деления"){
                result = DIV_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "Остаток деления"){
                result = MOD_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "НОД многочленов"){
                result = GCF_PP_P(inputString[0], inputString[1]);
            }else if (mod.name == "Производная"){
                result = DER_P_P(inputString[0]);
            }else if (mod.name == "Кратные в простые"){
                result = NMR_P_P(inputString[0]);
            }

            const size_t MAX_DISPLAY = 2000;      // сколько показывать в QTextEdit
            const size_t MAX_LEN = 100000;        // лимит пользователя

            if (result.size() > MAX_LEN) {
                // Генерация имени файла
                QString fileName = "result_" +
                                   QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";

                QString filePath = QDir::currentPath() + "/" + fileName;

                // Сохранение полного результата в файл
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << QString::fromStdString(result);
                    file.close();
                }

                // Формируем сокращённую версию для UI
                std::string shortRes = result.substr(0, MAX_DISPLAY);
                shortRes += "\n...\n(Полная версия сохранена в файл: "
                            + filePath.toStdString() + ")";

                resultField->setPlainText(QString::fromStdString(shortRes));
            } else {
                // Если строка короткая — просто выводим
                resultField->setPlainText(QString::fromStdString(result));
            }

        }catch(const UniversalStringException& e){
            // Обработка всех ошибок через showError
            showError(e.what());
            // Очищаем поле результата при ошибке
            resultField->clear();
        }
    });
    // Добавляем растягивающееся пространство
    moduleLayout->addStretch();
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Главное окно
    QWidget window;
    window.setWindowTitle("Калькулятор");
    window.resize(800, 600);
	// Кнопка переключения тем
    QPushButton *themeToggleButton = new QPushButton("Светлая тема");
    themeToggleButton->setFixedSize(135, 35);
    // Кнопка справки
    QPushButton *helpButton = new QPushButton("Справка");
    helpButton->setFixedSize(100, 35);
	// Проверяем наличие фонового изображения
    QString backgroundStyle = "";
    if (QFile::exists("background.jpg")) {
        backgroundStyle = R"(
            QWidget#mainWindow {
                background-image: url(background.jpg);
                background-position: center;
            }
        )";
    }
    // Устанавливаем тёмную тему по умолчанию вместе с фоном
    window.setObjectName("mainWindow");
    window.setStyleSheet(backgroundStyle + Theme::darkTheme);
    // Подключаем переключение темы
    bool isDarkTheme = true;
    QObject::connect(themeToggleButton, &QPushButton::clicked, [&]() {
		if (isDarkTheme) {
			window.setStyleSheet(backgroundStyle + Theme::lightTheme);
			themeToggleButton->setText("Тёмная тема");
		} else {
			window.setStyleSheet(backgroundStyle + Theme::darkTheme);
			themeToggleButton->setText("Светлая тема");
		}
		isDarkTheme = !isDarkTheme;
	});
	// Подключаем кнопку справки
	QObject::connect(helpButton, &QPushButton::clicked, [&](){
		showHelp(isDarkTheme);
	});
    // >>> Список всех модулей <<<
    QVector<Module> allModules = {
        {"Сравнение натуральных", "Натуральные и 0", 2, "Сравнение натуральных чисел: 2 - если первое больше второго, 0, если равно, 1 иначе."},
        {"Проверка на 0", "Натуральные и 0", 1, "Проверка на ноль: если число не равно нулю, то «да» иначе «нет»."},
        {"Добавление 1", "Натуральные и 0", 1, "Добавление 1 к натуральному числу."},
        {"Сложение натуральных", "Натуральные и 0", 2, "Сложение натуральных чисел."},
        {"Вычитание меньшего", "Натуральные и 0", 2, "Вычитание из первого большего натурального числа второго меньшего или равного."},
        {"Умножение на цифру", "Натуральные и 0", 2, "Умножение натурального числа на цифру."},
        {"Умножение на 10^k", "Натуральные и 0", 2, "Умножение натурального числа на 10^k, *k-натуральное (не длинное)*."},
        {"Умножение натуральных", "Натуральные и 0", 2, "Умножение натуральных чисел."},
        {"Вычитание умноженного", "Натуральные и 0", 3, "Вычитание из натурального другого натурального, умноженного на цифру."},
        {"Первая цифра деления", "Натуральные и 0", 3, "Вычисление первой цифры деления большего натурального на меньшее, домноженное на 10^k, где k - номер позиции этой цифры (номер считается с нуля)."},
        {"Неполное частное", "Натуральные и 0", 2, "Неполное частное от деления первого натурального числа на второе с остатком (делитель отличен от нуля)."},
        {"Остаток от деления", "Натуральные и 0", 2, "Остаток от деления первого натурального числа на второе натуральное (делитель отличен от нуля)."},
        {"НОД", "Натуральные и 0", 2, "НОД натуральных чисел."},
        {"НОК", "Натуральные и 0", 2, "НОК натуральных чисел."},
        {"Абсолютная величина", "Целые", 1, "Абсолютная величина числа, результат - натуральное."},
        {"Положительность", "Целые", 1, "Определение положительности числа(2 - положительное, 0 - равное нулю, 1 - отрицательное)."},
        {"Умножение на -1", "Целые", 1, "Умножение целого на (-1)."},
        {"Натуральное в целое", "Целые", 1, "Преобразование натурального в целое."},
        {"Целое в натуральное", "Целые", 1, "Преобразование целого неотрицательного в натуральное."},
        {"Сложение целых чисел", "Целые", 2, "Сложение целых чисел."},
        {"Вычитание целых чисел", "Целые", 2, "Вычитание целых чисел"},
        {"Умножение целых чисел", "Целые", 2, "Умножение целых чисел"},
        {"Частное деления целых", "Целые", 2, "Частное от деления целого на целое (делитель отличен от нуля)."},
        {"Остаток деления целых", "Целые", 2, "Остаток от деления целого на целое(делитель отличен от нуля)."},
        {"Сокращение дроби", "Дроби", 1, "Пример ввода: “-14/3”.\nСокращение дроби."},
        {"Проверка на целое", "Дроби", 1, "Пример ввода: “-14/3”.\nПроверка *сокращенного дробного* на целое, если рациональное число является целым, то «да», иначе «нет»."},
        {"Целое в дробное", "Дроби", 1, "Пример ввода: “-14/3”.\nПреобразование целого в дробное."},
        {"Дробное в целое", "Дроби", 1, "Пример ввода: “-14/3”.\nПреобразование *сокращенного* дробного в целое (если знаменатель равен 1)."},
        {"Сложение дробей", "Дроби", 2, "Пример ввода: “-14/3”.\nСложение дробей."},
        {"Вычитание дробей", "Дроби", 2, "Пример ввода: “-14/3”.\nВычитание дробей."},
        {"Умножение дробей", "Дроби", 2, "Пример ввода: “-14/3”.\nУмножение дробей."},
        {"Деление дробей", "Дроби", 2, "Пример ввода: “-14/3”.\nДеление дробей (делитель отличен от нуля)."},
        {"Сложение многочленов", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nСложение многочленов."},
        {"Вычитание многочленов", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nВычитание многочленов."},
        {"Умножение на дробь", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nУмножение многочлена на рациональное число."},
        {"Умножение на x^k", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nУмножение многочлена на x^k, k-натуральное или 0."},
        {"Старший коэффициент", "Многочлены", 1, "Пример ввода: “x^3+2/5x^2+3x+4”.\nСтарший коэффициент многочлена."},
        {"Степень многочлена", "Многочлены", 1, "Пример ввода: “x^3+2/5x^2+3x+4”.\nСтепень многочлена."},
        {"НОК и НОД", "Многочлены", 1, "Пример ввода: “x^3+2/5x^2+3x+4”.\nВынесение из многочлена НОК знаменателей коэффициентов и НОД числителей."},
        {"Умножение многочленов", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nУмножение многочленов."},
        {"Частное деления", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nЧастное от деления многочлена на многочлен при делении с остатком."},
        {"Остаток деления", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nОстаток от деления многочлена на многочлен при делении с остатком."},
        {"НОД многочленов", "Многочлены", 2, "Пример ввода: “x^3+2/5x^2+3x+4”.\nНОД многочленов."},
        {"Производная", "Многочлены", 1, "Пример ввода: “x^3+2/5x^2+3x+4”.\nПроизводная многочлена."},
        {"Кратные в простые", "Многочлены", 1, "Пример ввода: “x^3+2/5x^2+3x+4”.\nПреобразование многочлена — кратные корни в простые."}
    };
    // === Создание интерфейса ===
    // Основа
    QHBoxLayout* mainLayout = new QHBoxLayout(&window);
    // Левая часть - выбор модуля
    QWidget* leftPanel = new QWidget();
    leftPanel->setFixedWidth(260);
    QVBoxLayout* leftPanelLayout = new QVBoxLayout(leftPanel);
    // Кнопки темы и справки
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(themeToggleButton);
    controlLayout->addWidget(helpButton);
    leftPanelLayout->addLayout(controlLayout);
    leftPanelLayout->addSpacing(10);
    // Создаем виджет вкладок
    QTabWidget* leftTabs = new QTabWidget();
    leftTabs->setStyleSheet(R"(
		QTabBar::scroller{
			width: 80px;
		}
	)");
    leftPanelLayout->addWidget(leftTabs);
    // Правая часть - ввод и вывод
    QWidget* rightArea = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    // Заголовок справа
    QLabel* currentModuleLabel = new QLabel("Выберите модуль");
    currentModuleLabel->setAlignment(Qt::AlignCenter);
    QFont font = currentModuleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    currentModuleLabel->setFont(font);
    rightLayout->addWidget(currentModuleLabel);
    // Область ввода
    QWidget* moduleArea = new QWidget();
    QVBoxLayout* moduleLayout = new QVBoxLayout(moduleArea);
    moduleLayout->setAlignment(Qt::AlignTop);
    rightLayout->addWidget(moduleArea);
    // Создание черновика
    rightLayout->addSpacing(10);
    QLabel* draftLabel = new QLabel("Черновик:");
    rightLayout->addWidget(draftLabel);
    QTextEdit* draftField = new QTextEdit();
    draftField->setPlaceholderText("Введите текст и он сохранится между модулями...");
    draftField->setLineWrapMode(QTextEdit::WidgetWidth);
    draftField->setAcceptRichText(false);
    draftField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    draftField->setMinimumHeight(150);
    rightLayout->addWidget(draftField, 1);
    // Панель инструментов для черновика
    QHBoxLayout* draftToolsLayout = new QHBoxLayout();
    QPushButton* clearDraftButton = new QPushButton("Очистить черновик");
    QPushButton* copyResultButton = new QPushButton("Копировать результат в черновик");
    draftToolsLayout->addWidget(clearDraftButton);
    draftToolsLayout->addWidget(copyResultButton);
    draftToolsLayout->addStretch();
    rightLayout->addLayout(draftToolsLayout);
    // Обработчик для черновика
    QObject::connect(clearDraftButton, &QPushButton::clicked, [=](){
        draftField->clear();
    });
    // === Автоматическая генерация вкладок === 
    QVector<QString> categories;
    for(const Module& mod : allModules){
        if(!categories.contains(mod.category)){
            categories.append(mod.category);
        }
    }
    // Для каждой категории создаем вкладку
    for(const QString& category : categories){
        QWidget* tab = new QWidget();
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);        
        // Заголовок вкладки
        QLabel* categoryLabel = new QLabel(category);
        categoryLabel->setAlignment(Qt::AlignCenter);
        QFont categoryFont = categoryLabel->font();
        categoryFont.setBold(true);
        categoryLabel->setFont(categoryFont);
        tabLayout->addWidget(categoryLabel);
        // Добавляем кнопки для всех модулей этой категории
        for(const Module& mod : allModules){
            if(mod.category == category){
                QPushButton* button = new QPushButton(mod.name);
                tabLayout->addWidget(button);
                // Подключение кнопки - передаем черновик и кнопку копирования
                QObject::connect(button, &QPushButton::clicked, [=](){
                    showModuleInterface(moduleLayout, currentModuleLabel, draftField, copyResultButton, mod);
                });
            }
        }
        tabLayout->addStretch();
        leftTabs->addTab(tab, category);
    }
    // Сборка левой и правой частей
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightArea);
    // Запуск окна
    window.show();
    return app.exec();
}
