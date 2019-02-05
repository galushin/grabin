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
        return std::inner_product(x.begin(), x.end(), y.begin(), zero);
    }

    /** @brief Решение СЛАУ методом минимизации невязки
    @param A матрица системы
    @param b вектор правой части
    @pre <tt>A.dim2() == b.dim()</tt>
    @return Приближённое решение СЛАУ <tt>A*x == b</tt>
    */
    template <class Matrix, class Vector>
    Vector minimal_residue(Matrix const & A, Vector b)
    {
        double const tol = 1e-3;
        auto const max_iter = 100;

        Vector x(b.dim());

        for(auto n = max_iter; n > 0; --n)
        {
            auto r = A * x - b;
            if(linear_algebra::inner_prod(r, r) < tol*tol)
            {
                break;
            }

            auto lambda = linear_algebra::inner_prod(r, A*r)
                        / linear_algebra::inner_prod(A*r, A*r);
            x -= lambda * r;
        }

        return x;
    }
}
// namespace numeric
}
// namespace v1
}
// namespace grabin


#endif
// Z_GRABIN_NUMERIC_LINEAR_ALGEBRA_HPP_INCLUDED
