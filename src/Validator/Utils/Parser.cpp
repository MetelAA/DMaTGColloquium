#include "Parser.h"
#include "../../Exceptions/UniversalStringException.h"

/**
 * @brief Парсит последовательность токенов в мономы.
 *
 * Алгоритм работает по принципу конечного автомата, обрабатывая токены слева направо.
 * Основные этапы обработки каждого монома:
 *
 * 1. ОПРЕДЕЛЕНИЕ ЗНАКА:
 *    - Обрабатывает начальные '+' или '-'
 *    - Знак по умолчанию: положительный (+1)
 *
 * 2. ЧТЕНИЕ КОЭФФИЦИЕНТА:
 *    - Вариант A: Число (TokenType::Num) → целый коэффициент
 *    - Вариант B: Дробь (Num '/' Num) → дробный коэффициент
 *    - Вариант C: Переменная 'x' → коэффициент = 1
 *
 * 3. ОБРАБОТКА ПЕРЕМЕННОЙ И СТЕПЕНИ:
 *    - Если после коэффициента следует 'x', обрабатывает степень
 *    - Степень может быть явной (x^Num) или неявной (x = x^1)
 *    - Если 'x' отсутствует → степень = 0
 *
 * 4. СОХРАНЕНИЕ МОНОМА:
 *    - Применяет знак к коэффициенту
 *    - Добавляет моном в результирующий вектор
 *
 * Примеры преобразования:
 * - "3x^2"    → Monom("3", "1", "2")
 * - "-1/2x"   → Monom("-1", "2", "1")
 * - "x"       → Monom("1", "1", "1")
 * - "5"       → Monom("5", "1", "0")
 * - "2*x^3"   → Monom("2", "1", "3")
 */
std::vector<Monom> Parser::parse(std::vector<Token> &tokens) {
    std::vector<Monom> monoms;

    if (tokens.empty())
        throw UniversalStringException("Parser.parse: empty token sequence");

    // Обрабатываем токены последовательно, пока не достигнем конца
    for (size_t i = 0; i < tokens.size(); ) {
        Monom monom;
        int sign = 1;  // Знак монома: 1 = положительный, -1 = отрицательный

        // === ЭТАП 1: ОПРЕДЕЛЕНИЕ ЗНАКА ===
        // Обрабатываем опциональный знак в начале монома
        if (tokens[i].type == TokenType::Add) {
            ++i;  // Пропускаем '+', знак остается положительным
        }
        else if (tokens[i].type == TokenType::Sub) {
            sign = -1;  // Устанавливаем отрицательный знак
            ++i;        // Пропускаем '-'
        }

        // Проверка: после знака должен быть хотя бы один токен
        if (i >= tokens.size())
            throw UniversalStringException("Parser: Unexpected end of input after sign");

        // === ЭТАП 2: ЧТЕНИЕ КОЭФФИЦИЕНТА ===
        if (tokens[i].type == TokenType::Num) {
            // СЛУЧАЙ 2A: Целочисленный коэффициент
            monom.numerator = tokens[i].value;  // Сохраняем числитель
            ++i;  // Переходим к следующему токену

            // Проверяем, является ли коэффициент дробью
            if (i < tokens.size() && tokens[i].type == TokenType::Div) {
                // СЛУЧАЙ 2B: Дробный коэффициент (числитель/знаменатель)
                ++i;  // Пропускаем '/'

                // После '/' ОБЯЗАТЕЛЬНО должно быть число (знаменатель)
                if (i >= tokens.size() || tokens[i].type != TokenType::Num)
                    throw UniversalStringException("Parser: Expected denominator after division sign");

                monom.denominator = tokens[i].value;  // Сохраняем знаменатель
                ++i;  // Переходим к следующему токену
            } else {
                // Целое число: знаменатель = 1
                monom.denominator = "1";
            }
        }
            // СЛУЧАЙ 2C: Коэффициент не указан (подразумевается 1), начинается с 'x'
        else if (tokens[i].type == TokenType::X) {
            monom.numerator = "1";
            monom.denominator = "1";
        } else {
            // Недопустимый токен на месте коэффициента
            throw UniversalStringException("Parser: Expected number or variable x as coefficient");
        }

        // === ЭТАП 3: ПРИМЕНЕНИЕ ЗНАКА К КОЭФФИЦИЕНТУ ===
        // Если знак отрицательный, добавляем минус к числителю
        if (sign == -1) {
            if (monom.numerator[0] == '-') {
                // Убираем двойной минус: -(-a) = a
                monom.numerator = monom.numerator.substr(1);
            } else {
                // Добавляем минус к положительному числителю
                monom.numerator = "-" + monom.numerator;
            }
        }

        // === ЭТАП 4: ПРОПУСК ЯВНОГО УМНОЖЕНИЯ ===
        // Обрабатываем случай "2*x" - пропускаем символ '*'
        if (i < tokens.size() && tokens[i].type == TokenType::Mul)
            ++i;

        // === ЭТАП 5: ОБРАБОТКА ПЕРЕМЕННОЙ И СТЕПЕНИ ===
        if (i < tokens.size() && tokens[i].type == TokenType::X) {
            // Обнаружена переменная x - обрабатываем степень
            ++i;  // Пропускаем 'x'

            // Проверяем, указана ли степень явно (символ '^')
            if (i < tokens.size() && tokens[i].type == TokenType::Pow) {
                // СЛУЧАЙ 5A: Явная степень (x^num)
                ++i;  // Пропускаем '^'

                // После '^' ОБЯЗАТЕЛЬНО должно быть число (показатель степени)
                if (i >= tokens.size() || tokens[i].type != TokenType::Num)
                    throw UniversalStringException("Parser: Expected exponent number after power sign");

                monom.degree = tokens[i].value;  // Сохраняем степень
                ++i;  // Переходим к следующему токену
            } else {
                // СЛУЧАЙ 5B: Неявная степень (x = x^1)
                monom.degree = "1";
            }
        } else {
            // СЛУЧАЙ 5C: Переменная отсутствует (константа = x^0)
            monom.degree = "0";
        }

        // === ЭТАП 6: СОХРАНЕНИЕ РЕЗУЛЬТАТА ===
        monoms.push_back(monom);
    }

    return monoms;
}