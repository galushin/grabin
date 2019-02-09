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

#ifndef Z_GRABIN_STATISTICS_LINEAR_REGRESSION_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_LINEAR_REGRESSION_HPP_INCLUDED

/** @file grabin/statistics/linear_regression.hpp
 @brief Функциональность, связанная с линейной регрессией
*/

#include <grabin/statistics/mean.hpp>
#include <grabin/statistics/variance.hpp>
#include <grabin/utility/use_default.hpp>

#include <cassert>
#include <cstddef>

namespace grabin
{
inline namespace v1
{
namespace statistics
{
    class division_solver
    {
    public:
        /** @brief Решение уравнения a*x == b относительно @c x
        @pre <tt>a != 0</tt>
        */
        template <class T>
        T operator()(T const & a, T const & b) const
        {
            assert(a != T(0));

            return b / a;
        }
    };

    /** @brief Класс-накопитель для вычисления дисперсии
    @tparam X тип входной переменной
    @tparam InnerProduct тип функционального объекта, задающего скалярное произведение
    @tparam OuterProduct тип функционального объекта, задающего внешнее произведение
    @tparam Solver тип функционального объекта, задающего метод решения линейного уравнения
    @tparam Count тип для представления количества элементов
    @todo Можно ли избавиться от дублирования счётчика в x_stat_ и y_stat_?
    @todo Возможность выводить тип операций в зависимости от типа X: default_inner_product_t<X> и т.д.
    @todo Сформулировать пост-условия конструкторов
    */
    template <class X, class Count = use_default, class InnerProduct = use_default,
              class OuterProduct = use_default, class Solver = use_default>
    class linear_regression_accumulator
    {
    public:
        // Типы
        /// @brief Тип для представления количества элементов
        using count_type = grabin::replace_use_default_t<Count, std::intmax_t>;

        /// @brief Тип функционального объекта, вычисляющего скалярное произведение
        using inner_prod_type = grabin::replace_use_default_t<InnerProduct, std::multiplies<>>;

        /// @brief Тип функционального объекта, вычисляющего внешнее произведение
        using outer_prod_type = grabin::replace_use_default_t<OuterProduct, std::multiplies<>>;

        /// @brief Тип для представления свободного члена уравнения регрессии
        using intercept_type = decltype(std::declval<inner_prod_type>()(std::declval<X>(), std::declval<X>()));

        /** @brief Тип для представления коэффициента наклона уравнения регрессии
        @todo Более точное определение этого типа, это должно быть пространство, сопряжённое к X
        */
        using slope_type = X;

        // Создание, копирование, уничтожение
        /// @brief Конструктор без аргументов
        linear_regression_accumulator() = default;

        /** @brief Конструктор с явным заданием нулевого элемента
        @param zero нулевой элемент
        @todo Может быть инициализировать y_stat_ выражением inner_prod(zero, zero) ?
        */
        linear_regression_accumulator(X const & zero)
         : inner_prod_()
         , x_stat_(zero)
         , y_stat_()
         , cov_sum_(intercept_type(0)*zero)
        {}

        // Свойства
        /// @brief Тип функционального объекта, задающего операцию скалярного произведения
        inner_prod_type const & inner_prod() const
        {
            return this->inner_prod_;
        }

        /** @brief Количество обработанных элементов
        @return Количество обработанных элементов, равное количеству вызовов <tt>operator()</tt>
        */
        count_type const & count() const
        {
            return this->y_stat_.count();
        }

        intercept_type intercept() const
        {
            return y_stat_.mean() - this->inner_prod()(this->slope(), x_stat_.mean());
        }

        slope_type slope() const
        {
            if(this->count() < 2)
            {
                // @todo Оптимизировать
                return this->x_stat_.mean() - this->x_stat_.mean();
            }

            // @todo Что если x_stat_.variance() == 0? Покрыть этот случай тестом
            return this->solver_(this->x_stat_.variance(), this->covariance());
        }

        // Обновление
        /** @brief Обработка нового значения
        @param x новое значение входной переменной
        @param y новое значение выходной переменной
        @return <tt> *this </tt>
        */
        linear_regression_accumulator & operator()(X const & x, intercept_type const & y)
        {
            auto const y_mean_old = this->y_stat_.mean();

            x_stat_(x);
            y_stat_(y);

            cov_sum_ += (x - this->x_stat_.mean()) * (y - y_mean_old);

            return *this;
        }

    private:
        using X_stat = grabin::statistics::variance_accumulator<X, count_type, outer_prod_type>;
        using Y_stat = grabin::statistics::mean_accumulator<intercept_type, count_type>;
        using covariance_type = decltype(std::declval<intercept_type>() * std::declval<X>());

        using solver_type = grabin::replace_use_default_t<Solver, grabin::statistics::division_solver>;

        inner_prod_type inner_prod_;
        X_stat x_stat_;
        Y_stat y_stat_;
        covariance_type cov_sum_ = covariance_type(0);
        solver_type solver_;

        // @todo Возможно, эту операцию тоже следует обобщить?

        covariance_type covariance() const
        {
            if(this->count() == 0)
            {
                // @todo assert(cov_sum_ == 0)?
                return this->cov_sum_;
            }

            return this->cov_sum_ / this->count();
        }
    };
}
// namespace statistics
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_LINEAR_REGRESSION_HPP_INCLUDED
