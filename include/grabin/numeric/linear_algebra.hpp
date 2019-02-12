/* (c) 2019 Галушин Павел Викторович, galushin@gmail.com

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

#ifndef Z_GRABIN_NUMERIC_LINEAR_ALGEBRA_HPP_INCLUDED
#define Z_GRABIN_NUMERIC_LINEAR_ALGEBRA_HPP_INCLUDED

/** @file grabin/numeric/linear_algebra.hpp
 @brief Численные методы линейной алгебры
*/

#include <grabin/numeric.hpp>
#include <numeric>

namespace grabin
{
inline namespace v1
{
namespace linear_algebra
{
    /** @brief Скалярное произведение векторов
    @param x, y аргументы
    @pre <tt>x.dim() == y.dim()</tt>
    @return <tt> std::inner_product(x.begin(), x.end(), y.begin(), zero)</tt>,
    где <tt>zero == typename Vector::value_type(0)</tt>
    */
    template <class Vector>
    typename Vector::value_type
    inner_prod(Vector const & x, Vector const & y)
    {
        if(x.dim() != y.dim())
        {
            throw std::logic_error("Dimensions must be equal");
        }

        auto const zero = typename Vector::value_type(0);
        return grabin::inner_product(x, y, zero);
    }

    /// @brief Тип функционального объекта, выполняющего внутреннее (скалярное) произведение
    struct inner_product
    {
        /** @brief Скалярное произведение векторов
        @param x, y аргументы
        @pre <tt>x.dim() == y.dim()</tt>
        @return <tt> std::inner_product(x.begin(), x.end(), y.begin(), zero)</tt>,
        где <tt>zero == typename Vector::value_type(0)</tt>
        */
        template <class Vector>
        typename Vector::value_type
        operator()(Vector const & x, Vector const & y) const
        {
            return grabin::linear_algebra::inner_prod(x, y);
        }
    };

    /** @brief Решение СЛАУ методом минимизации невязки
    @param A матрица системы
    @param b вектор правой части
    @pre <tt>A.dim2() == b.dim()</tt>
    @return Приближённое решение СЛАУ <tt>A*x == b</tt>
    */
    template <class Matrix, class Vector>
    Vector minimal_residue(Matrix const & A, Vector const & b)
    {
        double const tol = 1e-10;
        auto const max_iter = 100;

        Vector x(b.dim());

        for(auto n = max_iter; n > 0; --n)
        {
            auto r = A * x - b;

            auto lambda = linear_algebra::inner_prod(r, A*r)
                        / linear_algebra::inner_prod(A*r, A*r);

            if(linear_algebra::inner_prod(r, r) * lambda * lambda < tol*tol)
            {
                break;
            }

            x -= lambda * r;
        }

        return x;
    }

    struct minimal_residue_solver
    {
        template <class Matrix, class Vector>
        Vector operator()(Matrix const & A, Vector const & b) const
        {
            return grabin::linear_algebra::minimal_residue(A, b);
        }
    };

    struct LU_solver
    {
        template <class Matrix, class Vector>
        Vector operator()(Matrix const & A, Vector const & b) const
        {
            auto const n = A.dim1();

            assert(b.dim() == n);
            assert(A.dim2() == n);

            // Находим LU-разложение
            Matrix LU(n, n);

            for(auto const & j : grabin::view::indices(n))
            {
                LU(0, j) = A(0, j);

                if(j == 0)
                {
                    continue;
                }

                assert(LU(0, 0) != 0);
                LU(j, 0) = A(j, 0) / LU(0, 0);
            }

            for(auto const & i : grabin::view::indices(static_cast<typename Matrix::size_type>(1), n))
            {
                for(auto const & j : grabin::view::indices(i, n))
                {
                    LU(i, j) = A(i, j);
                    for(auto const & k : grabin::view::indices(i))
                    {
                        LU(i, j) -= LU(i, k) * LU(k, j);
                    }

                    if(j == i)
                    {
                        continue;
                    }

                    LU(j, i) = A(j, i);
                    for(auto const & k : grabin::view::indices(i))
                    {
                        LU(j, i) -= LU(j, k) * LU(k, i);
                    }
                    LU(j, i) /= LU(i, i);
                }
            }

            // Решаем Ly=b
            Vector y = b;

            for(auto const & i : grabin::view::indices(n))
            {
                for(auto const & j : grabin::view::indices(i))
                {
                    y[i] -= LU(i, j) * y[j];
                }
            }

            // Решаем Ux=y
            Vector x = y;

            for(auto i = n; i > 0; -- i)
            {
                for(auto j = i; j < n; ++ j)
                {
                    x[i-1] -= LU(i-1, j) * x[j];
                }
                x[i-1] /= LU(i-1, i-1);
            }

            return x;
        }
    };
}
// namespace numeric
}
// namespace v1
}
// namespace grabin


#endif
// Z_GRABIN_NUMERIC_LINEAR_ALGEBRA_HPP_INCLUDED
