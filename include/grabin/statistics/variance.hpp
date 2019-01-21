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

#ifndef Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED

/** @file grabin/statistics/variance.hpp
 @brief Накопитель для вычисления дисперсии (и среднеквадратического отклонения).
*/

#include <grabin/math.hpp>
#include <grabin/statistics/mean.hpp>

#include <cmath>
#include <cstdint>

namespace grabin
{
inline namespace v1
{
namespace statistics
{
    /** @brief Класс-накопитель для вычисления дисперсии, а также среднеквадратического отклонения
    и среднего.
    @tparam T тип значений, для которых вычисляется среднее
    @tparam Count тип количества элементов
    */
    template <class T, class Count = std::ptrdiff_t>
    class variance_accumulator
    {
        using Mean = grabin::statistics::mean_accumulator<T, Count>;

    public:
        // Типы
        /// @brief Тип значений
        using value_type = T;

        /// @brief Тип для представления количества элементов
        using count_type = typename Mean::count_type;

        /// @brief Тип для представления среднего значения
        using mean_type = typename Mean::mean_type;

        /// @brief Тип для представления дисперсии
        using variance_type = mean_type;

        // Создание, копирование, уничтожение

        // Свойства
        /** @brief Количество обработанных элементов
        @return Количество обработанных элементов, равное количеству вызовов <tt>operator()</tt>
        */
        count_type const & count() const
        {
            return this->mean_.count();
        }

        /** @brief Среднее значение
        @return Среднее значение обработанных к данному моменту значений
        */
        mean_type const & mean() const
        {
            return this->mean_.mean();
        }

        /** @brief Дисперсия
        @return Значение дисперсии обработанных к данному моменту значений
        */
        variance_type variance() const
        {
            if(this->count() == 0)
            {
                // @todo assert(s2_ == 0)?
                return this->s2_;
            }
            else
            {
                return this->s2_ / this->count();
            }
        }

        /** @brief Среднеквадратическое отклонение
        @return <tt> sqrt(this->variance()) </tt>
        */
        variance_type standard_deviation() const
        {
            using std::sqrt;
            return sqrt(this->variance());
        }

        // Обновление
        /** @brief Обработка нового значения
        @param value новое значение
        @return <tt> *this </tt>
        */
        variance_accumulator & operator()(value_type const & value)
        {
            auto const mean_old = this->mean();

            this->mean_(value);

            s2_ += (value - this->mean()) * (value - mean_old);

            return *this;
        }

    private:
        Mean mean_;
        variance_type s2_ = variance_type(0);
    };
}
// namespace statistics
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_VARIANCE_HPP_INCLUDED
