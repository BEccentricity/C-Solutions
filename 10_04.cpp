#include <cassert>
#include <cstddef>
#include <boost/numeric/ublas/matrix.hpp>

using Matrix = boost::numeric::ublas::matrix<unsigned long long>;

Matrix multiply(const Matrix& A, const Matrix& B)
{
    Matrix C(2, 2);
    C(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
    C(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
    C(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
    C(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
    return C;
}

Matrix power(Matrix base, unsigned long long exponent)
{
    Matrix result(2, 2);
    result(0, 0) = 1; result(0, 1) = 0;
    result(1, 0) = 0; result(1, 1) = 1;

    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

unsigned long long fibonacci(unsigned long long n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    Matrix base(2, 2);
    base(0, 0) = 1; base(0, 1) = 1;
    base(1, 0) = 1; base(1, 1) = 0;

    Matrix result = power(base, n - 1);
    return result(0, 0);
}

int main()
{
    assert(fibonacci(0) == 0);
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(3) == 2);
    assert(fibonacci(4) == 3);
    assert(fibonacci(5) == 5);
    assert(fibonacci(6) == 8);
    assert(fibonacci(7) == 13);
    assert(fibonacci(8) == 21);
    assert(fibonacci(9) == 34);
    assert(fibonacci(10) == 55);
    assert(fibonacci(20) == 6765);
}


/*
Обоснование алгоритмической сложности вычисления N-го числа Фибоначчи
методом матричной экспоненциации:

1. Матричная экспоненциация (данная реализация):
    - Умножение двух матриц 2x2 выполняется за константное время O(1),
       так как количество операций фиксировано (8 умножений, 4 сложения).
    - Возведение матрицы в степень N методом бинарного возведения требует
       O(log N) умножений матриц.
    - Итоговая сложность: O(log N).
2. Итеративное суммирование (динамическое программирование):
     - Требует O(N) итераций с двумя переменными.
     - Сложность: O(N).
 
 3. Наивная рекурсия:
     - Каждый вызов порождает два рекурсивных подвызова.
     - Сложность: O(2^N), экспоненциальный рост делает метод неприменимым
       уже для N > 40.
 
 4. Формула Бине (через золотое сечение):
     - Использует возведение в степень чисел с плавающей точкой O(log N),
     но страдает от потери точности для больших N.
 */
