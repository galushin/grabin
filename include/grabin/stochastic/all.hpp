/* (c) 2018 Галушин Павел Викторович, galushin@gmail.com

Данный файл -- часть библиотеки Grabin.

Grabin -- это свободной программное обеспечение: вы можете перераспространять ее и/или изменять ее
на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом
свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более
поздней версии.

Это программное обеспечение распространяется в надежде, что оно будет полезной, но БЕЗО ВСЯКИХ
ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ.
Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этим программным
обеспечение. Если это не так, см. https://www.gnu.org/licenses/.
*/

#ifndef Z_GRABIN_STOCHASTIC_ALL_HPP_INCLUDED
#define Z_GRABIN_STOCHASTIC_ALL_HPP_INCLUDED

/** @file grabin/stochastic/all.hpp
 @brief Функционал, связанный с теорией вероятностей и теорией случайных процессов
*/

#include <grabin/numeric/linear_algebra.hpp>

namespace grabin
{
inline namespace v1
{
namespace stochastic
{
    /** @brief Определение стационарных вероятностей марковского процесса с дискретными состояниями
    и непрерывным временем
    @param lambda матрица интенсивности переходов
    @param solver метод решения систем линейных алгебраических уравнений, по умолчанию используется
    LU-разложение
    @pre <tt>lambda.dim1() == lambda.dim2()</tt>
    @todo Возможность задавать тип вектора
    @return Вектор, компоненты которого равны вероятностям состояний в стационарном режиме
    */
    template <class Matrix, class Solver = grabin::linear_algebra::LU_solver>
    grabin::math_vector<typename Matrix::value_type>
    ctmc_stationary(Matrix const & lambda, Solver const & solver = Solver())
    {
        auto const n = lambda.dim1();
        assert(lambda.dim2() == n);

        Matrix A(n, n);

        for(auto i : grabin::view::indices(n-1))
        {
            for(auto j : grabin::view::indices(n))
            {
                if(j != i)
                {
                    A(i, i) -= lambda(i, j);
                    A(i, j) = lambda(j, i);
                }
            }
        }

        for(auto j : grabin::view::indices(n))
        {
            A(n-1, j) = 1;
        }

        grabin::math_vector<typename Matrix::value_type> b(n, 0);
        b[n-1] = 1;

        return solver(A, b);
    }
}
// namespace stochastic
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_STOCHASTIC_ALL_HPP_INCLUDED
