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

#include <cstddef>

namespace grabin
{
inline namespace v1
{
namespace statistics
{
    /** @brief Класс-накопитель для вычисления дисперсии
    @tparam X тип входной переменной
    @tparam Y тип выходной переменной
    @tparam Count тип для представления количества элементов
    @todo Можно ли избавиться от дублирования счётчика в x_stat_ и y_stat_?
    */
    template <class X, class Y = X, class Count = std::ptrdiff_t>
    class linear_regression_accumulator
    {
        using X_stat = grabin::statistics::variance_accumulator<X, Count>;
        using Y_stat = grabin::statistics::mean_accumulator<Y, Count>;

    public:
        // Типы
        /// @brief Тип для представления количества элементов
        using count_type = typename X_stat::count_type;

        /// @brief Тип для представления свободного члена уравнения регрессии
        using intercept_type = Y;

        /** @brief Тип для представления коэффициента наклона уравнения регрессии
        @todo Более точное определение этого типа
        */
        using slope_type = Y;

        // Создание, копирование, уничтожение

        // Свойства
        /** @brief Количество обработанных элементов
        @return Количество обработанных элементов, равное количеству вызовов <tt>operator()</tt>
        */
        count_type const & count() const
        {
            return this->y_stat_.count();
        }

        intercept_type intercept() const
        {
            return y_stat_.mean() - this->slope() * x_stat_.mean();
        }

        slope_type slope() const
        {
            if(this->count() < 2)
            {
                return slope_type(0);
            }

            // @todo Что если x_stat_.variance() == 0? Покрыть этот случай тестом
            return this->covariance() / this->x_stat_.variance();
        }

        // Обновление
        /** @brief Обработка нового значения
        @param x новое значение входной переменной
        @param y новое значение выходной переменной
        @return <tt> *this </tt>
        */
        linear_regression_accumulator & operator()(X const & x, Y const & y)
        {
            auto const y_mean_old = this->y_stat_.mean();

            x_stat_(x);
            y_stat_(y);

            cov_sum_ += (x - this->x_stat_.mean()) * (y - y_mean_old);

            return *this;
        }

    private:
        X_stat x_stat_;
        Y_stat y_stat_;

        using covariance_type = Y;
        covariance_type cov_sum_ = covariance_type(0);

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
