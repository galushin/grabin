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

#ifndef Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED

/** @file grabin/statistics/mean.hpp
 @brief Накопитель для вычисления среднего
*/

#include <grabin/math/average_type.hpp>

#include <cstdint>

namespace grabin
{
inline namespace v1
{
namespace statistics
{
    /** @brief Накопитель для вычисления математического ожидания
    @tparam T тип значений, для которых вычисляется среднее
    @tparam Count тип количества элементов
    */
    template <class T, class Count = std::ptrdiff_t>
    class mean_accumulator
    {
    public:
        // Типы
        /// @brief Тип значений
        using value_type = T;

        /// @brief Тип для представления количества элементов
        using count_type = Count;

        /// @brief Тип для представления среднего значения
        using mean_type = average_type_t<T, Count>;

        // Создание, копирование, уничтожение
        /** @brief Конструктор без аргументов
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == mean_type(0) </tt>
        */
        mean_accumulator() = default;

        /** @brief Конструктор с явным указанием нулевого элемента
        @param zero нулевой элемент
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == zero </tt>
        */
        explicit mean_accumulator(value_type zero)
         : count_(0)
         , mean_(zero)
        {}

        // Свойства
        /** @brief Количество обработанных элементов
        @return Количество обработанных элементов, равное количеству вызовов <tt>operator()</tt>
        */
        count_type const & count() const
        {
            return this->count_;
        }

        /** @brief Среднее значение
        @return Среднее значение обработанных к данному моменту значений
        */
        mean_type const & mean() const
        {
            return this->mean_;
        }

        // Обновление
        /** @brief Обработка нового значения
        @param value новое значение
        @return <tt> *this </tt>
        */
        mean_accumulator & operator()(value_type const & value)
        {
            ++ this->count_;
            this->mean_ += (value - this->mean_) / this->count_;
            return *this;
        }

    private:
        count_type count_ = count_type(0);
        mean_type mean_ = T(0);
    };
}
// namespace accumulator
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
